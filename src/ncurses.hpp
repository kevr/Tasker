/**
 * @brief Main project ncurses header
 *
 * This header should be included in all application code using
 * ncurses. `meson` is configured to supply -DTESTING, which will
 * enable switching to "stubs/ncurses.hpp". Otherwise, by default
 * (without -DTESTING), "ext/ncurses.hpp" is included, which includes
 * real ncurses library calls.
 *
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#ifndef SRC_NCURSES_HPP
#define SRC_NCURSES_HPP

#ifndef TESTING
#include "ext/ncurses.hpp"
#else
#include "stubs/ncurses.hpp"
#endif

#endif /* SRC_NCURSES_HPP */
