/**
 * Description: Main program entry-point.
 *
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "config.hpp"
#include "ncurses.hpp"
#include "tui.hpp"
#include "utility.hpp"
#include <chrono>
#include <iostream>
#include <thread>
using namespace tasker;

int tasker_main(ext::ncurses &ncurses, int argc, char *argv[])
{
    // Print out the program name and version.
    std::cout << PROG << " " << VERSION << std::endl;

    // Construct and initialize the TUI
    tui::tui term(ncurses);
    if (!term.init())
        return term.end();

    // Refresh the TUI
    term.refresh();

    // End the TUI
    return term.end();
}

int main(int argc, char *argv[])
{
    auto ncurses = tasker::ext::ncurses();
    return tasker_main(ncurses, argc, argv);
}
