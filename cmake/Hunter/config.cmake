include(hunter_add_version)

hunter_config(Boost VERSION 1.66.0 CONFIGURATION_TYPES Release)

hunter_config(GTest CONFIGURATION_TYPES Release)

hunter_config(log4cplus
              CONFIGURATION_TYPES Release
              CMAKE_ARGS BUILD_SHARED_LIBS=FALSE
              LOG4CPLUS_BUILD_TESTING=OFF
              LOG4CPLUS_BUILD_LOGGINGSERVER=FALSE
              LOG4CPLUS_ENABLE_DECORATED_LIBRARY_NAME=OFF)
