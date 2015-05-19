#include <TLogger/inc/TLogger.h>

#include <gtest/gtest.h>
using namespace ::testing;
using namespace TLogger;

struct TLoggerTest : Test
{

};

TEST_F(TLoggerTest, ChechNoThrowOfContructorFile)
{
  LoggerFacade logger(LoggerType::FILE);
  logger.getStreamWithDate() << "Only Date";
}

TEST_F(TLoggerTest, ChechNoThrowOfContructorWithAppendAndDestructorRemove)
{
  LoggerFacade logger(LoggerType::FILE, LogFileOnEntry::APPEND, LogFileOnExit::REMOVE);
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

TEST_F(TLoggerTest, ChechWhatToDoOnCrash)
{
  LoggerFacade logger(LoggerType::FILE);
  LOG << "before crash";
  LOG << "just before crash";
  throw 8;
  LOG << "just after crash";
}