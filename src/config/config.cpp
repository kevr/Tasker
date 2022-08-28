#include "config.hpp"
#include "../config.hpp"
#include <algorithm>
#include <vector>
using namespace tasker;
namespace po = boost::program_options;

static cfg::config conf;
cfg::config *cfg::config::m_conf = &conf;

cfg::config::config() noexcept
{
    m_desc.add_options()("help,h", "produce help message");
    m_desc.add_options()("version,v", "print version string");
}

cfg::config &cfg::config::option(const std::string &key,
                                 const std::string &value)
{
    m_config->add_options()(key.c_str(), value.c_str());
    return *this;
}

std::string cfg::config::usage() const
{
    std::string usage("[-hv] ");
    for (auto &opt : m_config->options()) {
        usage.append("[");
        usage.append(opt->format_name());
        usage.append("] ");
    }
    usage.pop_back();

    return PROG + " " + usage;
}

cfg::config &cfg::config::parse_args(int ac, char *av[])
{
    po::options_description cmdline;
    cmdline.add(m_desc).add(*m_config);
    po::store(po::parse_command_line(ac, av, cmdline), m_vars);
    return *this;
}

cfg::config &cfg::config::parse_config(const std::filesystem::path &path)
{
    po::store(po::parse_config_file(path.c_str(), *m_config), m_vars);
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