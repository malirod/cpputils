cmake_minimum_required(VERSION 3.9)
project(cpputils VERSION 1.0 LANGUAGES CXX)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)

set(THIRDPARTY_PATH "thirdparty/")

include(CTest)

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
find_package(Boost 1.62 REQUIRED)

set(SRC_LIST
    "src/core/general_error.cc"
    "src/core/general_error.h"
    "src/util/enum_util.h"
    "src/util/logger.cc"
    "src/util/logger.h"
    "src/util/scope_guard.h"
    "src/util/singleton.h"
    "src/util/smartptr_util.h"
    "src/util/static_string.h"
    "src/util/string_fmt.h"
    "src/util/type_traits.h")

add_executable(testrunner
    "${SRC_LIST}"
    "test/core/general_error_test.cc"
    "test/core/testrunner.cc"
    "test/util/enum_util_test.cc"
    "test/util/logger_test.cc"
    "test/util/rvo_test.cc"
    "test/util/scope_guard_test.cc"
    "test/util/singleton_test.cc"
    "test/util/static_string_test.cc"
    "test/util/string_fmt_test.cc")

message("Processing GTest")
add_subdirectory("${THIRDPARTY_PATH}/googletest")

message("Processing Log4cplus")
# Add options for lib to exlude not required parts
set(BUILD_SHARED_LIBS FALSE CACHE BOOL "" FORCE)
set(LOG4CPLUS_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(LOG4CPLUS_BUILD_LOGGINGSERVER OFF CACHE BOOL "" FORCE)
add_subdirectory("${THIRDPARTY_PATH}/log4cplus")

target_include_directories(testrunner
    PRIVATE
    src
    test
    "${Boost_INCLUDE_DIRS}"
    "${THIRDPARTY_PATH}/log4cplus/include"
    "${PROJECT_BINARY_DIR}/${THIRDPARTY_PATH}/log4cplus/include"
    )
target_link_libraries(testrunner PRIVATE gtest gmock_main log4cplusS)

add_test(NAME all COMMAND testrunner)

#Copy default config to the output dir
configure_file(config/logger.cfg logger.cfg COPYONLY)
