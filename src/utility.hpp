#ifndef SRC_UTILITY_HPP
#define SRC_UTILITY_HPP

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

int error(int rc, const std::string &message);
std::vector<std::string> split(const std::string &str, char delim);
std::string strip(std::string orig, char delim = ' ');
void touch(std::filesystem::path path);

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
