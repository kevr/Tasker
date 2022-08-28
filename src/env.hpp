#ifndef SRC_ENV_HPP
#define SRC_ENV_HPP

#include <filesystem>
#include <optional>
#include <string>
#include <utility>

namespace tasker::env
{

class variable
{
private:
    std::pair<std::string, std::string> m_kv;

public:
    variable() = default;

    variable(const std::string &key);

    variable(variable &&) = default;
    variable &operator=(variable &&) = default;
    variable(const variable &) = default;
    variable &operator=(const variable &) = default;

    const std::string &key() const;
    const std::string &value() const;

    bool operator==(const variable &v) const;
    bool operator==(const std::string &v) const;

    friend std::ostream &operator<<(std::ostream &os, const variable &v)
    {
        os << v.m_kv.second;
        return os;
    }
};

std::optional<std::filesystem::path> search_config_path();

}; // namespace tasker::env

#endif /* SRC_ENV_HPP */
