#define main main_real
#include "main.cpp"
#undef main

#include "../ncurses.hpp"
#include <gtest/gtest.h>

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
