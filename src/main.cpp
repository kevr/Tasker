/**
 * Description: Main program entry-point.
 *
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "config.hpp"
#include "config/config.hpp"
#include "env.hpp"
#include "ncurses.hpp"
#include "tui.hpp"
#include "utility.hpp"
#include <chrono>
#include <iostream>
#include <thread>
using namespace tasker;

int tasker_main(ext::ncurses &ncurses, int argc, char *argv[])
{
    // Parse command line arguments and handle them.
    auto &conf = cfg::config::ref();
    conf.parse_args(argc, argv);

    if (conf.exists("help")) {
        std::cout << "usage: " << conf.usage() << std::endl
                  << conf << std::endl;
        return SUCCESS;
    }

    if (conf.exists("version")) {
        std::cout << VERSION << std::endl;
        return SUCCESS;
    }

    std::optional<std::filesystem::path> path;

    if (conf.exists("config")) {
        path = conf["config"];
    } else {
        path = env::search_config_path();
    }

    if (path.has_value()) {
        try {
            conf.parse_config(path.value());
        } catch (boost::program_options::unknown_option &e) {
            std::cerr << "error: " << e.what() << " found in "
                      << path.value().string() << std::endl;
            return ERR;
        }
    }

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
