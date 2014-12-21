#include <TLogger/inc/TLogger.h>

#include <gtest/gtest.h>
using namespace ::testing;
using namespace TLogger;

struct TLoggerTest : Test
{
  
};

TEST_F(TLoggerTest, ChechNoThrowOfContructorDefault)
{
  Logger logger;
}

TEST_F(TLoggerTest, ChechNoThrowOfContructorWithAppendAndDestructorRemove)
{
  Logger logger{LogFileOnEntry::APPEND, LogFileOnExit::REMOVE};
}
