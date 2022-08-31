#include "../tui.hpp"
#include "../mocks/ncurses.hpp"
#include "config.hpp"
#include "config/keybinds.hpp"
#include <fmt/format.h>
#include <gtest/gtest.h>
using namespace tasker;
namespace po = boost::program_options;

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

template <typename CI>
void expect_root(CI &ncurses, WINDOW *win)
{
    EXPECT_CALL(ncurses, initscr()).WillOnce(Return(win));
    EXPECT_CALL(ncurses, keypad(_, _)).WillOnce(Return(OK));
    EXPECT_CALL(ncurses, raw()).WillOnce(Return(OK));
    EXPECT_CALL(ncurses, noecho()).WillOnce(Return(OK));
    EXPECT_CALL(ncurses, endwin()).WillOnce(Return(OK));
    EXPECT_CALL(ncurses, get_max_yx(_, _, _))
        .WillOnce(Invoke([](WINDOW *, int &y, int &x) {
            x = 800;
            y = 600;
        }));
}

template <typename CI>
void expect_pane(CI &ncurses, WINDOW *win)
{
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _)).WillRepeatedly(Return(win));
    EXPECT_CALL(ncurses, delwin(_)).WillOnce(Return(OK));
    EXPECT_CALL(ncurses, w_add_str(_, _)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, wborder(_, _, _, _, _, _, _, _, _))
        .WillRepeatedly(Return(OK));
}

void setup_config()
{
    auto &conf = cfg::config::new_ref();
    conf.option(
        "key_quit",
        po::value<char>()->default_value(cfg::default_keybinds::KEY_QUIT),
        "quit keybind");
    const char *_argv[] = { PROG.data(), nullptr };
    char **argv = const_cast<char **>(_argv);
    conf.parse_args(1, argv);
}

class tui_test : public ::testing::Test
{
protected:
    ext::ncurses ncurses;
    tui::tui<ext::ncurses> term { ncurses };

public:
    static void SetUpTestSuite()
    {
        setup_config();
    }
};

class mock_tui_test : public ::testing::Test
{
protected:
    WINDOW mock_win;
    WINDOW mock_pane;

    ext::mock_ncurses ncurses;
    tui::tui<ext::ncurses> term { ncurses };

public:
    static void SetUpTestSuite()
    {
        setup_config();
    }

    virtual void SetUp() override
    {
        expect_root(ncurses, &mock_win);
        expect_pane(ncurses, &mock_pane);
    }
};

TEST_F(tui_test, runs)
{
    ASSERT_EQ(term.init().end(), OK);
}

TEST_F(mock_tui_test, init_twice_noop)
{
    term.init();
    ASSERT_EQ(&term.init(), &term);
}

TEST_F(mock_tui_test, end_twice_noop)
{
    // By default, if the term is not yet initialized,
    // end is a noop and just returns tui::return_code().
    ASSERT_EQ(term.end(), int());

    term.init();
    ASSERT_EQ(term.return_code(), OK);

    // Now, end it for real.
    auto rc = term.end();

    // End again, expect the same return code since we hit
    // the noop path like we did at the beginning of this test.
    ASSERT_EQ(term.end(), rc);
}

TEST_F(mock_tui_test, w_add_str)
{
    std::string output;
    EXPECT_CALL(ncurses, w_add_str(_, _))
        .Times(2)
        .WillOnce(Return(OK))
        .WillOnce(Invoke([&output](WINDOW *win, const char *str) -> int {
            output = str;
            return OK;
        }));
    term.init();
    ASSERT_EQ(term.return_code(), OK);

    auto expected = fmt::format("Press '{0}' to quit...",
                                (char)cfg::default_keybinds::KEY_QUIT);
    EXPECT_EQ(output, expected);
}

TEST_F(mock_tui_test, waddstr_fails)
{
    EXPECT_CALL(ncurses, w_add_str(_, _)).WillRepeatedly(Return(ERR));
    term.init();
    ASSERT_EQ(term.return_code(), ERROR_WADDSTR);
}

TEST_F(mock_tui_test, project_init_fails)
{
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .WillOnce(Return(&mock_pane))
        .WillOnce(Return(nullptr));
    term.init();
    ASSERT_EQ(term.return_code(), ERROR_DERWIN);
}

TEST_F(mock_tui_test, bar_init_fails)
{
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .WillOnce(Return(&mock_pane))
        .WillOnce(Return(&mock_pane))
        .WillOnce(Return(nullptr));
    term.init();
    ASSERT_EQ(term.return_code(), ERROR_DERWIN);
}

TEST_F(mock_tui_test, content_init_fails)
{
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .WillOnce(Return(&mock_pane))
        .WillOnce(Return(&mock_pane))
        .WillOnce(Return(&mock_pane))
        .WillOnce(Return(nullptr));
    term.init();
    ASSERT_EQ(term.return_code(), ERROR_DERWIN);
}

TEST_F(mock_tui_test, root_draw_fails)
{
    term.init();

    EXPECT_CALL(ncurses, wborder(_, _, _, _, _, _, _, _, _))
        .Times(1)
        .WillOnce(Return(ERR));
    ASSERT_EQ(term.draw(), ERR);
}

TEST(tui, pane_init_fails)
{
    WINDOW win_root;

    ext::mock_ncurses ncurses;
    expect_root(ncurses, &win_root);
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _)).WillOnce(Return(nullptr));

    tui::tui<ext::ncurses> term(ncurses);
    ASSERT_FALSE(term.init());
}
