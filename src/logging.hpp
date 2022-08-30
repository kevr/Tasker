#ifndef SRC_LOGGING_HPP
#define SRC_LOGGING_HPP

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

    void info(const std::string &line);
    void error(const std::string &line);
    void warning(const std::string &line);
    void debug(const std::string &line);
};

}; // namespace tasker

#endif /* SRC_LOGGING_HPP */
