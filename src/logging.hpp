#ifndef SRC_LOGGING_HPP
#define SRC_LOGGING_HPP

#include "utility.hpp"
#include <boost/core/demangle.hpp>
#include <fmt/format.h>
#include <iostream>

namespace tasker
{

class logger
{
private:
    static std::ostream *out;
    static bool debug_enabled;

public:
    static std::ostream &stream(std::ostream &os);
    static void reset();
    static void set_debug(bool enabled);

    void info(const std::string &line) const;
    void error(const std::string &line) const;
    void warning(const std::string &line) const;
    void debug(const std::string &line) const;
};

}; // namespace tasker

const char *source_path(const std::string &path);

#define LOGTRACE()                                                            \
    fmt::format("[TRACE:{0}#L{1}] {2}()",                                     \
                source_path(__FILE__),                                        \
                __LINE__,                                                     \
                __FUNCTION__)

#define R_LOGTRACE()                                                          \
    fmt::format("[TRACE:{0}#L{1}] {2}::{3}()",                                \
                source_path(__FILE__),                                        \
                __LINE__,                                                     \
                boost::core::demangle(typeid(*this).name()),                  \
                __FUNCTION__)

#define LOG(message)                                                          \
    fmt::format("[{0}#L{1}] {2}", source_path(__FILE__), __LINE__, message)

#endif /* SRC_LOGGING_HPP */
