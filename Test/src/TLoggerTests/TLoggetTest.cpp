#include <TLogger/inc/TLogger.h>

#include <gtest/gtest.h>
using namespace ::testing;
using namespace TLogger;

struct TLoggerTest : Test
{
  
};

TEST_F(TLoggerTest, ChechNoThrowOfContructorDefault)
{
  LoggerFacade logger;
  logger.getStreamWithDate() << "Only Date";
}

TEST_F(TLoggerTest, ChechNoThrowOfContructorWithAppendAndDestructorRemove)
{
  LoggerFacade logger(LogFileOnEntry::APPEND, LogFileOnExit::REMOVE);
  LOG << "log";
}


TEST(TimeTest, Time)
{
}