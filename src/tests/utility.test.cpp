#include "utility.hpp"
#include "config.hpp"
#include <filesystem>
#include <gtest/gtest.h>
using namespace tasker;

TEST(utility, split)
{
    auto pieces = split("a;bc;d", ';');
    ASSERT_EQ(pieces[0], "a");
    ASSERT_EQ(pieces[1], "bc");
    ASSERT_EQ(pieces[2], "d");

    pieces = split(";bc;d", ';');
    ASSERT_EQ(pieces[0], "");
    ASSERT_EQ(pieces[1], "bc");
    ASSERT_EQ(pieces[2], "d");

    pieces = split("a;bc;", ';');
    ASSERT_EQ(pieces[0], "a");
    ASSERT_EQ(pieces[1], "bc");
    ASSERT_EQ(pieces[2], "");

    pieces = split("", ';');
    ASSERT_EQ(pieces.size(), 1);
    ASSERT_EQ(pieces[0], "");
}

TEST(utility, strip)
{
    ASSERT_EQ(strip("a;bc;d", ';'), "abcd");
}

TEST(utility, make_temp_directory)
{
    // Create a tmpdir.
    auto tmpdir = test::make_temp_directory();

    // Assert that the name format is correct and the resulting
    // tmpdir exists in the filesystem.
    ASSERT_NE(tmpdir.find("/tmp/" + PROG + "_"), std::string::npos);
    ASSERT_TRUE(std::filesystem::exists(tmpdir));

    // Remove the tmpdir we created.
    std::filesystem::remove_all(tmpdir);
    ASSERT_FALSE(std::filesystem::exists(tmpdir));
}
