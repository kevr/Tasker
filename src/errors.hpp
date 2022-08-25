#ifndef SRC_ERRORS_HPP
#define SRC_ERRORS_HPP

#include "ncurses.hpp"

enum tasker_error : int {
    ERROR = ERR,
    SUCCESS = OK,
    ERROR_INITSCR = 1,
    ERROR_KEYPAD = 2,
    ERROR_RAW = 3,
    ERROR_ECHO = 4,
};

#endif /* SRC_ERRORS_HPP */
