#include "tui/basic_window.hpp"
#include "config/config.hpp"
#include "mocks/ncurses.hpp"
#include "testing.hpp"
#include "tui/root_window.hpp"
#include "tui/window.hpp"
#include <gtest/gtest.h>
using namespace tasker;

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::Test;

/**
 * This test suite tests tui::basic_window paths by using
 * direct derivatives, tui::root_window and tui::window.
 *
 * This forces us to write root_window and window code in
 * such a way that we do access basic_window paths through
 * them, which proves that the functionality in basic_window
 * does directly connect to the derivative.
 **/
class basic_window_test : public Test
{
protected:
    using CI = ext::ncurses;

    using root_window_t = tui::root_window<CI>;
    using root_window_ptr = std::shared_ptr<root_window_t>;

    using window_t = tui::window<CI>;
    using window_ptr = std::shared_ptr<window_t>;

protected:
    ext::mock_ncurses ncurses;

    WINDOW fake_win;
    root_window_ptr root;
    window_ptr child;

public:
    static void SetUpTestSuite()
    {
        MAKE_ARGS();
        cfg::config::ref().parse_args(argc, argv);
    }

    void SetUp()
    {
        root = std::make_shared<root_window_t>(ncurses);
        child = std::make_shared<window_t>(ncurses, root);

        EXPECT_CALL(ncurses, get_max_yx(_, _, _))
            .WillRepeatedly(Invoke([](WINDOW *, int &y, int &x) {
                x = 800;
                y = 600;
            }));
    }
};

TEST_F(basic_window_test, end_child_error)
{
    EXPECT_CALL(ncurses, initscr()).WillRepeatedly(Return(&fake_win));
    ASSERT_EQ(root->init(), OK);

    EXPECT_CALL(ncurses, derwin(_, _, _, _, _)).WillOnce(Return(&fake_win));
    ASSERT_EQ(child->init(), OK);

    EXPECT_CALL(ncurses, delwin(_)).WillOnce(Return(ERR));
    // TODO: This should return something like ERROR_DELWIN?
    ASSERT_EQ(root->end(), ERR);
}
