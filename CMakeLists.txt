cmake_minimum_required(VERSION 3.5)

# Prefer Clang compiler
if (NOT CMAKE_CXX_COMPILER AND NOT CMAKE_C_COMPILER AND NOT DEFINED ENV{CXX} AND NOT DEFINED ENV{CC} )
    message(STATUS "Compiler is not set explicitly. Using clang++")
    set(CMAKE_CXX_COMPILER "clang++")
    set(CMAKE_C_COMPILER "clang")
endif()

# Configure CCache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message(STATUS "Ccache found. Will be use used.")
    set(CMAKE_C_COMPILER_LAUNCHER ccache)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif(CCACHE_FOUND)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Use Gold linker if available
if (UNIX AND NOT APPLE)
  execute_process(COMMAND ${CMAKE_CXX_COMPILER}
                  -fuse-ld=gold -Wl,--version
                  ERROR_QUIET OUTPUT_VARIABLE ld_version)
  if ("${ld_version}" MATCHES "GNU gold")
    message(STATUS "Found Gold linker, use faster linker")
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
    set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold")
  endif()
endif()

project(cpputils VERSION 1.0 LANGUAGES CXX)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)

# Manage dependencies

# Setup Conan
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        message(STATUS "Setup dependencies with conan (clang)")
        execute_process(COMMAND conan install ${CMAKE_CURRENT_LIST_DIR} --profile ${CMAKE_CURRENT_LIST_DIR}/tools/conan/profile-clang)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        message(STATUS "Setup dependencies with conan (gcc)")
        execute_process(COMMAND conan install ${CMAKE_CURRENT_LIST_DIR} --profile ${CMAKE_CURRENT_LIST_DIR}/tools/conan/profile-gcc)
    else()
        set(CONAN_INSTALL_CMD "conan install .. <list-of-settings>")
        message(FATAL_ERROR "Dependencies are not configured. Install dependencies first, e.g. running in build dir: ${CONAN_INSTALL_CMD}")
    endif()
endif()

# This will set output dir to bin
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# Add possibility to sinitize code
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/sanitizers-cmake/cmake/")
find_package(Sanitizers REQUIRED)

# Add possibility to get code coverage
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/CMake-codecov/cmake")
find_package(codecov REQUIRED)

include(CTest)

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
    "test/util/smartptr_util_test.cc"
    "test/util/static_string_test.cc"
    "test/util/string_fmt_test.cc")

add_sanitizers(testrunner)

add_coverage(testrunner)

# Setup clang-tidy target
find_program(CLANG_TIDY_EXE NAMES "clang-tidy-5.0" PATHS /usr/bin)
find_program(RUN_CLANG_TIDY_EXE NAMES "run-clang-tidy.py" PATHS /usr/lib/llvm-5.0/share/clang)

if (NOT CLANG_TIDY_EXE)
    message(WARNING "clang-tidy not found!")
endif()

if (NOT RUN_CLANG_TIDY_EXE)
    message(WARNING "run-clang-tidy.py not found!")
endif()

if (CLANG_TIDY_EXE AND RUN_CLANG_TIDY_EXE)
    message(STATUS "Found clang-tidy and run-clang-tidy.py")
    set(CLANG_TIDY_HEADER_FILTER "'\/src\/|\/test\/'")

    add_custom_target(clang-tidy
        COMMAND ${RUN_CLANG_TIDY_EXE} -quiet -header-filter=${CLANG_TIDY_HEADER_FILTER}
        VERBATIM)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setup include-what-you-use
find_program(IWYU_EXE NAMES "include-what-you-use" PATHS /usr/local/bin)
find_program(IWYU_SCRIPT NAMES "iwyu_tool.py" PATHS /usr/local/bin)
if(NOT IWYU_EXE OR NOT IWYU_SCRIPT)
    message(WARNING "Could not find the program(s) include-what-you-use or iwyu_tool.py")
else()
    message(STATUS "Found include-what-you-use: ${IWYU_EXE}")
    message(STATUS "Found iwyu_tool.py: ${IWYU_SCRIPT}")
    add_custom_target(iwyu
        COMMAND ${CMAKE_CURRENT_LIST_DIR}/tools/iwyu/run_iwyu.sh
        VERBATIM)
endif()

# Setup clang static analyzer
add_custom_target(clang-static-analyzer
    COMMAND scan-build --use-analyzer=/usr/bin/clang++-5.0 make
    VERBATIM)

target_include_directories(testrunner
    PRIVATE
    src
    test)

target_compile_options(testrunner PRIVATE -Wextra -Werror)

target_link_libraries(testrunner PRIVATE CONAN_PKG::gtest CONAN_PKG::log4cplus CONAN_PKG::Boost)

add_test(NAME all COMMAND testrunner)

#Copy default config to the output dir
configure_file(config/logger.cfg bin/logger.cfg COPYONLY)
