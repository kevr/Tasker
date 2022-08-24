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
    return (m_root = ::initscr());
}

int ext::ncurses::keypad(WINDOW *win, bool bf) noexcept
{
    return ::keypad(win, bf);
}

int ext::ncurses::raw() noexcept
{
    return ::raw();
}

int ext::ncurses::noecho() noexcept
{
    return ::noecho();
}

int ext::ncurses::refresh() noexcept
{
    return ::refresh();
}

int ext::ncurses::endwin() noexcept
{
    return ::endwin();
}

WINDOW *ext::ncurses::root() noexcept
{
    return m_root;
}
// LCOV_EXCL_STOP
