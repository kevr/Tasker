#include "users/user.hpp"
#include <gtest/gtest.h>
using namespace tasker;

class user_test : public ::testing::Test
{
protected:
    user u { { 1, "Tester" } };
};

TEST_F(user_test, constructs)
{
    ASSERT_EQ(u.id(), 1);
    ASSERT_EQ(u.login(), "Tester");
}

TEST_F(user_test, copies)
{
    // Copy construct
    user u2(u);
    ASSERT_EQ(u.id(), 1);
    ASSERT_EQ(u.login(), "Tester");
    ASSERT_EQ(u2.id(), u.id());
    ASSERT_EQ(u2.login(), u.login());

    // Copy assign
    user u3;
    u3 = u;
    ASSERT_EQ(u3.id(), 1);
    ASSERT_EQ(u3.login(), "Tester");
}

TEST_F(user_test, moves)
{
    // Move construct
    user u2(std::move(u));
    ASSERT_EQ(u2.id(), 1);
    ASSERT_EQ(u2.login(), "Tester");
    ASSERT_EQ(u.login(), "");

    // Move assign
    user u3({ 3, "Tester3" });
    u = std::move(u3);
    ASSERT_EQ(u.id(), 3);
    ASSERT_EQ(u.login(), "Tester3");
    ASSERT_EQ(u3.login(), "");
}

TEST_F(user_test, id)
{
    ASSERT_EQ(u.id(), 1);
}

TEST_F(user_test, login)
{
    ASSERT_EQ(u.login(), "Tester");
}
