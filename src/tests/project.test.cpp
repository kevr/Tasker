#include "projects/project.hpp"
#include <gtest/gtest.h>
using namespace tasker;

TEST(project, constructs)
{
    project(1);
}

TEST(project, copies)
{
    // Construct a project
    project proj(1);

    // Copy construct
    project proj2(proj);
    ASSERT_EQ(proj2.id(), 1);
    ASSERT_EQ(proj.id(), proj2.id());

    // Copy assign
    project proj3(6);
    ASSERT_EQ(proj3.id(), 6);
    proj = proj3;
    ASSERT_EQ(proj.id(), 6);
    ASSERT_EQ(proj.id(), proj3.id());
}

TEST(project, id)
{
    project proj(1);
    ASSERT_EQ(proj.id(), 1);
}

TEST(project, invalid_argument)
{
    ASSERT_THROW(project(-1), std::invalid_argument);
}
