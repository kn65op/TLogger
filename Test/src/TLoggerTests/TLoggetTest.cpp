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

TEST_F(TLoggerTest, StdOutLoggerTest)
{
  LoggerFacade logger(LoggerType::STDOUT);
  LOG << "log";
}

TEST_F(TLoggerTest, StdErrLoggerTest)
{
  LoggerFacade logger(LoggerType::STDERR);
  LOG << "log";
}

TEST(TimeTest, Time)
{
}