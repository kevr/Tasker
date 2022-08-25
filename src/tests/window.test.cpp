#include "../tui/window.hpp"
#include <gtest/gtest.h>
using namespace tasker;

TEST(root_window, ncurses_null)
{
    tui::root_window<ext::ncurses> root;
    ASSERT_EQ(root.init(), ERROR);
    ASSERT_EQ(root.refresh(), ERROR);
}
