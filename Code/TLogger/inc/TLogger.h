#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <memory>
#include <ctime>

namespace TLogger
{

#define LOG LoggerFacade::getLoggerFacade().getStreamWithDate(__FILE__, __LINE__, __PRETTY_FUNCTION__)

enum class LogFileOnEntry
{
  OVERRIDE,
  APPEND,
  THROW_EXCEPTION
};

enum class LogFileOnExit
{
  REMOVE,
  DO_NOTHING
};

class Logger
{
public:

  ~Logger()
  {
    log_file << "\nremoving logger\n";
    switch (file_on_exit)
    {
    case LogFileOnExit::REMOVE:
    {
      std::remove(filename.c_str());
      break;
    }
    case LogFileOnExit::DO_NOTHING: // Fallthrough
    default:
    {
    }
    }
  }
private:
  friend class LoggerFacade;
  friend class std::unique_ptr < Logger >;

  Logger(LogFileOnEntry p_file_on_entry, LogFileOnExit p_file_on_exit) :
    file_on_entry(p_file_on_entry), file_on_exit(p_file_on_exit)
  {
    switch (file_on_entry)
    {
    case LogFileOnEntry::OVERRIDE:
      log_file.open(filename, std::ios::trunc);
      break;
    case LogFileOnEntry::APPEND:
      log_file.open(filename, std::ios::app);
      break;
    case LogFileOnEntry::THROW_EXCEPTION:
      throw std::runtime_error("Not yet implemeted");
    }
    log_file << "creating logger";
  }

  std::ostream & getStream()
  {
    log_file.close();
    log_file.open(filename, std::ios::app);
    return log_file;
  }

  const std::string filename = "TLogger.log";
  std::ofstream log_file;

  LogFileOnEntry file_on_entry;
  LogFileOnExit file_on_exit;
};

class LoggerFacade
{
private:
  class LoggerFacadeInstance;
public:

  LoggerFacade(LogFileOnEntry p_file_on_entry = LogFileOnEntry::OVERRIDE,
               LogFileOnExit p_file_on_exit = LogFileOnExit::DO_NOTHING)
  {
    if (!ref_count++)
    {
      logger_facade_inst_priv.reset(new LoggerFacadeInstance(p_file_on_entry,
                                                             p_file_on_exit));
      getStreamWithDate() << "Starting logging";
    }
  }

  ~LoggerFacade()
  {
    if (!--ref_count)
    {
      getStreamWithDate() << "Ending logging";
      logger_facade_inst_priv.reset(nullptr);
    }
  }

  std::ostream & getStream()const
  {
    return logger_facade_inst_priv->getStream();
  }

  std::ostream & getStreamWithDate(const std::string & level) const
  {
    getStream() << level;
    return getStreamWithDate();
  }

  std::ostream & getStreamWithDate() const
  {
    std::time_t t = std::time(NULL);
    char mbstr[100];
    if (std::strftime(mbstr, sizeof (mbstr), "%A %c", std::localtime(&t)))
    {
      return getStream() << mbstr << " ";
    }
    throw std::runtime_error("Unable to get date");
  }

  std::ostream & getStreamWithDate(const std::string & level,
                                   const std::string & file,
                                   int line,
                                   const std::string & function) const
  {
    return getStreamWithDate(level) << " " << file << ":" << line
      << ": " << function << ": ";
  }

  std::ostream & getStreamWithDate(const std::string & file,
                                   int line,
                                   const std::string & function) const
  {
    return getStreamWithDate() << " " << file << ":" << line <<
      ": " << function << ": ";
  }

  static LoggerFacade & getLoggerFacade()
  {
    return *logger_facade_inst;
  }

private:

  class LoggerFacadeInstance
  {
  public:

    LoggerFacadeInstance(LogFileOnEntry p_file_on_entry,
                         LogFileOnExit p_file_on_exit)
    {
      logger.reset(new Logger(p_file_on_entry, p_file_on_exit));
    }

    std::ostream & getStream()const
    {
      return logger->getStream() << "\n";
    }
  private:
    std::unique_ptr < Logger > logger;
  };

  static unsigned ref_count;
  static std::unique_ptr < LoggerFacadeInstance > logger_facade_inst_priv;
  static std::unique_ptr < LoggerFacade > logger_facade_inst;
  static bool instance_created;
};

}


