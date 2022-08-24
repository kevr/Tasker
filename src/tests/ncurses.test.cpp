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
