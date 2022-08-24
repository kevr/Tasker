#ifndef SRC_MOCKS_NCURSES_HPP
#define SRC_MOCKS_NCURSES_HPP

#include "../stubs/ncurses.hpp"
#include <gmock/gmock.h>

namespace tasker::ext
{

class mock_ncurses : public ncurses
{
public:
    MOCK_METHOD(WINDOW *, initscr, (), (noexcept, override));
    MOCK_METHOD(int, keypad, (WINDOW *, bool), (noexcept, override));
    MOCK_METHOD(int, raw, (), (noexcept, override));
    MOCK_METHOD(int, noecho, (), (noexcept, override));
    MOCK_METHOD(int, endwin, (), (noexcept, override));
};

}; // namespace tasker::ext

#endif /* SRC_MOCKS_NCURSES_HPP */
