#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdio>

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
      
    }
    
    ~Logger()
    {
      switch (file_on_exit)
      {
        case LogFileOnExit::REMOVE:
        {
          std::remove(filename.c_str());
          break;
        }
        case LogFileOnExit::DO_NOTHING: //Fallthrough
        default :
        {
        }
      }
    }
private:
  const std::string filename = "TLogger.log";
  std::ofstream log_file;
  
  LogFileOnEntry file_on_entry;
  LogFileOnExit file_on_exit;
};
}