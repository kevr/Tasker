/**
 * Description: Main program entry-point.
 *
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "config.hpp"
#include "ncurses.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << PROG << " " << VERSION << std::endl;

    auto ncurses = tasker::ext::ncurses();
    ncurses.initscr();
    return ncurses.endwin();
}
