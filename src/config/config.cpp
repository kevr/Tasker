#include "config.hpp"
#include "../config.hpp"
#include "defaults.hpp"
#include <algorithm>
#include <boost/program_options/errors.hpp>
#include <fmt/format.h>
#include <vector>
using namespace tasker;
namespace po = boost::program_options;

static cfg::config conf;
cfg::config *cfg::config::m_conf = &conf;

cfg::config::config() noexcept
{
    m_desc.add_options()("help,h", "produce help message");
    m_desc.add_options()("version,v", "print version string");
    m_desc.add_options()(
        "config,c", po::value<std::string>(), "custom config file path");
    reset();
}

cfg::config &cfg::config::option(const std::string &key,
                                 const std::string &value)
{
    m_config->add_options()(key.c_str(), value.c_str());
    return *this;
}

std::string cfg::config::usage() const
{
    std::string usage("[-hvc] ");
    for (auto &opt : m_config->options()) {
        usage.append("[");
        usage.append(opt->format_name());
        usage.append("] ");
    }
    usage.pop_back();

    return PROG + " " + usage;
}

cfg::config &cfg::config::parse_args(int argc, char *argv[])
{
    return parse_args(argc, const_cast<const char **>(argv));
}

cfg::config &cfg::config::parse_args(int argc, const char *argv[])
{
    po::options_description cmdline;
    cmdline.add(m_desc).add(*m_config);
    po::store(po::parse_command_line(argc, argv, cmdline), m_vars);
    return *this;
}

cfg::config &cfg::config::parse_config(const std::filesystem::path &path)
{
    po::store(po::parse_config_file(path.c_str(), *m_config), m_vars);
    return *this;
}

cfg::config &cfg::config::check_args()
{
    if (auto x = get<int>("style.task_list_width"); x <= 0) {
        auto exc = po::invalid_option_value(std::to_string(x));
        exc.set_option_name("style.task_list_width");
        throw exc;
    }

    return *this;
}

bool cfg::config::exists(const std::string &key) const
{
    return m_vars.count(key);
}

std::string cfg::config::operator[](const std::string &key) const
{
    return get<std::string>(key);
}

void cfg::config::reset()
{
    m_vars.clear();
    m_config = std::make_shared<po::options_description>("Config options");
    m_config->add_options()("color.root_border",
                            po::value<short>()->default_value(4),
                            "8/256 color ordinal");
    m_config->add_options()("color.project_bar_bg",
                            po::value<short>()->default_value(4),
                            "8/256 color ordinal");
    m_config->add_options()("color.project_bar_fg",
                            po::value<short>()->default_value(0),
                            "8/256 color ordinal");
    m_config->add_options()("style.task_list_width",
                            po::value<int>()->default_value(20),
                            "task list width");

    m_config->add_options()(
        "keybindings.quit",
        po::value<char>()->default_value(defaults::keybinds::KEY_QUIT),
        "quit the program");
    m_config->add_options()(
        "keybindings.new_list",
        po::value<char>()->default_value(defaults::keybinds::KEY_NEW_LIST),
        "add a new list to a project board");
}

cfg::config &cfg::config::ref(cfg::config &conf)
{
    m_conf = &conf;
    return ref();
}

cfg::config &cfg::config::ref()
{
    return *m_conf;
}

cfg::config &cfg::config::new_ref()
{
    auto c = ref(conf);
    ref(conf).reset();
    return ref();
}
