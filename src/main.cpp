/**
 * Description: Main program entry-point.
 *
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "callback.hpp"
#include "config.hpp"
#include "config/config.hpp"
#include "config/keybinds.hpp"
#include "context.hpp"
#include "env.hpp"
#include "logging.hpp"
#include "ncurses.hpp"
#include "tui.hpp"
#include "utility.hpp"
#include <chrono>
#include <iostream>
#include <thread>
using namespace tasker;

static logger logging;
static std::ofstream ofs;

int tasker_main(ext::ncurses &ncurses, int argc, char *argv[])
{
    // Parse command line arguments and handle them.
    auto &conf = cfg::config::ref();
    namespace po = boost::program_options;
    conf.option("logfile,l",
                po::value<std::string>(),
                "designate a log file instead of stderr");

    cfg::add_keybind_options(conf);

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

    if (conf.exists("logfile")) {
        auto logfile = conf["logfile"];
        ofs.open(logfile.c_str(), std::ios::out);
        logger::stream(ofs);
    }

    // Construct and initialize the TUI
    logging.info("starting tui...");
    tui::tui term(ncurses);
    if (!term.init())
        return term.end();

    // Refresh the TUI
    term.refresh();

    tasker::context<int> ctx;
    ctx.bind_keys(ctx, conf);

    // TUI input logic, wait-state until quit key is pressed
    int ch;
    while (ctx && (ch = ncurses.getchar())) {
        if (ctx.keybind_exists(ch)) {
            ctx.call_keybind(ch);
        }
    }

    // Restore logger pointer and close any open file stream
    logger::reset();
    ofs.close();

    // End the TUI
    return term.end();
}

int main(int argc, char *argv[])
{
    auto ncurses = tasker::ext::ncurses();
    return tasker_main(ncurses, argc, argv);
}
