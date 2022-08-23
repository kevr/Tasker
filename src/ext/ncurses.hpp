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
public:
    WINDOW *initscr() noexcept;
    int endwin() noexcept;
};

}; // namespace tasker::ext

#endif /* SRC_EXTERN_NCURSES_HPP */
