#ifndef SRC_STUBS_NCURSES_HPP
#define SRC_STUBS_NCURSES_HPP

#include <map>

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
    static WINDOW *m_root;
    static std::map<WINDOW *, bool> m_keypad;

public:
    virtual ~ncurses() = default;

    // Stub functions.
    virtual WINDOW *initscr() noexcept;
    virtual int keypad(WINDOW *, bool) noexcept;
    virtual int raw() noexcept;
    virtual int noecho() noexcept;
    virtual int endwin() noexcept;

public:
    // Test utilities.
    WINDOW *root() noexcept;
    bool keypad(WINDOW *) noexcept;
};

}; // namespace tasker::ext

#endif /* SRC_EXTERN_NCURSES_HPP */
