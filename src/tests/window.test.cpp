#include "../tui/window.hpp"
#include "../mocks/ncurses.hpp"
#include <gtest/gtest.h>
using namespace tasker;

using ::testing::_;
using ::testing::Invoke;
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
    EXPECT_CALL(ncurses, subwin(_, _, _, _, _)).WillOnce(Return(nullptr));

    using window_t = tui::window<ext::ncurses>;
    auto window = std::make_shared<window_t>(ncurses, root);
    ASSERT_EQ(window->init(), ERROR_SUBWIN);
}

TEST_F(mock_window_test, root_refresh_all)
{
    WINDOW win;
    EXPECT_CALL(ncurses, initscr()).WillOnce(Return(&win));
    EXPECT_CALL(ncurses, get_max_yx(_, _, _))
        .WillOnce(Invoke([](WINDOW *, int &y, int &x) {
            x = 800;
            y = 600;
        }));
    EXPECT_CALL(ncurses, endwin()).WillOnce(Return(OK));

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

    EXPECT_CALL(ncurses, delwin(_)).WillOnce(Return(OK));
    window->end();
}

TEST_F(mock_window_test, refresh_all)
{
    WINDOW win;
    EXPECT_CALL(ncurses, initscr()).WillOnce(Return(&win));
    EXPECT_CALL(ncurses, get_max_yx(_, _, _))
        .WillOnce(Invoke([](WINDOW *, int &y, int &x) {
            x = 800;
            y = 600;
        }));
    EXPECT_CALL(ncurses, endwin()).WillOnce(Return(OK));

    ASSERT_EQ(root->init(), OK);

    using window_t = tui::window<ext::ncurses>;
    auto window = std::make_shared<window_t>(ncurses, root);

    WINDOW child;
    EXPECT_CALL(ncurses, subwin(_, _, _, _, _)).WillOnce(Return(&child));
    window->init();

    auto window2 = std::make_shared<window_t>(ncurses, window);

    WINDOW child2;
    EXPECT_CALL(ncurses, subwin(_, _, _, _, _)).WillOnce(Return(&child2));
    window2->init();

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
    window2->end();
    window->end();
}
