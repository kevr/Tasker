#define main main_real
#include "main.cpp"
#undef main

#include "../mocks/ncurses.hpp"
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Return;

TEST(main, initscr_fails)
{
    tasker::ext::mock_ncurses ncurses;
    EXPECT_CALL(ncurses, initscr()).WillOnce(Return(nullptr));

    const char *_argv[] = { PROG, nullptr };
    auto argv = const_cast<char **>(_argv);

    auto rc = tasker_main(ncurses, 1, argv);
    ASSERT_EQ(rc, 1);
}

TEST(main, keypad_fails)
{
    tasker::ext::mock_ncurses ncurses;

    WINDOW win;
    EXPECT_CALL(ncurses, initscr()).WillOnce(Return(&win));
    EXPECT_CALL(ncurses, keypad(_, _)).WillOnce(Return(ERR));

    const char *_argv[] = { PROG, nullptr };
    auto argv = const_cast<char **>(_argv);

    auto rc = tasker_main(ncurses, 1, argv);
    ASSERT_EQ(rc, 2);
}

TEST(main, runs)
{
    const char *_argv[] = { PROG, nullptr };
    auto argv = const_cast<char **>(_argv);

    auto rc = main_real(1, argv);
    ASSERT_EQ(rc, 0);

    // main() sets keypad(root_win, true); let's assert that it did.
    auto ncurses = tasker::ext::ncurses();
    ASSERT_TRUE(ncurses.keypad(ncurses.root()));
}
