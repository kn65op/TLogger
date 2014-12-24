# - Try to find TLogger
#Once done this will define
# TLOGGER_FOUND - system has TLogger
# TLOGGER_INCLUDE_DIRS - TLogger include directories
# TLOGGER_LIBRARIES - TLogger library

set(TLOGGER_FOUND TRUE)
set(TLOGGER_INCLUDE_DIRS TLogger/Code/TLogger/inc)
set(TLOGGER_LIBRARIES TLogger/${CMAKE_BUILD_TYPE}/libTLogger.a)
