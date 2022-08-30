#include "logging.hpp"
#include <gtest/gtest.h>

class logging_test : public ::testing::Test
{
protected:
    tasker::logger logger;
    std::stringstream ss;

public:
    void SetUp() override
    {
        tasker::logger::stream(ss);
    }

    void TearDown() override
    {
        tasker::logger::reset();
    }
};

TEST_F(logging_test, info)
{
    logger.info("Test");
    ASSERT_EQ(ss.str(), "[INFO] Test\n");
}

TEST_F(logging_test, warning)
{
    logger.warning("Test");
    ASSERT_EQ(ss.str(), "[WARN] Test\n");
}

TEST_F(logging_test, error)
{
    logger.error("Test");
    ASSERT_EQ(ss.str(), "[ERROR] Test\n");
}

TEST_F(logging_test, debug)
{
    tasker::logger::set_debug(true);
    logger.debug("Test");
    ASSERT_EQ(ss.str(), "[DEBUG] Test\n");
    tasker::logger::set_debug(false);
}
