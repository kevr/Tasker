/**
 * Description: Main program entry-point.
 *
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "config.hpp"
#include "ncurses.hpp"
#include "utility.hpp"
#include <iostream>

int tasker_main(tasker::ext::ncurses &ncurses, int argc, char *argv[])
{
    std::cout << PROG << " " << VERSION << std::endl;
    if (!ncurses.initscr()) {
        return error(1, "ncurses.initscr() failed");
    }

    if (auto rc = ncurses.keypad(ncurses.root(), true)) {
        return error(2, "ncurses.keypad failed: ", rc);
    }

    return ncurses.endwin();
}

int main(int argc, char *argv[])
{
    auto ncurses = tasker::ext::ncurses();
    return tasker_main(ncurses, argc, argv);
}
