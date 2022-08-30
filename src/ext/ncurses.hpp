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
    int getchar() noexcept;
    int refresh() noexcept;
    int wrefresh(WINDOW *) noexcept;
    int endwin() noexcept;

    // Child window functions
    WINDOW *subwin(WINDOW *, int, int, int, int) noexcept;
    void get_max_yx(WINDOW *, int &, int &) noexcept;
    int delwin(WINDOW *) noexcept;

    // Window utility functions
    int w_add_str(WINDOW *, const char *) noexcept;
    int wborder(WINDOW *, chtype, chtype, chtype, chtype, chtype, chtype,
                chtype, chtype) noexcept;

public:
    WINDOW *root() noexcept;
};

}; // namespace tasker::ext

#endif /* SRC_EXTERN_NCURSES_HPP */
