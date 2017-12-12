cmake_minimum_required(VERSION 3.9)

# Prefer Clang compiler
if (NOT CMAKE_CXX_COMPILER AND NOT CMAKE_C_COMPILER AND NOT DEFINED ENV{CXX} AND NOT DEFINED ENV{CC} )
    message("Compiler is not set explicitly. Using clang++")
    set(CMAKE_CXX_COMPILER "clang++")
    set(CMAKE_C_COMPILER "clang")
endif()

# Configure CCache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message("Ccache found. Will be use used.")
    set(CMAKE_C_COMPILER_LAUNCHER ccache)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif(CCACHE_FOUND)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message("Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

project(cpputils VERSION 1.0 LANGUAGES CXX)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)

# Add possibility to sinitize code
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/thirdparty/sanitizers-cmake/cmake/")
find_package(Sanitizers REQUIRED)


# Add possibility to get code coverage
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/thirdparty/CMake-codecov/cmake")
message("1.1 LCOV_REMOVE_PATTERNS: " ${LCOV_REMOVE_PATTERNS})
find_package(codecov REQUIRED)
list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/thirdparty/*'")

include(CTest)

set(Boost_USE_STATIC_LIBS ON) # only find static libs
set(Boost_USE_MULTITHREADED ON)
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

add_sanitizers(testrunner)

add_coverage(testrunner)

add_subdirectory(thirdparty)

target_include_directories(testrunner
    PRIVATE
    src
    test
    "${Boost_INCLUDE_DIRS}")

target_link_libraries(testrunner PRIVATE gtest gmock_main log4cplusS)

add_test(NAME all COMMAND testrunner)

#Copy default config to the output dir
configure_file(config/logger.cfg logger.cfg COPYONLY)
