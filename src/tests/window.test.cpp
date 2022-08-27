#include "../tui/window.hpp"
#include "../mocks/ncurses.hpp"
#include <gtest/gtest.h>
using namespace tasker;

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// Test fixture using stubbed ncurses.
class window_test : public ::testing::Test
{
protected:
    ext::ncurses ncurses;

    using root_window_t = tui::root_window<ext::ncurses>;
    std::shared_ptr<root_window_t> root =
        std::make_shared<root_window_t>(ncurses);
};

//! Text fixture using mocked ncurses.
class mock_window_test : public ::testing::Test
{
protected:
    ext::mock_ncurses ncurses;

    using root_window_t = tui::root_window<ext::ncurses>;
    std::shared_ptr<root_window_t> root =
        std::make_shared<root_window_t>(ncurses);
};

//! Set some default root window initialization mock expectations
template <typename CI>
void expect_root_init(CI &ncurses, WINDOW *windowPointer)
{
    EXPECT_CALL(ncurses, initscr()).WillOnce(Return(windowPointer));
    EXPECT_CALL(ncurses, get_max_yx(_, _, _))
        .WillOnce(Invoke([](WINDOW *, int &y, int &x) {
            x = 800;
            y = 600;
        }));
    EXPECT_CALL(ncurses, endwin()).WillRepeatedly(Return(OK));
}

//! Assert some basic window actions: init(), refresh() and end().
template <typename W>
void assert_window_actions(W &window, int init_rc, int refresh_rc, int end_rc)
{
    ASSERT_EQ(window.init(), init_rc);
    ASSERT_EQ(window.refresh(), refresh_rc);
    ASSERT_EQ(window.end(), end_rc);
}

TEST(root_window, ncurses_null)
{
    // When root is constructed with nothing, everything errors out.
    tui::root_window<ext::ncurses> root;
    assert_window_actions(root, ERR, ERR, ERR);
}

TEST(root_window, stub_nulls_root)
{
    // Test that ncurses.root() is nulled whenever a root_window<CI> ends.
    ext::ncurses ncurses;
    tui::root_window<ext::ncurses> root(ncurses);
    ASSERT_EQ(root.init(), OK);
    ASSERT_EQ(ncurses.root(), root.handle());
    ASSERT_EQ(root.end(), OK);
    ASSERT_EQ(ncurses.root(), nullptr);
}

TEST(window, ncurses_null)
{
    // When window is constructed with nothing, everything errors out.
    tui::window<ext::ncurses> window;
    assert_window_actions(window, ERR, ERR, ERR);
}

TEST_F(window_test, works)
{
    // When it is constructed
    using window_t = tui::window<ext::ncurses>;
    auto window = std::make_shared<window_t>(ncurses, root);
    assert_window_actions(*window, OK, OK, OK);
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

TEST_F(mock_window_test, subwin_fails)
{
    using window_t = tui::window<ext::ncurses>;
    auto window = std::make_shared<window_t>(ncurses, root);

    EXPECT_CALL(ncurses, subwin(_, _, _, _, _)).WillOnce(Return(nullptr));
    ASSERT_EQ(window->init(), ERROR_SUBWIN);
}

TEST_F(mock_window_test, root_refresh_all)
{
    // Test that the root refreshes itself and all of its direct children.
    WINDOW win;
    expect_root_init(ncurses, &win);

    ASSERT_EQ(root->init(), OK);

    using window_t = tui::window<ext::ncurses>;
    auto window = std::make_shared<window_t>(ncurses, root);

    WINDOW child;
    EXPECT_CALL(ncurses, subwin(_, _, _, _, _)).WillOnce(Return(&child));
    window->init();

    EXPECT_CALL(ncurses, wrefresh(_)).WillOnce(Return(OK));
    EXPECT_CALL(ncurses, refresh()).WillOnce(Return(OK));
    root->refresh_all();

    // Test failure path #1, own window fails to refresh.
    EXPECT_CALL(ncurses, refresh()).Times(1).WillOnce(Return(ERR));
    ASSERT_EQ(root->refresh_all(), ERR);

    // Test failure path #2, child window fails to refresh.
    EXPECT_CALL(ncurses, refresh()).Times(1).WillOnce(Return(OK));
    EXPECT_CALL(ncurses, wrefresh(_)).Times(1).WillOnce(Return(ERR));
    ASSERT_EQ(root->refresh_all(), ERR);

    // End the child windows.
    EXPECT_CALL(ncurses, delwin(_)).WillRepeatedly(Return(OK));
}

TEST_F(mock_window_test, refresh_all)
{
    // Test that a non-root window with children refresh all of
    // those children via basic_window<CI>::refresh().
    WINDOW win;
    expect_root_init(ncurses, &win);
    ASSERT_EQ(root->init(), OK);

    using window_t = tui::window<ext::ncurses>;
    auto window = std::make_shared<window_t>(ncurses, root);

    WINDOW child;
    EXPECT_CALL(ncurses, subwin(_, _, _, _, _)).WillOnce(Return(&child));
    window->init();

    auto leaf = std::make_shared<window_t>(ncurses, window);

    WINDOW child2;
    EXPECT_CALL(ncurses, subwin(_, _, _, _, _)).WillOnce(Return(&child2));
    leaf->init();

    EXPECT_CALL(ncurses, wrefresh(_)).Times(2).WillRepeatedly(Return(OK));
    window->refresh_all();

    // Test failure path #1, own window fails to refresh.
    EXPECT_CALL(ncurses, wrefresh(_)).Times(1).WillOnce(Return(ERR));
    ASSERT_EQ(window->refresh_all(), ERR);

    // Test failure path #2, child window fails to refresh.
    EXPECT_CALL(ncurses, wrefresh(_))
        .Times(2)
        .WillOnce(Return(OK))
        .WillOnce(Return(ERR));
    ASSERT_EQ(window->refresh_all(), ERR);

    // End the child windows.
    EXPECT_CALL(ncurses, delwin(_)).WillRepeatedly(Return(OK));
}
