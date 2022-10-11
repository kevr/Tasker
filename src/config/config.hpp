#ifndef SRC_CONFIG_CONFIG_HPP
#define SRC_CONFIG_CONFIG_HPP

#include <boost/program_options.hpp>
#include <filesystem>
#include <fstream>
#include <map>
#include <stdexcept>
#include <tuple>

#ifdef TESTING
// Include testing.hpp if -DTESTING was given to resolve
// static symbols defined which setup HOME redirection
// for tests. This is located here in the config module
// because HOME lookup is only done during config assembly.
#include "testing.hpp"
#endif

namespace tasker::cfg
{

struct config_error : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class config
{
private:
    /**
     * @brief Static config reference pointer
     *
     * This pointer is statically initialized within the config.cpp
     * translation unit to a TU-static instance of config.
     **/
    static config *m_conf;

    boost::program_options::variables_map m_vars;

    //! Command-line program options description.
    boost::program_options::options_description m_desc { "Program options" };

    /**
     * @brief Configuration file program options description.
     *
     * This member is a shared_ptr because want the ability to completely
     * reset it. shared_ptr allows us to cleanly allocate a new object
     * to replace the old.
     **/
    std::shared_ptr<boost::program_options::options_description> m_config {
        std::make_shared<boost::program_options::options_description>(
            "Config options")
    };

public:
    config() noexcept;
    virtual ~config() = default;

    config &option(const std::string &key, const std::string &help);

    template <typename T>
    config &option(const std::string &key, T &&arg, const std::string &help)
    {
        m_config->add_options()(key.c_str(), std::move(arg), help.c_str());
        return *this;
    }

    std::string usage() const;

    config &parse_args(int argc, char *argv[]);
    config &parse_config(const std::filesystem::path &path);

    //! Perform further argument validation specific to this program
    config &check_args();

    bool exists(const std::string &key) const;

    template <typename T>
    T get(const std::string &key) const
    {
        return m_vars.at(key).as<T>();
    }

    template <typename T>
    T get(const std::string &key, const T &default_value)
    {
        if (!m_vars.count(key)) {
            return default_value;
        }
        return get<T>(key);
    }

    std::string operator[](const std::string &key) const;

    //! Reset configuration options and parsed variables
    void reset();

    /**
     * @brief Change the static config reference to `conf` and return it
     **/
    static config &ref(config &conf);

    /**
     * @brief Return static config reference
     **/
    static config &ref();

    /**
     * @brief Restore original static reference, reset it and return it
     **/
    static config &new_ref();

private:
    friend std::ostream &operator<<(std::ostream &os, const config &conf)
    {
        boost::program_options::options_description cmdline;
        cmdline.add(conf.m_desc);

        if (conf.m_config->options().size()) {
            cmdline.add(*conf.m_config);
        }

        os << cmdline;
        return os;
    }
};

}; // namespace tasker::cfg

#endif /* SRC_CONFIG_CONFIG_HPP */
