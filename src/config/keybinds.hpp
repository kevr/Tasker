#ifndef SRC_CONFIG_KEYBINDS_HPP
#define SRC_CONFIG_KEYBINDS_HPP

#include "config.hpp"
#include "context.hpp"

namespace tasker::cfg
{

enum default_keybinds : int {
    KEY_QUIT = 'q',
};

cfg::config &add_keybind_options(cfg::config &conf);

}; // namespace tasker::cfg

#endif /* SRC_CONFIG_KEYBINDS_HPP */
