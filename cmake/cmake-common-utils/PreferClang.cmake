# Prefer Clang compiler
if (NOT CMAKE_CXX_COMPILER AND NOT CMAKE_C_COMPILER AND NOT DEFINED ENV{CXX} AND NOT DEFINED ENV{CC} )
    message(STATUS "Compiler is not set explicitly. Using clang++")
    set(CMAKE_CXX_COMPILER "clang++")
    set(CMAKE_C_COMPILER "clang")
endif()
