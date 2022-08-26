#ifndef SRC_EXTERN_NCURSES_HPP
#define SRC_EXTERN_NCURSES_HPP

#include <ncurses.h>

namespace tasker::ext
{

//! Real ncurses library function call utility
class ncurses
{
private:
    WINDOW *m_root = nullptr;

public:
    WINDOW *initscr() noexcept;
    int keypad(WINDOW *, bool) noexcept;
    int raw() noexcept;
    int noecho() noexcept;
    int refresh() noexcept;
    int endwin() noexcept;

public:
    WINDOW *root() noexcept;
};

}; // namespace tasker::ext

#endif /* SRC_EXTERN_NCURSES_HPP */
