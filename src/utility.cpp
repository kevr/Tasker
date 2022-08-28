#include "utility.hpp"
#include "config.hpp"
#include <algorithm>
#include <fstream>
#include <stdexcept>
using namespace tasker;

std::vector<std::string> split(const std::string &str, char delim)
{
    std::vector<std::string> v;

    size_t old = 0;
    size_t pos = str.find(delim, old);
    while (pos != std::string::npos) {
        auto piece = str.substr(old, pos - old);
        v.emplace_back(std::move(piece));
        old = pos + 1;
        pos = str.find(delim, old);
    }

    auto piece = str.substr(old, str.size() - old);
    v.emplace_back(std::move(piece));

    return v;
}

std::string strip(std::string orig, char delim)
{
    orig.erase(std::remove(orig.begin(), orig.end(), delim), orig.end());
    return orig;
}

void touch(std::filesystem::path path)
{
    std::ofstream ofs(path, std::ios::out);
    ofs.close();
}

std::string test::make_temp_directory()
{
    std::filesystem::path p(std::filesystem::temp_directory_path());
    p /= PROG + "_XXXXXX";

    std::string tmpdir(p.string());
    if (!mkdtemp(tmpdir.data())) {
        // LCOV_EXCL_START
        // This function is only used in test code. This check is used
        // as a last resort which should cause tests to error out on
        // failure.
        throw std::runtime_error("unable to mkdir " + tmpdir);
        // LCOV_EXCL_STOP
    }

    return tmpdir;
}
