#include "env.hpp"
#include "config.hpp"
#include <cstdlib>
#include <stdexcept>
#include <unistd.h>
#include <vector>
using namespace tasker;

env::variable::variable(const std::string &key)
    : m_kv(key, "")
{
    auto cs = std::getenv(key.c_str());
    if (!cs) {
        throw std::runtime_error("no environment variable '" + key +
                                 "' found");
    }
    m_kv.second = std::string(cs);
}

const std::string &env::variable::key() const
{
    return m_kv.first;
}

const std::string &env::variable::value() const
{
    return m_kv.second;
}

bool env::variable::operator==(const env::variable &v) const
{
    return m_kv == v.m_kv;
}

bool env::variable::operator==(const std::string &v) const
{
    return value() == v;
}

std::optional<std::filesystem::path> env::search_config_path()
{
    using std::filesystem::exists;
    using std::filesystem::path;

    auto home = env::variable("HOME");
    std::vector<path> paths = {
        // ~/.config/tasker/config
        path(home.value()) / ".config" / PROG / "config",

        // /etc/tasker/config
        path("/etc") / PROG / "config",
    };

    // Iterate over `paths` 0..n; return the first existing path.
    for (auto &p : paths) {
        if (exists(p)) {
            return p;
        }
    }

    return std::nullopt;
}
