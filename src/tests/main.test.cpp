#define main main_real
#include "main.cpp"
#undef main

#include <gtest/gtest.h>

TEST(main, runs)
{
    const char *_argv[] = { PROG, nullptr };
    auto argv = const_cast<char **>(_argv);

    auto rc = main_real(1, argv);
    ASSERT_EQ(rc, 0);
}
