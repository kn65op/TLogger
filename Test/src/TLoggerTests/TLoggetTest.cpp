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

TEST_F(TLoggerTest, CreateFileWithDate)
{
  LoggerFacade logger(LoggerType::FILE, LogFileOnEntry::CREATE_WITH_DATE, LogFileOnExit::REMOVE);
  LOG << "log";
}

TEST_F(TLoggerTest, CreateFileWithDateAndFilename)
{
  LoggerFacade logger(LoggerType::FILE, LogFileOnEntry::CREATE_WITH_DATE, LogFileOnExit::REMOVE, "logFile-");
  LOG << "log";
}