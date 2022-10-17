#include "mocks/ncurses.hpp"
#include "testing.hpp"
#include "tui/project.hpp"
#include "tui/root_window.hpp"
#include <gtest/gtest.h>
using namespace tasker;

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::Test;

class project_test : public Test
{
private:
    // Some convenient type aliases
    using root_window_t = tui::root_window<ext::ncurses>;
    using root_window_ptr = std::shared_ptr<root_window_t>;

    using project_t = tui::project<ext::ncurses>;
    using project_ptr = std::shared_ptr<project_t>;

protected:
    // Handle to ncurses interface
    ext::mock_ncurses ncurses;

    // A fake WINDOW object used to return valid initializations
    WINDOW fake_win;

    // Pointers to tui windows
    root_window_ptr root;
    project_ptr project;

public:
    virtual void SetUp() override
    {
        // Config has to be parsed in order for tui::tui to be able
        // to reach certain config variables during initialization
        MAKE_ARGS();
        cfg::config::ref().parse_args(argc, argv);

        // Mock up some calls that always occur
        EXPECT_CALL(ncurses, initscr()).WillOnce(Return(&fake_win));
        EXPECT_CALL(ncurses, get_max_yx(_, _, _))
            .WillOnce(Invoke([](WINDOW *win, int &y, int &x) {
                x = 800;
                y = 600;
            }));

        // Construct and initialize our root
        root = std::make_shared<root_window_t>(ncurses);
        root->init();

        // Construct and initialize our project based off of root
        project = std::make_shared<project_t>(ncurses, root);
    }

    void mock_project()
    {
        // Mock all derwin() calls to succeed
        EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
            .WillRepeatedly(Return(&fake_win));
    }
};

TEST_F(project_test, init)
{
    mock_project();
    project->init();
}

TEST_F(project_test, bar_init_fails)
{
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .WillOnce(Return(&fake_win))
        .WillOnce(Return(nullptr));
    ASSERT_EQ(project->init(), ERROR_DERWIN);
}

TEST_F(project_test, content_init_fails)
{
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .WillOnce(Return(&fake_win))
        .WillOnce(Return(&fake_win))
        .WillOnce(Return(nullptr));
    ASSERT_EQ(project->init(), ERROR_DERWIN);
}

TEST_F(project_test, board_init_fails)
{
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .WillOnce(Return(&fake_win))
        .WillOnce(Return(&fake_win))
        .WillOnce(Return(&fake_win))
        .WillOnce(Return(nullptr));
    ASSERT_EQ(project->init(), ERROR_DERWIN);
}
