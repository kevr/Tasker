#include "config/config.hpp"
#include "config.hpp"
#include "utility.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <vector>
using namespace tasker;
namespace po = boost::program_options;

class config_test : public ::testing::Test
{
protected:
    std::string tmpdir;
    cfg::config conf;

protected:
    void write_config(const std::map<std::string, std::string> &options)
    {
        std::filesystem::path p(tmpdir);
        p /= "config";
        {
            std::ofstream ofs(p.c_str(), std::ios::out);
            for (auto &kv : options) {
                ofs << kv.first << " = " << kv.second << std::endl;
            }
            ofs.close();
        }
    }

public:
    void SetUp() override
    {
        tmpdir = test::make_temp_directory();
    }

    void TearDown() override
    {
        if (tmpdir.size()) {
            std::filesystem::remove_all(tmpdir);
        }
    }
};

template <typename T>
std::string capture_ostream(const T &t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

TEST(config, set_ref)
{
    // Set the reference to itself
    cfg::config::ref(cfg::config::ref());
}

TEST_F(config_test, usage)
{
    std::string expected = PROG + " [-hv]";
    ASSERT_EQ(conf.usage(), expected);
}

TEST_F(config_test, config_usage)
{
    conf.option("test", "test help");
    std::string expected = PROG + " [-hv] [--test]";
    ASSERT_EQ(conf.usage(), expected);
}

TEST_F(config_test, help)
{
    auto output = capture_ostream(conf);

    auto lines = split(output, '\n');
    ASSERT_EQ(lines.size(), 5);
    ASSERT_EQ(lines[0], "");
    ASSERT_EQ(lines[1], "Program options:");
    ASSERT_NE(lines[2].find("-h [ --help ]"), std::string::npos);
    ASSERT_NE(lines[3].find("-v [ --version ]"), std::string::npos);
    ASSERT_EQ(lines[4], "");
}

TEST_F(config_test, config_help)
{
    conf.option("test", "test help");
    auto output = capture_ostream(conf);

    auto lines = split(output, '\n');
    ASSERT_EQ(lines.size(), 8);
    ASSERT_EQ(lines[0], "");
    ASSERT_EQ(lines[1], "Program options:");
    ASSERT_NE(lines[2].find("-h [ --help ]"), std::string::npos);
    ASSERT_NE(lines[3].find("-v [ --version ]"), std::string::npos);
    ASSERT_EQ(lines[4], "");
    ASSERT_EQ(lines[5], "Config options:");
    ASSERT_NE(lines[6].find("--test"), std::string::npos);
    ASSERT_EQ(lines[7], "");
}

TEST_F(config_test, getattr)
{
    conf.option("test", po::value<std::string>(), "test help");

    const char *_argv[] = { PROG.c_str(), "--test", "test-data", nullptr };
    auto argv = const_cast<char **>(_argv);
    int argc = 3;

    conf.parse_args(argc, argv);

    // Exercise operator[]
    ASSERT_EQ(conf["test"], "test-data");
}

TEST_F(config_test, get)
{
    conf.option("test", po::value<int>(), "test help");

    const char *_argv[] = { PROG.c_str(), "--test", "9", nullptr };
    auto argv = const_cast<char **>(_argv);
    int argc = 3;

    conf.parse_args(argc, argv);

    ASSERT_EQ(conf.get<int>("test"), 9);
}

TEST_F(config_test, parse_config)
{
    conf.option("test", po::value<int>(), "test help");

    std::map<std::string, std::string> options;
    options["test"] = "5";
    write_config(options);

    std::filesystem::path config_path(tmpdir);
    config_path /= "config";

    conf.parse_config(config_path);

    ASSERT_EQ(conf.get<int>("test"), 5);
}
