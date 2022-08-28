#include "testing.hpp"
#include "config.hpp"
#include "env.hpp"
#include "utility.hpp"
using namespace tasker;

test::context test::context::instance;

test::context::context() noexcept
{
    // Save the current $HOME value
    home = env::variable("HOME").value();

    // Make a tmpdir with an empty config. Set $HOME to it. This will
    // cause the env search tool to ignore any configs found on the user.
    m_tmpdir = test::make_temp_directory();
    auto tmp_config = m_tmpdir / ".config" / PROG;
    std::filesystem::create_directories(tmp_config);
    tmp_config /= ".config";
    touch(tmp_config);

    setenv("HOME", m_tmpdir.c_str(), 1);
}

test::context::~context() noexcept
{
    // Restore $HOME
    setenv("HOME", home.c_str(), 1);

    // Remove all the tmpfiles we made in the constructor
    std::filesystem::remove_all(m_tmpdir);
}

test::context &test::context::ref()
{
    return instance;
}
