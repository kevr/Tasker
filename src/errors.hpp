#ifndef SRC_ERRORS_HPP
#define SRC_ERRORS_HPP

#include "ncurses.hpp"
#include <string>

enum tasker_error : int {
    // ERR is accessible via ncurses
    // OK is accessible via ncurses
    ERROR_INITSCR = 1,
    ERROR_KEYPAD = 2,
    ERROR_RAW = 3,
    ERROR_ECHO = 4,
    ERROR_DERWIN = 5,
    ERROR_GETMAXYX = 6,
    ERROR_WADDSTR = 7,
    ERROR_NOIMPL = 8,
    ERROR_ARGS = 9,
    ERROR_CONFIG = 10,
};

int raw_error(int rc, const std::string &message);

#endif /* SRC_ERRORS_HPP */
