#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <exception>
#include <cstdio>
#include <memory>
#include <ctime> //TODO: remove when std::put_time will be available in gcc
#include <sstream>
#include <iomanip>
#include <chrono>

namespace TLogger
{

#define LOG TLogger::LoggerFacade::getLoggerFacade().getStreamWithDate(__FILE__, __LINE__, __FUNCTION__)

enum class LogFileOnEntry
{
  OVERRIDE,
  APPEND,
  CREATE_WITH_DATE,
  THROW_EXCEPTION
};

enum class LogFileOnExit
{
  REMOVE,
  DO_NOTHING
};

enum class LoggerType
{
  STDOUT,
  STDERR,
  FILE
};

class ILogger
{
public:

  virtual ~ILogger()
  {
  }

  virtual std::ostream & getStream() = 0;
};

class FileLogger : public ILogger
{
public:

  ~FileLogger()
  {
    log_file << "\nremoving logger\n";
    switch (file_on_exit)
    {
      case LogFileOnExit::REMOVE:
      {
        log_file.close();
        if (std::remove(filename.c_str()))
        {
          std::cerr << "Unable to remove log file: " << filename << "\n";
        }
        break;
      }
      case LogFileOnExit::DO_NOTHING: // Fallthrough
      default:
        {
      }
    }
  }

  std::ostream & getStream()
  {
    log_file.close();
    log_file.open(filename, std::ios::app);
    return log_file;
  }
private:
  friend class std::unique_ptr<FileLogger>;
  friend class LoggerFacade;

  class TerminateHandler
  {
  public:
    ~TerminateHandler()
    {
      std::set_terminate(old_handler);
    }
    TerminateHandler(FileLogger *file_logger)
    {
      logger = file_logger;
      old_handler = std::set_terminate(&TerminateHandler::handler);
    }

  private:
    static void handler()
    {
      std::cerr << "std::terminate called - logs written to: " << logger->filename << "\n";
      logger->getStream() << "\nstd::terminate called!\n";
      logger->getStream();

      old_handler();
    }

    static FileLogger *logger;
    static std::terminate_handler old_handler;
  };

  FileLogger(LogFileOnEntry p_file_on_entry, LogFileOnExit p_file_on_exit, const std::string& filename_prefix = "TLogger") :
             prefix(filename_prefix),
             file_on_exit(p_file_on_exit),
             file_on_entry(p_file_on_entry),
             terminate_handler(this)
  {
    switch (file_on_entry)
    {
      case LogFileOnEntry::OVERRIDE:
        filename = prefix + extension;
        log_file.open(filename, std::ios::trunc);
        break;
      case LogFileOnEntry::APPEND:
        filename = prefix + extension;
        log_file.open(filename, std::ios::app);
        break;
      case LogFileOnEntry::CREATE_WITH_DATE:
      {
        std::time_t t = std::time(NULL);
        char mbstr[100];
        if (!std::strftime(mbstr, sizeof(mbstr), "%Y%m%d%H%M%S", std::localtime(&t)))
        {
          throw std::runtime_error("Unable to get date");
        }
        filename = prefix + mbstr + extension;
        log_file.open(filename, std::ios::trunc);
        break;
      }
      case LogFileOnEntry::THROW_EXCEPTION:
        throw std::runtime_error("Not yet implemeted");
    }
    log_file << "creating logger";
  }

  std::string filename;
  const std::string prefix;
  const std::string extension = ".log";
  std::ofstream log_file;

  LogFileOnExit file_on_exit;
  LogFileOnEntry file_on_entry;
  TerminateHandler terminate_handler;
};

class StdOutLogger : public ILogger
{
public:

  virtual ~StdOutLogger()
  {
    std::cout << "\n";
  }

  virtual std::ostream& getStream()
  {
    return std::cout;
  }
};

class StdErrLogger : public ILogger
{
public:

  virtual ~StdErrLogger()
  {
    std::cout << "\n";
  }

  virtual std::ostream& getStream()
  {
    return std::cerr;
  }
};

class LoggerFacade
{
public:

  LoggerFacade(LoggerType p_type,
               LogFileOnEntry p_file_on_entry = LogFileOnEntry::OVERRIDE,
               LogFileOnExit p_file_on_exit = LogFileOnExit::DO_NOTHING,
               const std::string& prefix = "TLogger")
  {
    logger_facade_inst.reset(new LoggerFacade);
    switch (p_type)
    {
      case LoggerType::FILE:
        if (!ref_count++)
        {
          logger_facade_inst_priv.reset(new LoggerFacadeInstance(p_file_on_entry,
              p_file_on_exit, prefix));
          getStreamWithDate() << "Starting logging";
        }
        break;
      case LoggerType::STDOUT: //Fallthrough
      case LoggerType::STDERR:
        if (!ref_count++)
        {
          logger_facade_inst_priv.reset(new LoggerFacadeInstance(p_type));
          getStreamWithDate() << "Starting logging";
        }
        break;
      default:
        throw std::runtime_error("Invalid logger type");
    }
  }

  ~LoggerFacade()
  {
    if (ref_counted)
    {
      if (!--ref_count)
      {
        getStreamWithDate() << "Ending logging";
        logger_facade_inst_priv.reset(nullptr);
      }
    }
  }

  std::ostream & getStream() const
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
    if (std::strftime(mbstr, sizeof(mbstr), "%A %c", std::localtime(&t)))
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
    return getStreamWithDate(level) << " " << formatFile(file) << ":" << line
        << ": " << function << ": ";
  }

  std::ostream & getStreamWithDate(const std::string & file,
                                   int line,
                                   const std::string & function) const
  {
    return getStreamWithDate() << " " << formatFile(file) << ":" << line <<
        ": " << function << ": ";
  }

  static LoggerFacade & getLoggerFacade()
  {
    return *logger_facade_inst;
  }

private:
  LoggerFacade()
  {
    ref_counted = false;
  }

  class LoggerFacadeInstance
  {
  public:

    LoggerFacadeInstance(LoggerType p_type)
    {
      switch (p_type)
      {
        case LoggerType::STDOUT:
          logger.reset(new StdOutLogger());
          break;
        case LoggerType::STDERR:
          logger.reset(new StdErrLogger());
          break;
        case LoggerType::FILE:
          throw std::runtime_error("Invalid Logger Type passed");
        default:
          std::runtime_error("Invalid LoggerType");
      }
    }

    LoggerFacadeInstance(LogFileOnEntry p_file_on_entry,
                         LogFileOnExit p_file_on_exit,
                         const std::string& prefix)
    {
      logger.reset(new FileLogger(p_file_on_entry, p_file_on_exit, prefix));
    }

    std::ostream & getStream() const
    {
      return logger->getStream() << "\n";
    }
  private:
    std::unique_ptr<ILogger> logger;
  };

  static std::string formatFile(const std::string &file)
  {
    using Size = std::string::size_type;
    Size backslash_pos = file.find_last_of("/");
    if (backslash_pos != std::string::npos)
    {
      return file.substr(backslash_pos + 1);
    }
    return file;
  }

  static unsigned ref_count;
  static std::unique_ptr<LoggerFacadeInstance> logger_facade_inst_priv;
  static std::unique_ptr<LoggerFacade> logger_facade_inst;
  bool ref_counted = true;
};

}
