#include "config/config.hpp"
#include "config.hpp"
#include "testing.hpp"
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
        std::filesystem::remove_all(tmpdir);
    }
};

template <typename T>
std::string capture_ostream(const T &t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

TEST(config, parse_args)
{
    MAKE_ARGS();
    char **av = const_cast<char **>(argv);
    cfg::config::ref().parse_args(argc, av);
}

TEST(config, parse_args_const)
{
    MAKE_ARGS();
    cfg::config::ref().parse_args(argc, argv);
}

TEST(config, set_ref)
{
    // Set the reference to itself
    cfg::config::ref(cfg::config::ref());
}

TEST_F(config_test, usage)
{
    std::string expected =
        PROG + (" [-hvc] [--color.root_border] [--color.project_bar_bg] "
                "[--color.project_bar_fg] [--style.task_list.width] "
                "[--keybindings.quit] [--keybindings.project.new_list]");
    ASSERT_EQ(conf.usage(), expected);
}

TEST_F(config_test, config_usage)
{
    conf.option("test", "test help");
    std::string expected =
        PROG + (" [-hvc] [--color.root_border] [--color.project_bar_bg] "
                "[--color.project_bar_fg] [--style.task_list.width] "
                "[--keybindings.quit] [--keybindings.project.new_list] "
                "[--test]");
    ASSERT_EQ(conf.usage(), expected);
}

TEST_F(config_test, help)
{
    auto output = capture_ostream(conf);

    auto lines = split(output, '\n');
    ASSERT_EQ(lines.size(), 18);
    ASSERT_EQ(lines[0], "");
    ASSERT_EQ(lines[1], "Command-line options:");
    ASSERT_NE(lines[2].find("-h [ --help ]"), std::string::npos);
    ASSERT_NE(lines[3].find("-v [ --version ]"), std::string::npos);
    ASSERT_NE(lines[4].find("-d [ --debug ]"), std::string::npos);
    ASSERT_NE(lines[5].find("-l [ --logfile ] arg"), std::string::npos);
    ASSERT_NE(lines[6].find("-c [ --config ] arg"), std::string::npos);
    ASSERT_NE(lines[7].find("--show-config"), std::string::npos);
    ASSERT_EQ(lines[8], "");
    ASSERT_EQ(lines[9], "Config options:");
    ASSERT_NE(lines[10].find("--color.root_border arg"), std::string::npos);
    ASSERT_NE(lines[11].find("--color.project_bar_bg arg"), std::string::npos);
    ASSERT_NE(lines[12].find("--color.project_bar_fg arg"), std::string::npos);
    ASSERT_NE(lines[13].find("--style.task_list.width arg"),
              std::string::npos);
    ASSERT_NE(lines[14].find("--keybindings.quit arg"), std::string::npos);
    ASSERT_NE(lines[15].find("--keybindings.project.new_list arg"),
              std::string::npos);
    ASSERT_NE(lines[16].find("add a new list to the project board"),
              std::string::npos);
    ASSERT_EQ(lines[17], "");
}

TEST_F(config_test, config_add_option)
{
    conf.option("test", "test help");
    auto output = capture_ostream(conf);

    auto lines = split(output, '\n');

    // Look for --test in the second to last line, which is where
    // the last option defined shows up in the --help display.
    auto line = lines[lines.size() - 2];
    ASSERT_NE(line.find("--test"), std::string::npos);
}

TEST_F(config_test, getattr)
{
    conf.option("test", po::value<std::string>(), "test help");

    MAKE_ARGS("--test", "test-data");
    conf.parse_args(argc, argv);

    // Exercise operator[]
    ASSERT_EQ(conf["test"], "test-data");
}

TEST_F(config_test, get)
{
    conf.option("test", po::value<int>(), "test help");

    MAKE_ARGS("--test", "9");
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

TEST(config, default_value)
{
    auto &conf = cfg::config::new_ref();
    auto some_option = conf.get<std::string>("some_option", "some_default");
    ASSERT_EQ(some_option, "some_default");
}

TEST(config, get_value)
{
    namespace po = boost::program_options;
    auto &conf = cfg::config::new_ref();

    conf.option("test", po::value<int>()->default_value(10), "test help");
    MAKE_ARGS();
    conf.parse_args(argc, argv);

    auto test = conf.get<int>("test");
    ASSERT_EQ(test, 10);

    // Let's also exercise the default_value path where the key
    // exists, so the key's value is returned and not the default.
    test = conf.get<int>("test", 0);
    ASSERT_EQ(test, 10);
}
