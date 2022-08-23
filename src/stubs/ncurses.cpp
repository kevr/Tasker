/**
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "ncurses.hpp"
using namespace tasker;

static WINDOW window;

WINDOW *ext::ncurses::initscr() noexcept
{
    m_root = &window;
    return root();
}

int ext::ncurses::endwin() noexcept
{
    return OK;
}

WINDOW *ext::ncurses::root() noexcept
{
    return m_root;
}
