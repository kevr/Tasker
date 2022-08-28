#ifndef SRC_UTILITY_HPP
#define SRC_UTILITY_HPP

#include <iostream>
#include <string>
#include <vector>

template <typename T>
int _error(int rc, T &&arg)
{
    std::cerr << arg << std::endl;
    return rc;
}

template <typename T, typename... Args>
int _error(int rc, T &&arg, Args &&...args)
{
    std::cerr << arg;
    return _error(rc, std::forward<Args>(args)...);
}

template <typename... Args>
int error(int rc, Args &&...args)
{
    std::cerr << "error: ";
    return _error(rc, std::forward<Args>(args)...);
}

template <typename T>
int error(int rc, T &&arg)
{
    std::cerr << "error: ";
    return _error(rc, std::move(arg));
}

std::vector<std::string> split(const std::string &str, char delim);
std::string strip(std::string orig, char delim = ' ');

namespace tasker::test
{

/**
 * @brief Creates a tmpdir in /tmp
 *
 * This function is meant to be used with unit tests, thus it sits in
 * the `tasker::test` namespace. Users should __NOT__ use this function
 * outside of tests.
 **/
std::string make_temp_directory();

}; // namespace tasker::test

#endif /* SRC_UTILITY_HPP */
