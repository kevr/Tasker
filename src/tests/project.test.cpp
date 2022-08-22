#include "projects/project.hpp"
#include <gtest/gtest.h>
using namespace tasker;

TEST(project, constructs)
{
    project({ .id = 1 });
}

TEST(project, copies)
{
    // Construct a project
    project proj({ .id = 1 });

    // Copy construct
    project proj2(proj);
    ASSERT_EQ(proj2.id(), 1);
    ASSERT_EQ(proj.id(), proj2.id());

    // Copy assign
    project proj3({ .id = 6 });
    ASSERT_EQ(proj3.id(), 6);
    proj = proj3;
    ASSERT_EQ(proj.id(), 6);
    ASSERT_EQ(proj.id(), proj3.id());
}

TEST(project, moves)
{
    project proj({ .id = 1, .name = "Tester" });
    ASSERT_EQ(proj.name(), "Tester");
    project proj2(std::move(proj));
    ASSERT_EQ(proj.name(), "");
    ASSERT_EQ(proj2.name(), "Tester");

    project proj3;
    proj3 = std::move(proj2);
    ASSERT_EQ(proj2.name(), "");
    ASSERT_EQ(proj3.name(), "Tester");
}

TEST(project, id)
{
    project proj({ .id = 1 });
    ASSERT_EQ(proj.id(), 1);
}

TEST(project, names)
{
    project proj({ .id = 1, .name = "Tester", .full_name = "Full Tester" });
    ASSERT_EQ(proj.name(), "Tester");
    ASSERT_EQ(proj.full_name(), "Full Tester");
}

TEST(project, invalid_argument)
{
    ASSERT_THROW(project({ -1 }), std::invalid_argument);
}
