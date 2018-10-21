cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake-common-utils")
include(PreferClang)
include(PreferCcache)
include(DefaultDebug)
include(PreferGoldLinker)

project(cpputils VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Manage dependencies

include(PrepareConan)

# This will set output dir to bin
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")
include(conanbuildinfo)
conan_basic_setup(TARGETS)

include(CTest)

# Add possibility to sanitize code
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/sanitizers-cmake/cmake/")
find_package(Sanitizers REQUIRED)

# Add possibility to get code coverage
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMake-codecov/cmake")
find_package(codecov REQUIRED)

set(SRC_LIST
    "src/core/general_error.cc"
    "src/core/general_error.h"
    "src/util/enum_util.h"
    "src/util/logger.cc"
    "src/util/logger.h"
    "src/util/scope_guard.h"
    "src/util/singleton.h"
    "src/util/static_string.h"
    "src/util/boost_string_fmt.h"
    "src/util/type_traits.h"
    "src/util/cloneable.h")

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
    "test/util/fmt_test.cc"
    "test/util/boost_string_fmt_test.cc"
    "test/util/cloneable_test.cc"
    "test/misc/inter_layer_test")

add_sanitizers(testrunner)

add_coverage(testrunner)

# Setup clang-tidy target
include(ClangTidy)
# Setup doxygen target
include(PrepareDoxygen)

# Setup clang static analyzer
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Setup clang static analyzer: ${CMAKE_CXX_COMPILER}")
    add_custom_target(clang-static-analyzer
    COMMAND scan-build --use-analyzer=/usr/bin/clang++-7 make
        VERBATIM)
else()
    message(STATUS "Static analysed target not created. Current CXX is not clang")
endif()

target_include_directories(testrunner
    PRIVATE
    src
    test)

target_compile_options(testrunner PRIVATE -Wextra -Werror)

target_link_libraries(testrunner PRIVATE CONAN_PKG::gtest CONAN_PKG::log4cplus CONAN_PKG::boost CONAN_PKG::fmt)

add_test(NAME all COMMAND testrunner)

#Copy default config to the output dir
configure_file(config/logger.cfg bin/logger.cfg COPYONLY)
