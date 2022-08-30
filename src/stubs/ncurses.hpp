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
    virtual WINDOW *subwin(WINDOW *, int, int, int, int) noexcept;
    virtual void get_max_yx(WINDOW *, int &, int &) noexcept;
    virtual int delwin(WINDOW *) noexcept;

public:
    // Test utilities.
    const WINDOW *root() const noexcept;
    const std::map<WINDOW *, WINDOW *> &windows() const;
    bool keypad(WINDOW *) const noexcept;
};

}; // namespace tasker::ext

#endif /* SRC_EXTERN_NCURSES_HPP */
