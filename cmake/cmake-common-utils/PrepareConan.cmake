# Setup Conan
if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    file(REMOVE "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
endif()

# Get current version of compiler from toolchain
string(REPLACE "." ";" VERSION_LIST ${CMAKE_CXX_COMPILER_VERSION})
list(GET VERSION_LIST 0 COMPILER_VERSION_MAJOR)
list(GET VERSION_LIST 1 COMPILER_VERSION_MINOR)
list(GET VERSION_LIST 2 COMPILER_VERSION_PATCH)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Setup dependencies with conan (clang ${COMPILER_VERSION_MAJOR}.${COMPILER_VERSION_MINOR})")
    set(CONAN_COMPILER "clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(STATUS "Setup dependencies with conan (gcc ${COMPILER_VERSION_MAJOR}.${COMPILER_VERSION_MINOR})")
    set(CONAN_COMPILER "gcc")
else()
    # break cmake execution
    set(CONAN_INSTALL_CMD "conan install .. <list-of-settings>")
    message(FATAL_ERROR "Dependencies are not configured. Install dependencies first, e.g. running in build dir: ${CONAN_INSTALL_CMD}")
endif()
message(STATUS "Using the following toolchain for conan: CXX=${CMAKE_CXX_COMPILER}, CC=${CMAKE_C_COMPILER}")
execute_process(COMMAND conan install ${CMAKE_CURRENT_SOURCE_DIR} -if ${CMAKE_BINARY_DIR} -s arch=x86_64 -s os=Linux -s compiler=${CONAN_COMPILER} -s compiler.version=${COMPILER_VERSION_MAJOR}.${COMPILER_VERSION_MINOR} -s compiler.libcxx=libstdc++11 -s build_type=Release -e CXX=${CMAKE_CXX_COMPILER} -e CC=${CMAKE_C_COMPILER} --build missing RESULT_VARIABLE RET_CODE)
if(NOT "${RET_CODE}" STREQUAL "0")
    # break cmake execution
    message(FATAL_ERROR "Failed to setup dependencies with conan: ${RET_CODE}")
endif()
