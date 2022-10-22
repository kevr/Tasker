#include "defaults.hpp"
#include <gtest/internal/gtest-port.h>
#define main main_real
#include "main.cpp"
#undef main

#include "mocks/ncurses.hpp"
#include "utility.hpp"
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

class main_test : public ::testing::Test
{
protected:
    std::filesystem::path tmpdir;
    std::filesystem::path conf_path;

protected:
    void write_config(const std::map<std::string, std::string> &options = {})
    {
        {
            std::ofstream ofs(conf_path.c_str(), std::ios::out);
            for (auto &kv : options) {
                ofs << kv.first << " = " << kv.second << std::endl;
            }
            ofs << std::endl;
            ofs.close();
        }
    }

public:
    void SetUp() override
    {
        cfg::config::new_ref();
        tmpdir = test::make_temp_directory();
        conf_path = tmpdir / "config";
    }

    void TearDown() override
    {
        std::filesystem::remove_all(tmpdir);
    }
};

TEST_F(main_test, initscr_fails)
{
    tasker::ext::mock_ncurses ncurses;
    EXPECT_CALL(ncurses, initscr()).WillOnce(Return(nullptr));

    MAKE_ARGS();
    auto rc = tasker_main(ncurses, argc, argv);
    ASSERT_EQ(rc, ERROR_INITSCR);
}

TEST_F(main_test, getmaxyx_fails)
{
    tasker::ext::mock_ncurses ncurses;

    WINDOW win;
    EXPECT_CALL(ncurses, initscr()).WillOnce(Return(&win));
    EXPECT_CALL(ncurses, get_max_yx(_, _, _))
        .WillOnce(Invoke([](WINDOW *, int &y, int &x) {
            x = y = -1;
        }));
    EXPECT_CALL(ncurses, endwin()).WillOnce(Return(0));

    MAKE_ARGS();
    auto rc = tasker_main(ncurses, argc, argv);
    ASSERT_EQ(rc, ERROR_GETMAXYX);
}

TEST_F(main_test, keypad_fails)
{
    tasker::ext::mock_ncurses ncurses;
    tasker::ext::ncurses stub;

    WINDOW win;
    EXPECT_CALL(ncurses, initscr()).WillOnce(Return(&win));
    EXPECT_CALL(ncurses, get_max_yx(_, _, _))
        .WillOnce(Invoke([&stub](WINDOW *win, int &y, int &x) {
            stub.get_max_yx(win, y, x);
        }));
    EXPECT_CALL(ncurses, keypad(_, _)).WillOnce(Return(ERR));
    EXPECT_CALL(ncurses, endwin()).WillOnce(Return(0));

    MAKE_ARGS();
    auto rc = tasker_main(ncurses, argc, argv);
    ASSERT_EQ(rc, ERROR_KEYPAD);
}

TEST_F(main_test, raw_fails)
{
    tasker::ext::mock_ncurses ncurses;
    tasker::ext::ncurses stub;

    WINDOW win;
    EXPECT_CALL(ncurses, initscr()).WillOnce(Return(&win));
    EXPECT_CALL(ncurses, get_max_yx(_, _, _))
        .WillOnce(Invoke([&stub](WINDOW *win, int &y, int &x) {
            stub.get_max_yx(win, y, x);
        }));
    EXPECT_CALL(ncurses, keypad(_, _)).WillOnce(Return(OK));
    EXPECT_CALL(ncurses, raw()).WillOnce(Return(ERR));
    EXPECT_CALL(ncurses, endwin()).WillOnce(Return(0));

    MAKE_ARGS();
    auto rc = tasker_main(ncurses, argc, argv);
    ASSERT_EQ(rc, ERROR_RAW);
}

TEST_F(main_test, noecho_fails)
{
    tasker::ext::mock_ncurses ncurses;
    tasker::ext::ncurses stub;

    WINDOW win;
    EXPECT_CALL(ncurses, initscr()).WillOnce(Return(&win));
    EXPECT_CALL(ncurses, get_max_yx(_, _, _))
        .WillOnce(Invoke([&stub](WINDOW *win, int &y, int &x) {
            stub.get_max_yx(win, y, x);
        }));
    EXPECT_CALL(ncurses, keypad(_, _)).WillOnce(Return(OK));
    EXPECT_CALL(ncurses, raw()).WillOnce(Return(OK));
    EXPECT_CALL(ncurses, noecho()).WillOnce(Return(ERR));
    EXPECT_CALL(ncurses, endwin()).WillOnce(Return(0));

    MAKE_ARGS();
    auto rc = tasker_main(ncurses, argc, argv);
    ASSERT_EQ(rc, ERROR_ECHO);
}

TEST_F(main_test, runs)
{
    MAKE_ARGS();
    auto rc = main_real(argc, argv);
    ASSERT_EQ(rc, OK);
}

TEST_F(main_test, help)
{
    testing::internal::CaptureStdout();

    MAKE_ARGS("--help");
    auto rc = main_real(argc, argv);
    ASSERT_EQ(rc, OK);

    auto output = testing::internal::GetCapturedStdout();
    auto lines = split(output, '\n');
    auto &conf = cfg::config::ref();
    ASSERT_EQ(lines[0], "usage: " + conf.usage());
    ASSERT_EQ(lines[1], "");
    ASSERT_EQ(lines[2], "Command-line options:");
    ASSERT_NE(lines[3].find("-h [ --help ]"), std::string::npos);
    ASSERT_NE(lines[4].find("-v [ --version ]"), std::string::npos);
    ASSERT_NE(lines[5].find("-d [ --debug ]"), std::string::npos);
    ASSERT_NE(lines[6].find("-l [ --logfile ] arg"), std::string::npos);
    ASSERT_NE(lines[7].find("-c [ --config ] arg"), std::string::npos);
    ASSERT_NE(lines[8].find("--show-config"), std::string::npos);
    ASSERT_EQ(lines[9], "");
}

TEST_F(main_test, version)
{
    testing::internal::CaptureStdout();

    MAKE_ARGS("--version");
    auto rc = main_real(argc, argv);
    ASSERT_EQ(rc, OK);

    auto output = strip(testing::internal::GetCapturedStdout(), '\n');
    ASSERT_EQ(output, VERSION);
}

TEST_F(main_test, custom_config)
{
    write_config();

    MAKE_ARGS("--config", conf_path.c_str());
    auto rc = main_real(argc, argv);
    ASSERT_EQ(rc, OK);
}

TEST_F(main_test, custom_config_unknown_option)
{
    std::map<std::string, std::string> options;
    options["fake-option"] = "blahblah";
    write_config(options);

    std::filesystem::path path(tmpdir);
    path /= "config";

    MAKE_ARGS("--config", path.c_str());
    auto rc = main_real(argc, argv);
    ASSERT_EQ(rc, ERROR_CONFIG);
}

TEST_F(main_test, logfile)
{
    auto logpath = tmpdir / "test.log";

    MAKE_ARGS("--logfile", logpath.c_str());
    auto rc = main_real(argc, argv);
    ASSERT_EQ(rc, OK);

    std::ifstream ifs(logpath.c_str());
    std::string output;
    std::getline(ifs, output);
    ASSERT_EQ(output, "[INFO] ===== BEGIN SESSION =====");
}

TEST_F(main_test, enable_debug_logging)
{
    const auto logpath = tmpdir / "tasker.log";
    MAKE_ARGS("--debug", "--logfile", logpath.c_str());
    auto rc = main_real(argc, argv);
    ASSERT_EQ(rc, OK);

    std::ifstream ifs(logpath.c_str());
    std::string line;
    std::string content;
    while (std::getline(ifs, line)) {
        content += line;
        content.push_back('\n');
    }

    ASSERT_NE(content.find("[DEBUG]"), std::string::npos);
}

TEST_F(main_test, resize)
{
    ext::mock_ncurses ncurses;

    WINDOW win;
    EXPECT_CALL(ncurses, initscr()).WillRepeatedly(Return(&win));
    EXPECT_CALL(ncurses, get_max_yx(_, _, _))
        .WillRepeatedly(Invoke([](WINDOW *win, int &y, int &x) {
            x = 800;
            y = 600;
        }));
    EXPECT_CALL(ncurses, keypad(_, _)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, raw()).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, noecho()).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, endwin()).WillRepeatedly(Return(OK));

    WINDOW child;
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _)).WillRepeatedly(Return(&child));
    EXPECT_CALL(ncurses, delwin(_)).WillRepeatedly(Return(OK));

    EXPECT_CALL(ncurses, refresh()).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, wrefresh(_)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, w_add_str(_, _)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, wborder(_, _, _, _, _, _, _, _, _))
        .WillRepeatedly(Return(OK));

    EXPECT_CALL(ncurses, getchar())
        .Times(2)
        .WillOnce(Return(KEY_RESIZE))
        .WillOnce(Return(defaults::keybinds::KEY_QUIT));

    MAKE_ARGS();
    ASSERT_EQ(tasker_main(ncurses, argc, argv), OK);
}

TEST_F(main_test, error_args)
{
    ext::ncurses ncurses;

    MAKE_ARGS("--fake-argument");
    ASSERT_EQ(tasker_main(ncurses, argc, argv), ERROR_ARGS);
}

TEST_F(main_test, error_validate)
{
    ext::ncurses ncurses;

    MAKE_ARGS("--style.task_list.width", "-1");
    ASSERT_EQ(tasker_main(ncurses, argc, argv), ERROR_VALIDATE);
}

TEST_F(main_test, show_config)
{
    testing::internal::CaptureStdout();

    ext::ncurses ncurses;
    write_config();

    MAKE_ARGS("--config", conf_path.c_str(), "--show-config");
    ASSERT_EQ(tasker_main(ncurses, argc, argv), OK);

    auto output = testing::internal::GetCapturedStdout();
    auto lines = split(output, '\n');

    auto str = fmt::format("config: \"{0}\"", conf_path.c_str());
    ASSERT_EQ(lines[0], str);

    ASSERT_EQ(lines[1], "debug: false");
    ASSERT_EQ(lines[2], "logfile: stdout");
    ASSERT_EQ(lines[3], "color.root_border: 4");
    ASSERT_EQ(lines[4], "color.project_bar_bg: 4");
    ASSERT_EQ(lines[5], "color.project_bar_fg: 0");

    str = fmt::format("style.task_list.width: {0}",
                      static_cast<int>(defaults::TASK_LIST_WIDTH));
    ASSERT_EQ(lines[6], str);

    str = fmt::format("keybindings.quit: '{0}'",
                      static_cast<char>(defaults::KEY_QUIT));
    ASSERT_EQ(lines[7], str);

    str = fmt::format("keybindings.project.new_list: '{0}'",
                      static_cast<char>(defaults::KEY_NEW_LIST));
    ASSERT_EQ(lines[8], str);
}

TEST_F(main_test, show_config_custom_logfile)
{
    testing::internal::CaptureStdout();

    ext::ncurses ncurses;
    write_config();

    MAKE_ARGS("--config",
              conf_path.c_str(),
              "--logfile",
              "test.log",
              "--show-config");
    ASSERT_EQ(tasker_main(ncurses, argc, argv), OK);

    auto output = testing::internal::GetCapturedStdout();
    auto lines = split(output, '\n');

    ASSERT_EQ(lines[2], "logfile: \"test.log\"");
}
