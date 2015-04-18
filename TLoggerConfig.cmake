# - Config file for the TLogger package
# It defines the following variables
#  TLOGGER_INCLUDE_DIRS - include directories for TLogger
#  TLOGGER_LIBRARIES    - libraries to link against
#  TLOGGER_LIBRARIES_DIR- diretory to libraries to link against
#  TLOGGER_EXECUTABLE   - the bar executable
 
# Compute paths
get_filename_component(TLOGGER_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(TLOGGER_INCLUDE_DIRS "TLogger/Code/TLogger/inc")
 
# These are IMPORTED targets created by TLoggerTargets.cmake
set(TLOGGER_LIBRARIES_DIR "TLogger/${CMAKE_BUILD_TYPE}")
set(TLOGGER_LIBRARIES "libTLogger.a")
set(TLOGGER_TARGET "TLogger")
