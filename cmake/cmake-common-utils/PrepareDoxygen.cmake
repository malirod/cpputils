 # add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(BUILD_DOC "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOC)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(DOXYFILE_IN ${CMAKE_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYFILE ${CMAKE_BINARY_DIR}/Doxyfile)

    message(STATUS "Configuring Doxygen")
    configure_file(${DOXYFILE_IN} ${DOXYFILE} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/html DESTINATION share/doc)
endif()
