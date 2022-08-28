#include "logging.hpp"
using namespace tasker;

std::ostream *logger::out = &std::cout;

std::ostream &logger::stream(std::ostream &os)
{
    out = &os;
    return *out;
}

void logger::reset()
{
    out = &std::cout;
}

void logger::p_info(const std::string &line)
{
    auto output = fmt::format("[INFO] {0}\n", line);
    *out << output;
}

void logger::p_error(const std::string &line)
{
    auto output = fmt::format("[ERROR] {0}\n", line);
    *out << output;
}

void logger::p_warning(const std::string &line)
{
    auto output = fmt::format("[WARN] {0}\n", line);
    *out << output;
}

void logger::info(const std::string &msg)
{
    p_info(msg);
}

void logger::error(const std::string &msg)
{
    p_error(msg);
}

void logger::warning(const std::string &msg)
{
    p_warning(msg);
}
