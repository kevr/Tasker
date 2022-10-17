#ifndef SRC_STUBS_NCURSES_HPP
#define SRC_STUBS_NCURSES_HPP

#include <map>
#include <memory>

// We use a stub WINDOW in our stub definition, since
// we won't get the real symbols elsewhere.
class WINDOW
{
};

#define ERR -1
#define OK 0

typedef unsigned chtype;

#define ACS_VLINE '|'
#define ACS_HLINE '_'
#define ACS_ULCORNER '*'
#define ACS_URCORNER '*'
#define ACS_LLCORNER '*'
#define ACS_LRCORNER '*'

// KEY_* values used in ncurses.h on a Linux x86_64 system
// Stubbed here for testing purposes
#define KEY_RESIZE 410
#define KEY_DOWN 258
#define KEY_UP 259
#define KEY_LEFT 260
#define KEY_RIGHT 261

namespace tasker::ext
{

//! Stubbed version of ext/ncurses.hpp's class.
class ncurses
{
private:
    // Used to track a stub root window pointer of valid address.
    WINDOW *m_root;
    std::map<WINDOW *, bool> m_keypad;

    //! child -> parent map
    std::map<WINDOW *, WINDOW *> m_windows;

    //! color pairs
    std::map<short, std::pair<short, short>> m_pairs;

public:
    virtual ~ncurses();

    // Stub functions.
    virtual WINDOW *initscr() noexcept;
    virtual int keypad(WINDOW *, bool) noexcept;
    virtual int raw() noexcept;
    virtual int noecho() noexcept;
    virtual int getchar() noexcept;
    virtual int wrefresh(WINDOW *) noexcept;
    virtual int refresh() noexcept;
    virtual int endwin() noexcept;

    // Child window functions
    virtual WINDOW *derwin(WINDOW *, int, int, int, int) noexcept;
    virtual void get_max_yx(WINDOW *, int &, int &) noexcept;
    virtual int delwin(WINDOW *) noexcept;

    // Window utility functions
    virtual int w_add_str(WINDOW *, const char *) noexcept;
    virtual int wborder(WINDOW *, chtype, chtype, chtype, chtype, chtype,
                        chtype, chtype, chtype) noexcept;
    virtual int werase(WINDOW *) noexcept;
    virtual int wmove(WINDOW *, int, int) noexcept;
    virtual int curs_set(int) noexcept;

    // Colors
    virtual int start_color() noexcept;

    virtual int init_pair(short, short, short) noexcept;
    virtual int get_pair(short) noexcept;

    virtual int supported_colors() noexcept;
    virtual bool has_colors() noexcept;
    virtual int use_default_colors() noexcept;

    virtual int wattr_enable(WINDOW *, int) noexcept;
    virtual int wattr_disable(WINDOW *, int) noexcept;

    virtual int wbkgd(WINDOW *, chtype) noexcept;

public:
    // Test utilities.
    const WINDOW *root() const noexcept;
    const std::map<WINDOW *, WINDOW *> &windows() const;
    bool keypad(WINDOW *) const noexcept;
};

}; // namespace tasker::ext

#define COLOR_PAIR(x) this->ncurses->get_pair(x)

#define COLOR_BLACK 0
#define COLOR_RED 1
#define COLOR_GREEN 2
#define COLOR_YELLOW 3
#define COLOR_BLUE 4
#define COLOR_MAGENTA 5
#define COLOR_CYAN 6
#define COLOR_WHITE 7

#endif /* SRC_EXTERN_NCURSES_HPP */
