#include "tasks/task.hpp"
#include <gtest/gtest.h>
using namespace tasker;

TEST(task_test, constructs)
{
    task t({ 2 });
}

TEST(task_test, copies)
{
    // Construct a task
    task t({ 2 });
    ASSERT_EQ(t.id(), 2);

    // Copy construct
    task t2(t);
    ASSERT_EQ(t2.id(), t.id());

    // Copy assign
    task t3({ 6 });
    ASSERT_EQ(t3.id(), 6);
    t = t3;
    ASSERT_EQ(t.id(), 6);
    ASSERT_EQ(t.id(), t3.id());
}

TEST(task_test, moves)
{
    // Move construct
    task t({ .id = 1, .title = "Title" });
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

TEST(task_test, id)
{
    task t({ 8 });
    ASSERT_EQ(t.id(), 8);
}

TEST(task_test, title)
{
    task t({ .id = 1, .title = "Title" });
    ASSERT_EQ(t.title(), "Title");
}

TEST(task_test, body)
{
    task t({ .id = 1, .body = "Body" });
    ASSERT_EQ(t.body(), "Body");
}

TEST(task_test, invalid_argument)
{
    ASSERT_THROW(task({ -1 }), std::invalid_argument);
}
