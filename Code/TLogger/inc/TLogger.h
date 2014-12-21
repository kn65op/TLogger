#ifndef TLOGGER_H
#define TLOGGER_H

namespace TLogger
{
  
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
  Logger(LogFileOnEntry p_file_on_entry = LogFileOnEntry::OVERRIDE, LogFileOnExit p_file_on_exit = LogFileOnExit::DO_NOTHING) :
    file_on_entry(p_file_on_entry),
    file_on_exit(p_file_on_exit)
    {
      
    }
private:
  LogFileOnEntry file_on_entry;
  LogFileOnExit file_on_exit;
};
}

#endif // TLOGGER_H
