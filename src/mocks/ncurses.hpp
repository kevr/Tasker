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
    MOCK_METHOD(int, getchar, (), (noexcept, override));
    MOCK_METHOD(int, wrefresh, (WINDOW *), (noexcept, override));
    MOCK_METHOD(int, refresh, (), (noexcept, override));
    MOCK_METHOD(int, endwin, (), (noexcept, override));
    MOCK_METHOD(void, get_max_yx, (WINDOW *, int &, int &),
                (noexcept, override));
    MOCK_METHOD(WINDOW *, derwin, (WINDOW *, int, int, int, int),
                (noexcept, override));
    MOCK_METHOD(int, delwin, (WINDOW *), (noexcept, override));
    MOCK_METHOD(int, w_add_str, (WINDOW *, const char *),
                (noexcept, override));
    MOCK_METHOD(int, wborder,
                (WINDOW *, chtype, chtype, chtype, chtype, chtype, chtype,
                 chtype, chtype),
                (noexcept, override));
};

}; // namespace tasker::ext

#endif /* SRC_MOCKS_NCURSES_HPP */
