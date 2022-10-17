#include "tui/root_window.hpp"
#include "mocks/ncurses.hpp"
#include <gtest/gtest.h>
using namespace tasker;

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::Test;

class root_window_test : public Test
{
protected:
    using CI = ext::ncurses;

    using root_window_t = tui::root_window<CI>;
    using root_window_ptr = std::shared_ptr<root_window_t>;

protected:
    ext::mock_ncurses ncurses;

    WINDOW fake_win;
    root_window_ptr root;

public:
    void SetUp()
    {
        root = std::make_shared<root_window_t>(ncurses);

        EXPECT_CALL(ncurses, initscr()).WillRepeatedly(Return(&fake_win));
        EXPECT_CALL(ncurses, get_max_yx(_, _, _))
            .WillOnce(Invoke([](WINDOW *, int &y, int &x) {
                x = 800;
                y = 600;
            }));
    }
};

TEST_F(root_window_test, init)
{
    ASSERT_EQ(root->init(), OK);
}

TEST_F(root_window_test, end)
{
    ASSERT_EQ(root->init(), OK);

    EXPECT_CALL(ncurses, endwin()).WillOnce(Return(OK));
    ASSERT_EQ(root->end(), OK);
}

TEST_F(root_window_test, end_error)
{
    ASSERT_EQ(root->init(), OK);

    EXPECT_CALL(ncurses, endwin()).WillOnce(Return(ERR));
    ASSERT_EQ(root->end(), ERR);
}
