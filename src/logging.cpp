#include "logging.hpp"
using namespace tasker;

std::ostream *logger::out = &std::cout;
bool logger::debug_enabled = false;

std::ostream &logger::stream(std::ostream &os)
{
    out = &os;
    return *out;
}

void logger::reset()
{
    out = &std::cout;
}

void logger::set_debug(bool enabled)
{
    debug_enabled = enabled;
}

void logger::info(const std::string &line)
{
    auto output = fmt::format("[INFO] {0}\n", line);
    *out << output;
}

void logger::error(const std::string &line)
{
    auto output = fmt::format("[ERROR] {0}\n", line);
    *out << output;
}

void logger::warning(const std::string &line)
{
    auto output = fmt::format("[WARN] {0}\n", line);
    *out << output;
}

void logger::debug(const std::string &line)
{
    if (debug_enabled) {
        auto output = fmt::format("[DEBUG] {0}\n", line);
        *out << output;
    }
}
