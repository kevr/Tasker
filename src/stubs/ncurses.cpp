/**
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "ncurses.hpp"
#include "defaults.hpp"
using namespace tasker;

// Static member initialization
// WINDOW *ext::ncurses::m_root = nullptr;
// std::map<WINDOW *, bool> ext::ncurses::m_keypad;

// Local WINDOW object used for stubbed addressing.
static WINDOW window;

ext::ncurses::~ncurses()
{
    for (auto &kv : m_windows) {
        delete kv.first;
    }
}

WINDOW *ext::ncurses::initscr() noexcept
{
    m_root = &window;
    return m_root;
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

int ext::ncurses::noecho() noexcept
{
    return OK;
}

int ext::ncurses::getchar() noexcept
{
    return defaults::keybinds::KEY_QUIT;
}

int ext::ncurses::wrefresh(WINDOW *) noexcept
{
    return OK;
}

int ext::ncurses::refresh() noexcept
{
    return OK;
}

int ext::ncurses::endwin() noexcept
{
    m_root = nullptr;
    return OK;
}

// Child window functions
WINDOW *ext::ncurses::derwin(WINDOW *parent, int, int, int, int) noexcept
{
    WINDOW *win = new WINDOW;
    m_windows[win] = parent;
    return win;
}

void ext::ncurses::get_max_yx(WINDOW *, int &y, int &x) noexcept
{
    x = 800;
    y = 600;
}

int ext::ncurses::delwin(WINDOW *win) noexcept
{
    auto it = m_windows.find(win);
    if (it == m_windows.end())
        return ERR;

    m_windows.erase(it);
    delete win;

    return OK;
}

int ext::ncurses::w_add_str(WINDOW *, const char *) noexcept
{
    return OK;
}

int ext::ncurses::wborder(WINDOW *, chtype, chtype, chtype, chtype, chtype,
                          chtype, chtype, chtype) noexcept
{
    return OK;
}

int ext::ncurses::werase(WINDOW *win) noexcept
{
    return OK;
}

int ext::ncurses::wmove(WINDOW *, int, int) noexcept
{
    return OK;
}

int ext::ncurses::curs_set(int) noexcept
{
    return OK;
}

int ext::ncurses::start_color() noexcept
{
    return OK;
}

int ext::ncurses::init_pair(short pair, short fg, short bg) noexcept
{
    m_pairs[pair] = std::make_pair(fg, bg);
    return OK;
}

int ext::ncurses::get_pair(short pair) noexcept
{
    if (m_pairs.find(pair) == m_pairs.end())
        return ERR;
    return pair;
}

int ext::ncurses::supported_colors() noexcept
{
    return 256;
}

bool ext::ncurses::has_colors() noexcept
{
    return true;
}

int ext::ncurses::use_default_colors() noexcept
{
    return OK;
}

int ext::ncurses::wattr_enable(WINDOW *, int) noexcept
{
    return OK;
}

int ext::ncurses::wattr_disable(WINDOW *, int) noexcept
{
    return OK;
}

int ext::ncurses::wbkgd(WINDOW *win, chtype ch) noexcept
{
    return OK;
}

// Public utility functions
const WINDOW *ext::ncurses::root() const noexcept
{
    return m_root;
}

const std::map<WINDOW *, WINDOW *> &ext::ncurses::windows() const
{
    return m_windows;
}

bool ext::ncurses::keypad(WINDOW *win) const noexcept
{
    auto it = m_keypad.find(win);
    if (it == m_keypad.end())
        return false;
    return it->second;
}
