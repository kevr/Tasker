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
    WINDOW *derwin(WINDOW *, int, int, int, int) noexcept;
    void get_max_yx(WINDOW *, int &, int &) noexcept;
    int delwin(WINDOW *) noexcept;

    // Window utility functions
    int w_add_str(WINDOW *, const char *) noexcept;
    int wborder(WINDOW *, chtype, chtype, chtype, chtype, chtype, chtype,
                chtype, chtype) noexcept;
    int werase(WINDOW *) noexcept;
    int wmove(WINDOW *, int, int) noexcept;
    int curs_set(int) noexcept;

    // Color functions
    int start_color() noexcept;
    int init_pair(short, short, short) noexcept;
    int supported_colors() noexcept;
    bool has_colors() noexcept;
    int use_default_colors() noexcept;

    int wattr_enable(WINDOW *, int) noexcept;
    int wattr_disable(WINDOW *, int) noexcept;

    int wbkgd(WINDOW *, chtype) noexcept;

public:
    WINDOW *root() noexcept;
};

}; // namespace tasker::ext

#endif /* SRC_EXTERN_NCURSES_HPP */
