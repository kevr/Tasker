#include "context.hpp"
#include "mocks/ncurses.hpp"
#include <gtest/gtest.h>
using namespace tasker;

using testing::Return;

TEST(context, safe_input_loop_exception)
{
    ext::mock_ncurses ncurses;

    context<int> ctx;
    ctx.keybinds['q'] = []() -> int {
        throw std::domain_error("test");
        return OK;
    };

    EXPECT_CALL(ncurses, getchar()).WillOnce(Return('q'));
    safe_input_loop(ncurses, ctx);
}
