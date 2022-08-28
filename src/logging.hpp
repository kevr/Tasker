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

private:
    void p_info(const std::string &line);
    void p_error(const std::string &line);
    void p_warning(const std::string &line);

public:
    static std::ostream &stream(std::ostream &os);
    static void reset();

    void info(const std::string &msg);
    void error(const std::string &msg);
    void warning(const std::string &msg);
};

}; // namespace tasker

#endif /* SRC_LOGGING_HPP */
