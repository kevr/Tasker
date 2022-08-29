#include "../tui.hpp"
#include "../mocks/ncurses.hpp"
#include <gtest/gtest.h>
using namespace tasker;

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
    EXPECT_CALL(ncurses, subwin(_, _, _, _, _)).WillOnce(Return(win));
    EXPECT_CALL(ncurses, delwin(_)).WillOnce(Return(OK));
}

class tui_test : public ::testing::Test
{
protected:
    ext::ncurses ncurses;
    tui::tui<ext::ncurses> term { ncurses };
};

class mock_tui_test : public ::testing::Test
{
protected:
    WINDOW mock_win;
    WINDOW mock_pane;

    ext::mock_ncurses ncurses;
    tui::tui<ext::ncurses> term { ncurses };

public:
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

TEST(tui, pane_init_fails)
{
    WINDOW win_root;

    ext::mock_ncurses ncurses;
    expect_root(ncurses, &win_root);
    EXPECT_CALL(ncurses, subwin(_, _, _, _, _)).WillOnce(Return(nullptr));

    tui::tui<ext::ncurses> term(ncurses);
    ASSERT_FALSE(term.init());
}
