#include "../tui/window.hpp"
#include "../mocks/ncurses.hpp"
#include <gtest/gtest.h>
using namespace tasker;

using ::testing::_;
using ::testing::Return;

class window_test : public ::testing::Test
{
protected:
    ext::ncurses ncurses;

    using root_window_t = tui::root_window<ext::ncurses>;
    std::shared_ptr<root_window_t> root =
        std::make_shared<root_window_t>(ncurses);
};

class mock_window_test : public ::testing::Test
{
protected:
    ext::mock_ncurses ncurses;

    using root_window_t = tui::root_window<ext::ncurses>;
    std::shared_ptr<root_window_t> root =
        std::make_shared<root_window_t>(ncurses);
};

TEST(root_window, ncurses_null)
{
    tui::root_window<ext::ncurses> root;
    ASSERT_EQ(root.init(), ERROR);
    ASSERT_EQ(root.refresh(), ERR);
    ASSERT_EQ(root.end(), ERR);
}

TEST(root_window, stub_nulls_root)
{
    ext::ncurses ncurses;
    tui::root_window<ext::ncurses> root(ncurses);
    ASSERT_EQ(root.init(), OK);
    ASSERT_EQ(ncurses.root(), root.handle());
    ASSERT_EQ(root.end(), OK);
    ASSERT_EQ(ncurses.root(), nullptr);
}

TEST(window, ncurses_null)
{
    tui::window<ext::ncurses> win;
    ASSERT_EQ(win.init(), ERROR);
    ASSERT_EQ(win.refresh(), ERR);
    ASSERT_EQ(win.end(), ERR);
}

TEST_F(window_test, works)
{
    using window_t = tui::window<ext::ncurses>;
    auto window = std::make_shared<window_t>(ncurses, root);
    ASSERT_EQ(window->init(), OK);
    ASSERT_EQ(window->refresh(), OK);
    ASSERT_EQ(window->end(), OK);
}

TEST_F(mock_window_test, subwin_fails)
{
    EXPECT_CALL(ncurses, subwin(_, _, _, _, _)).WillOnce(Return(nullptr));

    using window_t = tui::window<ext::ncurses>;
    auto window = std::make_shared<window_t>(ncurses, root);
    ASSERT_EQ(window->init(), ERROR_SUBWIN);
}

TEST_F(window_test, stub_raii)
{
    // When the stubbed ext::ncurses leaves scope, it deletes all
    // keys found in its internal mapping (which are WINDOW objects
    // created via init()).
    using window_t = tui::window<ext::ncurses>;
    auto window = std::make_shared<window_t>(ncurses, root);
    ASSERT_EQ(window->init(), OK);
    ASSERT_EQ(ncurses.windows().size(), 1);
}
