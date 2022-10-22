#include "config.hpp"
#include "../config.hpp"
#include "defaults.hpp"
#include "env.hpp"
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
    cmdline_option("help,h", "produce help message");
    cmdline_option("version,v", "print version string");
    cmdline_option("debug,d", "enable debug logging");
    cmdline_option("logfile,l",
                   po::value<std::string>()->composing(),
                   "designate a log file instead of stdout");
    cmdline_option(
        "config,c", po::value<std::string>(), "custom config file path");
    cmdline_option("show-config", "display the parsed configuration");

    reset();
}

cfg::config &cfg::config::cmdline_option(const std::string &key,
                                         const std::string &help)
{
    m_desc.add_options()(key.c_str(), help.c_str());
    return *this;
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
    po::options_description config;
    config.add(m_desc).add(*m_config);
    po::store(po::parse_config_file(path.c_str(), config), m_vars);
    return *this;
}

cfg::config &cfg::config::check_args()
{
    if (auto x = get<int>("style.task_list.width"); x <= 0) {
        auto exc = po::invalid_option_value(std::to_string(x));
        exc.set_option_name("style.task_list.width");
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

std::ostream &cfg::config::show(std::ostream &os)
{
    auto conf_path = env::search_config_path();
    if (exists("config")) {
        conf_path = get<std::string>("config");
    }

    std::string logfile("stdout");
    if (exists("logfile")) {
        logfile = "\"" + get<std::string>("logfile") + "\"";
    }

    os << "config: " << conf_path.value() << std::endl
       << "debug: " << (exists("debug") ? "true" : "false") << std::endl
       << "logfile: " << logfile << std::endl
       << "color.root_border: " << get<short>("color.root_border") << std::endl
       << "color.project_bar_bg: " << get<short>("color.project_bar_bg")
       << std::endl
       << "color.project_bar_fg: " << get<short>("color.project_bar_fg")
       << std::endl
       << "style.task_list.width: " << get<int>("style.task_list.width")
       << std::endl
       << "keybindings.quit: '" << get<char>("keybindings.quit") << "'"
       << std::endl
       << "keybindings.project.new_list: '"
       << get<char>("keybindings.project.new_list") << "'" << std::endl;

    return os;
}

void cfg::config::reset()
{
    m_vars.clear();
    m_config = std::make_shared<po::options_description>("Config options");

    option("color.root_border",
           po::value<short>()->default_value(4),
           "8/256 color ordinal");
    option("color.project_bar_bg",
           po::value<short>()->default_value(4),
           "8/256 color ordinal");
    option("color.project_bar_fg",
           po::value<short>()->default_value(0),
           "8/256 color ordinal");
    option("style.task_list.width",
           po::value<int>()->default_value(20),
           "task list width");
    option("keybindings.quit",
           po::value<char>()->default_value(defaults::keybinds::KEY_QUIT),
           "quit the program");
    option("keybindings.project.new_list",
           po::value<char>()->default_value(defaults::keybinds::KEY_NEW_LIST),
           "add a new list to the project board");
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
