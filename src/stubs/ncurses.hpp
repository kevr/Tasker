#ifndef SRC_STUBS_NCURSES_HPP
#define SRC_STUBS_NCURSES_HPP

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
    WINDOW *m_root = nullptr;

public:
    virtual ~ncurses() = default;

    // Stub functions.
    virtual WINDOW *initscr() noexcept;
    virtual int endwin() noexcept;

public:
    // Test utilities.
    WINDOW *root() noexcept;
};

}; // namespace tasker::ext

#endif /* SRC_EXTERN_NCURSES_HPP */
