#include "tui/board.hpp"
#include "mocks/ncurses.hpp"
#include "testing.hpp"
#include <gtest/gtest.h>
using namespace tasker;

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::Test;

class board_test : public Test
{
protected: // Convenient type aliases
    using CI = ext::ncurses;

    using board_t = tui::board<CI>;
    using board_ptr = std::shared_ptr<board_t>;

    using root_window_t = tui::root_window<CI>;
    using root_window_ptr = std::shared_ptr<root_window_t>;

    using task_list_t = tui::task_list<ext::ncurses>;

    char new_key;
    char quit_key;

protected: // Member variables
    ext::mock_ncurses ncurses;

    WINDOW fake_win;
    root_window_ptr root;
    board_ptr board;

public: // Member functions
    void SetUp()
    {
        MAKE_ARGS();
        auto &conf = cfg::config::ref();
        conf.parse_args(argc, argv);
        new_key = conf.get<char>("keybindings.project.new_list");
        quit_key = conf.get<char>("keybindings.quit");

        root = std::make_shared<root_window_t>(ncurses);
        board = std::make_shared<board_t>(ncurses, root);

        EXPECT_CALL(ncurses, initscr()).WillOnce(Return(&fake_win));
        EXPECT_CALL(ncurses, get_max_yx(_, _, _))
            .WillOnce(Invoke([](WINDOW *, int &y, int &x) {
                x = 800;
                y = 600;
            }));
        EXPECT_CALL(ncurses, endwin()).WillOnce(Return(OK));

        EXPECT_CALL(ncurses, delwin(_)).WillRepeatedly(Return(OK));
    }

    void TearDown()
    {
        root->end();
    }
};

TEST_F(board_test, works)
{
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .Times(1)
        .WillRepeatedly(Return(&fake_win));
    ASSERT_EQ(root->init(), OK);
    ASSERT_EQ(board->init(), OK);
}

TEST_F(board_test, adds_list_via_keybind)
{
    // Mock out ncurses window operations
    EXPECT_CALL(ncurses, w_add_str(_, _)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, wrefresh(_)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .Times(4)
        .WillRepeatedly(Return(&fake_win));

    // Initialize root and the board
    ASSERT_EQ(root->init(), OK);
    ASSERT_EQ(board->init(), OK);

    // Global keybindings must be bound before we start processing input
    root->context.bind_keys(cfg::config::ref());

    // Mock getchar() to spawn a new list, then quit
    EXPECT_CALL(ncurses, getchar())
        .WillOnce(Return(new_key))
        .WillOnce(Return(quit_key));

    // Run the input loop
    tasker::input_loop(ncurses, root->context);
}

TEST_F(board_test, navigate_board_via_keybinds)
{
    // Mock out ncurses window operations
    EXPECT_CALL(ncurses, w_add_str(_, _)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, wrefresh(_)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .Times(7)
        .WillRepeatedly(Return(&fake_win));

    // Initialize root and the board
    ASSERT_EQ(root->init(), OK);
    ASSERT_EQ(board->init(), OK);

    // Global keybindings must be bound before we start processing input
    root->context.bind_keys(cfg::config::ref());

    // Mock getchar() to:
    // 1. Spawn a list
    // 2. Try to navigate left
    // 3. Try to navigate right
    // 4. Spawn another list
    // 5. Navigate right
    // 6. Navigate left
    // 7. Quit
    EXPECT_CALL(ncurses, getchar())
        .WillOnce(Return(new_key))
        .WillOnce(Return(KEY_LEFT))
        .WillOnce(Return(KEY_RIGHT))
        .WillOnce(Return(new_key))
        .WillOnce(Return(KEY_RIGHT))
        .WillOnce(Return(KEY_LEFT))
        .WillOnce(Return(quit_key));

    // Run the input loop
    tasker::input_loop(ncurses, root->context);
}

TEST_F(board_test, new_list_error)
{
    testing::internal::CaptureStdout();

    ASSERT_EQ(root->init(), OK);

    EXPECT_CALL(ncurses, w_add_str(_, _)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, wrefresh(_)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .Times(1)
        .WillOnce(Return(&fake_win));
    ASSERT_EQ(board->init(), OK);

    // Global keybindings must be bound before we start processing input
    root->context.bind_keys(cfg::config::ref());

    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .Times(1)
        .WillOnce(Return(nullptr));

    // Mock getchar() to spawn a new list, then quit
    EXPECT_CALL(ncurses, getchar())
        .WillOnce(Return(new_key))
        .WillOnce(Return(quit_key));

    // Run the input loop
    tasker::input_loop(ncurses, root->context);

    auto output = testing::internal::GetCapturedStdout();
    auto pos = output.find("task_list::init() failed");
    ASSERT_NE(pos, std::string::npos);
}

TEST_F(board_test, initialize_lists_within_board)
{
    // Mock out ncurses window operations
    EXPECT_CALL(ncurses, w_add_str(_, _)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, wrefresh(_)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .WillRepeatedly(Return(&fake_win));

    auto list = std::make_shared<task_list_t>(ncurses, board);

    // TODO: Fix this std::logic_error
    // This happens twice; here, and through board->init, which causes
    // an issue because we're initializing the list twice.
    board->add_list(list);

    // Initialize root and the board
    ASSERT_EQ(root->init(), OK);
    ASSERT_EQ(board->init(), OK);
}

TEST_F(board_test, add_list_init_error)
{
    // Test that when list->init() fails during board::add_list,
    // ERROR_DERWIN is returned to the caller.

    // Mock out ncurses window operations
    EXPECT_CALL(ncurses, w_add_str(_, _)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, wrefresh(_)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .Times(1)
        .WillRepeatedly(Return(nullptr));

    ASSERT_EQ(root->init(), OK);

    auto list = std::make_shared<task_list_t>(ncurses, board);
    ASSERT_EQ(board->add_list(list), ERROR_DERWIN);
}

TEST_F(board_test, list_draw_error)
{
    ASSERT_EQ(root->init(), OK);

    EXPECT_CALL(ncurses, w_add_str(_, _)).WillOnce(Return(OK));
    EXPECT_CALL(ncurses, wrefresh(_)).WillRepeatedly(Return(OK));
    EXPECT_CALL(ncurses, derwin(_, _, _, _, _))
        .WillRepeatedly(Return(&fake_win));
    ASSERT_EQ(board->init(), OK);
    ASSERT_EQ(board->draw(), OK);

    auto list = std::make_shared<task_list_t>(ncurses, board);
    board->add_list(list);
    EXPECT_CALL(ncurses, w_add_str(_, _)).WillOnce(Return(ERR));
    ASSERT_EQ(board->draw(), ERROR_WADDSTR);
}
