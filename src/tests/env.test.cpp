#include "env.hpp"
#include "config.hpp"
#include "utility.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
using namespace tasker;

class env_test : public ::testing::Test
{
protected:
    std::filesystem::path tmpdir;

public:
    void SetUp() override
    {
        tmpdir = test::make_temp_directory();
    }

    void TearDown() override
    {
        std::filesystem::remove_all(tmpdir);
    }
};

TEST(variable, works)
{
    env::variable var("HOME");

    // Let's make sure that env::variable populates itself with
    // getenv("HOME") properly.
    auto cs = std::getenv("HOME");
    ASSERT_NE(cs, nullptr);
    ASSERT_EQ(var, cs);

    ASSERT_EQ(var.key(), "HOME");
    ASSERT_EQ(var.value(), cs);

    ASSERT_EQ(var, var);

    // This variable should always be available on Linux systems,
    // where these tests run.
    env::variable var2("USER");
    ASSERT_NE(var, var2);
}

TEST(variable, move_and_copy)
{
    env::variable var("HOME");

    // Create a var2 copy of var.
    env::variable var2(var);

    // Assert that var is identical to var2.
    ASSERT_EQ(var, var2);

    // Move var2 into var3.
    env::variable var3(std::move(var2));

    // Assert that now, var is identical to var3.
    ASSERT_EQ(var, var3);

    // And var2 is not identical to either, since it was moved from.
    ASSERT_NE(var2, var);
    ASSERT_NE(var2, var3);

    // Copy some around.
    var2 = var;

    // Once again, var is identical to var2.
    ASSERT_EQ(var, var2);

    // Move var3 into var2.
    var2 = std::move(var3);

    // var is still identical to var2.
    ASSERT_EQ(var, var2);

    // But var3 is not identical to the others, since it was moved from.
    ASSERT_NE(var3, var);
    ASSERT_NE(var3, var2);
}

TEST(variable, ostream)
{
    std::stringstream ss;
    env::variable home("HOME");
    ss << home;
    ASSERT_EQ(ss.str(), home.value());
}

TEST(variable, unset)
{
    ASSERT_THROW(env::variable("NON EXISTENT"), std::runtime_error);
}

TEST_F(env_test, search_config_path)
{
    auto orig_home = env::variable("HOME");
    setenv("HOME", tmpdir.c_str(), 1);

    auto path = tmpdir / ".config" / PROG;
    std::filesystem::create_directories(path);

    auto config_path = path / "config";
    touch(config_path);

    auto result = env::search_config_path();
    ASSERT_EQ(result.value(), config_path.string());
}
