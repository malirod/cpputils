# Setup clang-tidy target
find_program(CLANG_TIDY_EXE NAMES "clang-tidy-7" PATHS /usr/bin)
find_program(RUN_CLANG_TIDY_EXE NAMES "run-clang-tidy.py" PATHS /usr/lib/llvm-7/share/clang)

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
