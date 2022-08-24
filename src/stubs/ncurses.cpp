/**
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "ncurses.hpp"
using namespace tasker;

// Static member initialization
WINDOW *ext::ncurses::m_root = nullptr;
std::map<WINDOW *, bool> ext::ncurses::m_keypad;

// Local WINDOW object used for stubbed addressing.
static WINDOW window;

WINDOW *ext::ncurses::initscr() noexcept
{
    m_root = &window;
    return root();
}

int ext::ncurses::keypad(WINDOW *win, bool bf) noexcept
{
    m_keypad[win] = bf;
    return OK;
}

int ext::ncurses::raw() noexcept
{
    return OK;
}

int ext::ncurses::endwin() noexcept
{
    return OK;
}

WINDOW *ext::ncurses::root() noexcept
{
    return m_root;
}

bool ext::ncurses::keypad(WINDOW *win) noexcept
{
    auto it = m_keypad.find(win);
    if (it == m_keypad.end())
        return false;
    return it->second;
}
