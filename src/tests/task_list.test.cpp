#include "tui/task_list.hpp"
#include "mocks/ncurses.hpp"
#include "tui/root_window.hpp"
#include <gtest/gtest.h>
using namespace tasker;

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::Test;

class task_list_test : public Test
{
protected:
    using CI = ext::ncurses;

    using root_window_t = tui::root_window<CI>;
    using root_window_ptr = std::shared_ptr<root_window_t>;

    using task_list_t = tui::task_list<CI>;
    using task_list_ptr = std::shared_ptr<task_list_t>;

protected:
    ext::mock_ncurses ncurses;

    WINDOW fake_win;

    root_window_ptr root;
    task_list_ptr task_list;

public:
    void SetUp()
    {
        const char *_argv[] = { "tasker" };
        char **argv = const_cast<char **>(_argv);
        cfg::config::new_ref().parse_args(1, argv);

        root = std::make_shared<root_window_t>(ncurses);

        EXPECT_CALL(ncurses, initscr()).WillOnce(Return(&fake_win));
        EXPECT_CALL(ncurses, get_max_yx(_, _, _))
            .WillOnce(Invoke([](WINDOW *, int &y, int &x) {
                x = 800;
                y = 600;
            }));

        ASSERT_EQ(root->init(), OK);

        task_list = std::make_shared<task_list_t>(ncurses, root);
    }

    void mock_task_list()
    {
        EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
            .WillRepeatedly(Return(&fake_win));

        ASSERT_EQ(task_list->init(), OK);
    }
};

TEST_F(task_list_test, init)
{
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .WillRepeatedly(Return(&fake_win));

    ASSERT_EQ(task_list->init(), OK);
}

TEST_F(task_list_test, init_title_error)
{
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .WillOnce(Return(&fake_win))
        .WillOnce(Return(nullptr));

    ASSERT_EQ(task_list->init(), ERROR_DERWIN);
}

TEST_F(task_list_test, init_pane_error)
{
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .WillOnce(Return(&fake_win))
        .WillOnce(Return(&fake_win))
        .WillOnce(Return(nullptr));

    ASSERT_EQ(task_list->init(), ERROR_DERWIN);
}

TEST_F(task_list_test, draw)
{
    mock_task_list();

    EXPECT_CALL(ncurses, w_add_str(_, _)).WillRepeatedly(Return(OK));
    ASSERT_EQ(task_list->draw(), OK);
}

TEST_F(task_list_test, draw_title_error)
{
    mock_task_list();

    EXPECT_CALL(ncurses, w_add_str(_, _)).WillRepeatedly(Return(ERR));
    ASSERT_EQ(task_list->draw(), ERROR_WADDSTR);
}

TEST_F(task_list_test, name_exc)
{
    EXPECT_THROW(task_list->name(), std::logic_error);
}

TEST_F(task_list_test, set_name_exc)
{
    EXPECT_THROW(task_list->set_name("test"), std::logic_error);
}

TEST_F(task_list_test, set_name)
{
    mock_task_list();

    task_list->set_name("test");
    ASSERT_EQ(task_list->name(), "test");
}
