#ifndef SRC_TESTING_HPP
#define SRC_TESTING_HPP

#include "config.hpp"
#include "utility.hpp"
#include <filesystem>

namespace tasker::test
{

class context
{
private:
    static context instance;

    std::string home;
    std::filesystem::path m_tmpdir;

public:
    context() noexcept;
    ~context() noexcept;

    static context &ref();
};

}; // namespace tasker::test

namespace tasker
{

// Define a global reference to a test_context, which is
// initially bound to singleton test::context::ref().
// This inline static initialization causes test::context::ref()
// to do its job wherever this header is imported, while allowing
// test access to the test::context via test_context, if desired.
inline static test::context &test_context = test::context::ref();

}; // namespace tasker

#define MAKE_ARGS(...)                                                        \
    const char *argv[] = { PROG.data(), __VA_ARGS__ };                        \
    int argc = sizeof(argv) / sizeof(argv[0]);

#endif /* SRC_TESTING_HPP */
