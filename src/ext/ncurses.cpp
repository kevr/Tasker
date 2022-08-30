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

int ext::ncurses::getchar() noexcept
{
    return getch();
}

int ext::ncurses::refresh() noexcept
{
    return ::refresh();
}

int ext::ncurses::wrefresh(WINDOW *win) noexcept
{
    return ::wrefresh(win);
}

int ext::ncurses::endwin() noexcept
{
    return ::endwin();
}

WINDOW *ext::ncurses::subwin(WINDOW *parent, int nlines, int ncols,
                             int begin_y, int begin_x) noexcept
{
    return ::subwin(parent, nlines, ncols, begin_y, begin_x);
}

void ext::ncurses::get_max_yx(WINDOW *win, int &y, int &x) noexcept
{
    getmaxyx(win, y, x);
}

int ext::ncurses::delwin(WINDOW *win) noexcept
{
    return ::delwin(win);
}

int ext::ncurses::w_add_str(WINDOW *win, const char *str) noexcept
{
    return waddstr(win, str);
}

WINDOW *ext::ncurses::root() noexcept
{
    return m_root;
}
// LCOV_EXCL_STOP
