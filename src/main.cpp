/**
 * Description: Main program entry-point.
 *
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "callback.hpp"
#include "config.hpp"
#include "config/config.hpp"
#include "context.hpp"
#include "env.hpp"
#include "logging.hpp"
#include "ncurses.hpp"
#include "tui.hpp"
#include "utility.hpp"
#include <boost/program_options/errors.hpp>
#include <chrono>
#include <iostream>
#include <thread>
using namespace tasker;

static logger logging;
static std::ofstream ofs;

int tasker_main(ext::ncurses &ncurses, int argc, const char *argv[])
{
    namespace po = boost::program_options;

    // Parse command line arguments and handle them.
    auto &conf = cfg::config::ref();
    conf.option("debug,d", "enable debug logging");
    conf.option("logfile,l",
                po::value<std::string>(),
                "designate a log file instead of stderr");

    try {
        conf.parse_args(argc, argv);
    } catch (boost::program_options::error &e) {
        auto str = fmt::format("error: {0}\n", e.what());
        return raw_error(ERROR_ARGS, str);
    }

    if (conf.exists("help")) {
        std::cout << "usage: " << conf.usage() << std::endl
                  << conf << std::endl;
        return OK;
    }

    if (conf.exists("version")) {
        std::cout << VERSION << std::endl;
        return OK;
    }

    // The user can optionally provide --config
    std::optional<std::filesystem::path> config_path;

    // Set config_path where possible
    if (conf.exists("config")) {
        config_path = conf.get<std::string>("config");
    } else {
        config_path = env::search_config_path();
    }

    // If a valid config_path was determined, parse the config.
    if (config_path.has_value()) {
        try {
            conf.parse_config(config_path.value());
        } catch (boost::program_options::error &e) {
            auto str = fmt::format(
                "error: {0}\nthe configuration read was located at {1}\n",
                e.what(),
                config_path->string());
            return raw_error(ERROR_CONFIG, str);
        }
    }

    try {
        conf.check_args();
    } catch (boost::program_options::validation_error &e) {
        auto str = fmt::format("error: {0}\n", e.what());
        return raw_error(ERROR_VALIDATE, str);
    }

    if (conf.exists("debug")) {
        logger::set_debug(true);
    }

    if (conf.exists("logfile")) {
        auto logfile = conf["logfile"];
        ofs.open(logfile.c_str(), std::ios::out | std::ios::app);
        logger::stream(ofs);
    } else {
        // Use default log file 'tasker.log'
        ofs.open("tasker.log", std::ios::out | std::ios::app);
        logger::stream(ofs);
    }

    // Construct and initialize the TUI
    logging.info("===== BEGIN SESSION =====");
    logging.debug(LOGTRACE());
    tui::tui term(ncurses);
    if (!term.init())
        return term.end();

    auto message =
        fmt::format("supported colors: {0}", ncurses.supported_colors());
    logging.info(LOG(message));

    // Refresh the TUI
    term.refresh();

    auto root = term.get_root();
    root->context.bind_keys(conf);

    auto resize = [&]() -> int {
        auto root = term.get_root();
        int x, y;
        ncurses.get_max_yx(root->handle(), y, x);
        if (x > 22) {
            term.resize();
        }

        return OK;
    };
    root->context.keybinds[KEY_RESIZE] = resize;

    // TUI input logic, wait-state until quit key is pressed
    safe_input_loop(ncurses, root->context);

    // End the TUI
    logging.debug(LOG("ending tui"));
    auto rc = term.end();

    // Restore logger pointer and close any open file stream
    logging.info("===== END SESSION =====");
    logger::reset();
    ofs.close();

    return rc;
}

int main(int argc, const char *argv[])
{
    auto ncurses = tasker::ext::ncurses();
    return tasker_main(ncurses, argc, argv);
}
