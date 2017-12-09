message("Processing Log4cplus")

# Add options for lib to exlude not required parts
set(BUILD_SHARED_LIBS FALSE CACHE BOOL "" FORCE)
set(LOG4CPLUS_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(LOG4CPLUS_BUILD_LOGGINGSERVER OFF CACHE BOOL "" FORCE)

add_subdirectory(log4cplus)
