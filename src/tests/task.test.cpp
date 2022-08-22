#include "tasks/task.hpp"
#include <gtest/gtest.h>
using namespace tasker;

class task_test : public ::testing::Test
{
protected:
    project proj { { .id = 1 } };
};

TEST_F(task_test, constructs)
{
    task t(proj, { 2 });
}

TEST_F(task_test, copies)
{
    ASSERT_EQ(proj.id(), 1);

    // Construct a task
    task t(proj, { 2 });
    ASSERT_EQ(t.id(), 2);
    ASSERT_EQ(t.parent().id(), 1);

    // Copy construct
    task t2(t);
    ASSERT_EQ(t2.id(), t.id());
    ASSERT_EQ(&t2.parent(), &t.parent());

    // Copy assign
    task t3(proj, { 6 });
    ASSERT_EQ(t3.id(), 6);
    t = t3;
    ASSERT_EQ(t.id(), 6);
    ASSERT_EQ(t.id(), t3.id());
}

TEST_F(task_test, moves)
{
    // Move construct
    task t(proj, { .id = 1, .title = "Title" });
    ASSERT_EQ(t.title(), "Title");
    task t2(std::move(t));
    ASSERT_EQ(t.title(), "");
    ASSERT_EQ(t2.title(), "Title");

    // Move assign
    task t3;
    t3 = std::move(t2);
    ASSERT_EQ(t3.title(), "Title");
    ASSERT_EQ(t3.id(), 1);
}

TEST_F(task_test, id)
{
    task t(proj, { 8 });
    ASSERT_EQ(t.id(), 8);
}

TEST_F(task_test, title)
{
    task t(proj, { .id = 1, .title = "Title" });
    ASSERT_EQ(t.title(), "Title");
}

TEST_F(task_test, body)
{
    task t(proj, { .id = 1, .body = "Body" });
    ASSERT_EQ(t.body(), "Body");
}

TEST_F(task_test, invalid_argument)
{
    ASSERT_THROW(task(proj, { -1 }), std::invalid_argument);
}
