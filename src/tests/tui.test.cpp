#include "../tui.hpp"
#include "../mocks/ncurses.hpp"
#include <gtest/gtest.h>
using namespace tasker;

using ::testing::_;
using ::testing::Return;

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

    ext::mock_ncurses ncurses;
    tui::tui<ext::ncurses> term { ncurses };

public:
    virtual void SetUp() override
    {
        EXPECT_CALL(ncurses, initscr()).WillOnce(Return(&mock_win));
        EXPECT_CALL(ncurses, keypad(_, _)).WillOnce(Return(OK));
        EXPECT_CALL(ncurses, endwin()).WillOnce(Return(OK));
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
