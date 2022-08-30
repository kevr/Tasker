#include "tui/pane.hpp"
#include "mocks/ncurses.hpp"
#include "tui/root_window.hpp"
#include <gtest/gtest.h>
using namespace tasker;

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

class pane_test : public ::testing::Test
{
protected:
    ext::mock_ncurses ncurses;

    using root_window_t = tui::root_window<ext::ncurses>;
    std::shared_ptr<root_window_t> root;

    using window_t = tui::window<ext::ncurses>;

    using pane_t = tui::pane<ext::ncurses>;
    std::shared_ptr<pane_t> m_pane;

    // Metadata
    WINDOW win_root;
    WINDOW win_pane;

public:
    virtual void SetUp() override
    {
        EXPECT_CALL(ncurses, initscr()).WillOnce(Return(&win_root));
        EXPECT_CALL(ncurses, endwin()).WillOnce(Return(OK));

        EXPECT_CALL(ncurses, get_max_yx(_, _, _))
            .WillRepeatedly(Invoke([](WINDOW *, int &y, int &x) {
                x = 800;
                y = 600;
            }));
        EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
            .WillOnce(Return(&win_pane));
        EXPECT_CALL(ncurses, delwin(_)).WillRepeatedly(Return(OK));

        root = std::make_shared<root_window_t>(ncurses);
        ASSERT_EQ(root->init(), OK);

        m_pane = std::make_shared<pane_t>(ncurses, root);
        ASSERT_EQ(m_pane->init(), OK);
    }

    virtual void TearDown() override
    {
        m_pane->end();
        root->end();
    }
};

TEST_F(pane_test, refresh_error)
{
    EXPECT_CALL(ncurses, wrefresh(_)).WillOnce(Return(ERR));
    ASSERT_EQ(m_pane->refresh(), ERR);
}

TEST_F(pane_test, focus_on_empty_children)
{
    ASSERT_THROW(m_pane->focus(0), std::out_of_range);
}

TEST_F(pane_test, focus_out_of_range)
{
    WINDOW win_pane_child;
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .WillOnce(Return(&win_pane_child));

    auto window = std::make_shared<window_t>(ncurses, m_pane);
    ASSERT_EQ(window->init(), OK);

    // We only have one child; focusing on i=1 should overflow.
    ASSERT_THROW(m_pane->focus(1), std::out_of_range);
}

TEST_F(pane_test, focus_refresh_error)
{
    WINDOW win_pane_child;
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .WillOnce(Return(&win_pane_child));

    auto window = std::make_shared<window_t>(ncurses, m_pane);
    ASSERT_EQ(window->init(), OK);

    m_pane->focus(0);
    EXPECT_CALL(ncurses, wrefresh(_))
        .Times(2)
        .WillOnce(Return(OK))
        .WillOnce(Return(ERR));
    ASSERT_EQ(m_pane->refresh(), ERR);
}
