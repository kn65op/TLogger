cmake_minimum_required (VERSION 2.8.4) 

#variables
set(SRC_DIR "Code")
#set(MAIN_SRC_DIR "${SRC_DIR}/Main")
#set(MAIN_FILE "${MAIN_SRC_DIR}/main.cpp")
set(DEBUG_BIN_DIR "Debug")
set(RELEASE_BIN_DIR "Release")
set(LIB_NAME "TLogger")

#project
project (tlogger)

#externial libs
#FIND_PACKAGE( Boost 1.54 COMPONENTS date_time REQUIRED )
##FIND_PACKAGE( Boost 1.53 COMPONENTS program_options REQUIRED )

#compiler options
set(CMAKE_CXX_FLAGS "-std=c++14")

if ("${NO_COVERAGE}" STREQUAL "true")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
else ("${NO_COVERAGE}" STREQUAL "true")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
endif ("${NO_COVERAGE}" STREQUAL "true")

set(NORMAL_WARNING_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
set(EXTRA_WARNING_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wall  -Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "${EXTRA_WARNING_FLAGS}")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

#tests
set(COVERAGE_COMMAND "gcov")

set(CTEST_MEMORYCHECK_COMMAND "valgrind")
set(CTEST_MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --quiet
--tool=memcheck --leak-check=yes --show-reachable=yes
--num-callers=100 --verbose --demangle=yes")

#includes
include_directories(${SRC_DIR})

#files
file(GLOB TLOGGER_LIB_FILES ${SRC_DIR}/TLogger/src/*.cpp)
#file(GLOB MAIN_FILES ONE_MINUTE_CHANGES_FILES ${SRC_DIR}/Main/src/*.cpp)

#executables
if ("${NO_COVERAGE}" STREQUAL "true")
else ("${NO_COVERAGE}" STREQUAL "true")
  SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
  SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )
endif ("${NO_COVERAGE}" STREQUAL "true")

#test
if (NOT "${NO_TEST}" STREQUAL "true")
  add_subdirectory(Test)
endif (NOT "${NO_TEST}" STREQUAL "true")

#add_executable(${LIB_OUTPUT_FILE} ${MAIN_FILE} ${MAIN_FILES} ${ONE_MINUTE_CHANGES_FILES})
add_library(${LIB_NAME} ${TLOGGER_LIB_FILES})

#libraries
#TARGET_LINK_LIBRARIES(${LIB_OUTPUT_FILE} ${Boost_LIBRARIES} )

#exporting for other projects
export(TARGETS ${LIB_NAME} FILE "${PROJECT_BINARY_DIR}/TLoggerTargets.cmake")
export(PACKAGE tlogger)

set(EXPORT_TARGETS TLogger CACHE INTERNAL "export targets")
set(CONFIG_INCLUDE_DIRS "${tlogger_SOURCE_DIR}/Code/TLogger/inc")
set(CONFIG_LIBRARY_DIR "${PROJECT_BINARY_DIR}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tlogger-config.cmake.in
	"${PROJECT_BINARY_DIR}/tlogger-config.cmake" @ONLY)

