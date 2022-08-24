#ifndef SRC_EXTERN_NCURSES_HPP
#define SRC_EXTERN_NCURSES_HPP

// Forward ncurses declarations.
struct _win_st;
typedef struct _win_st WINDOW;

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
    int endwin() noexcept;

public:
    WINDOW *root() noexcept;
};

}; // namespace tasker::ext

#endif /* SRC_EXTERN_NCURSES_HPP */
