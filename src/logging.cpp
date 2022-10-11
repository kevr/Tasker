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
    out->flush();
}

void logger::error(const std::string &line) const
{
    auto output = fmt::format("[ERROR] {0}\n", line);
    *out << output;
    out->flush();
}

void logger::warning(const std::string &line) const
{
    auto output = fmt::format("[WARN] {0}\n", line);
    *out << output;
    out->flush();
}

void logger::debug(const std::string &line) const
{
    if (debug_enabled) {
        auto output = fmt::format("[DEBUG] {0}\n", line);
        *out << output;
        out->flush();
    }
}

const char *source_path(const std::string &path)
{
    std::string prefix("src/");
    auto pos = path.find(prefix) + prefix.size();

#ifdef TESTING
    // If we're testing, we can also end up with this path added
    // onto the end of the path prefix. This can occur when a
    // test file includes a header relative to itself:
    // #include "../some_file.hpp"
    std::string ext_prefix("tests/../");
    auto ext_pos = path.find(ext_prefix, pos);
    if (ext_pos != std::string::npos) {
        pos = ext_pos + ext_prefix.size();
    }
#endif

    const char *c_str = path.data() + pos;
    return c_str;
}
