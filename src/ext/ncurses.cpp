/**
 * Real ncurses library calls.
 *
 * The code defined in this file is completely excluded from coverage.
 * During tests, we use stubs found in <project_root>/src/stubs. Those
 * should see and require 100% coverage.
 *
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "ncurses.hpp"
#include <ncurses.h>
using namespace tasker;

// LCOV_EXCL_START
WINDOW *ext::ncurses::initscr() noexcept
{
    return ::initscr();
}

int ext::ncurses::endwin() noexcept
{
    return ::endwin();
}
// LCOV_EXCL_STOP
