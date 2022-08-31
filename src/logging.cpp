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

void logger::info(const std::string &line) const
{
    auto output = fmt::format("[INFO] {0}\n", line);
    *out << output;
}

void logger::error(const std::string &line) const
{
    auto output = fmt::format("[ERROR] {0}\n", line);
    *out << output;
}

void logger::warning(const std::string &line) const
{
    auto output = fmt::format("[WARN] {0}\n", line);
    *out << output;
}

void logger::debug(const std::string &line) const
{
    if (debug_enabled) {
        auto output = fmt::format("[DEBUG] {0}\n", line);
        *out << output;
    }
}

const char *source_path(const std::string &path)
{
    std::string prefix("src/");

#ifdef TESTING
    // When testing, source code pathing is different than in
    // the binary. Adjust here via macro.
    prefix.append("tests/../");
#endif

    auto pos = path.find(prefix);
    pos += prefix.size();

    const char *c_str = path.data() + pos;
    return c_str;
}
