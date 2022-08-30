#include "../mocks/ncurses.hpp"
#include <gtest/gtest.h>

using ::testing::Return;

TEST(ncurses, missing_keypad_returns_false)
{
    tasker::ext::ncurses ncurses;
    ASSERT_FALSE(ncurses.keypad(nullptr));
}

TEST(ncurses, endwin_mock)
{
    tasker::ext::mock_ncurses ncurses;
    EXPECT_CALL(ncurses, endwin()).WillOnce(Return(OK));
    ASSERT_EQ(ncurses.endwin(), OK);
}

TEST(ncurses, delwin_fails)
{
    tasker::ext::ncurses ncurses;
    ASSERT_EQ(ncurses.delwin(nullptr), ERR);
}

TEST(ncurses, keypad)
{
    tasker::ext::ncurses ncurses;
    WINDOW win;
    ncurses.keypad(&win, true);
    ASSERT_TRUE(ncurses.keypad(&win));
}

TEST(ncurses, get_pair_err)
{
    tasker::ext::ncurses ncurses;
    ASSERT_EQ(ncurses.get_pair(1), ERR);
}
