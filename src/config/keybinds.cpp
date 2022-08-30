#include "keybinds.hpp"
using namespace tasker;
namespace po = boost::program_options;

cfg::config &cfg::add_keybind_options(cfg::config &conf)
{
    conf.option("key_quit",
                po::value<char>()->default_value(default_keybinds::KEY_QUIT),
                "quit keybind");
    return conf;
}
