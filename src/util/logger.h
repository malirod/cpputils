// Copyright [2017] <Malinovsky Rodion>

#pragma once

#define DOWHILE_NOTHING() \
  do {                    \
  } while (0)

#if defined(DISABLE_LOGGER)

#define INIT_LOGGER(log_config) DOWHILE_NOTHING()
#define SHUTDOWN_LOGGER() DOWHILE_NOTHING()

#define LOG_TRACEL(logger, message) DOWHILE_NOTHING()
#define LOG_DEBUGL(logger, message) DOWHILE_NOTHING()
#define LOG_INFOL(logger, message) DOWHILE_NOTHING()
#define LOG_WARNL(logger, message) DOWHILE_NOTHING()
#define LOG_ERRORL(logger, message) DOWHILE_NOTHING()
#define LOG_FATALL(logger, message) DOWHILE_NOTHING()

#define IMPLEMENT_STATIC_LOGGER(logger_name) DOWHILE_NOTHING()
#define DECLARE_GET_LOGGER(logger_name)
#define DECLARE_GLOBAL_GET_LOGGER(logger_name)

#define LOG_TRACE(message) DOWHILE_NOTHING()
#define LOG_DEBUG(message) DOWHILE_NOTHING()
#define LOG_INFO(message) DOWHILE_NOTHING()
#define LOG_WARN(message) DOWHILE_NOTHING()
#define LOG_ERROR(message) DOWHILE_NOTHING()
#define LOG_FATAL(message) DOWHILE_NOTHING()

#define LOG_AUTO_TRACEL(logger, message) DOWHILE_NOTHING()
#define LOG_AUTO_TRACE() DOWHILE_NOTHING()

#define LOG_TRACEF(text, ...) DOWHILE_NOTHING()
#define LOG_DEBUGF(text, ...) DOWHILE_NOTHING()
#define LOG_INFOF(text, ...) DOWHILE_NOTHING()
#define LOG_WARNF(text, ...) DOWHILE_NOTHING()
#define LOG_ERRORF(text, ...) DOWHILE_NOTHING()
#define LOG_FATALF(text, ...) DOWHILE_NOTHING()

#else  // DISABLE_LOGGER

// Strip off log lines lower or qeual DEBUG in DEBUG mode
#if defined(CUT_OFF_DEBUG_LOG)
#define LOG4CPLUS_DISABLE_DEBUG
#endif  // CUT_OFF_DEBUG_LOG

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include <iosfwd>

#include "util/string_fmt.h"  // IWYU pragma: export

#define IMPL_LOGGER_CLASS_TYPE_ log4cplus::Logger
#define IMPL_LOGGER_NAMESPACE_ rms::util::logging

namespace rms {
namespace util {
namespace logging {

class LogManager {
 public:
  explicit LogManager(const char* config_file_path);

  explicit LogManager(std::istream& log_config);

  ~LogManager();

  static void Shutdown();

  LogManager(const LogManager&) = delete;
  LogManager(const LogManager&&) = delete;
  LogManager& operator=(const LogManager&) = delete;
  LogManager& operator=(const LogManager&&) = delete;
};

}  // namespace logging
}  // namespace util
}  // namespace rms

#define INIT_LOGGER(log_config) IMPL_LOGGER_NAMESPACE_::LogManager log_manager__(log_config)
#define SHUTDOWN_LOGGER() IMPL_LOGGER_NAMESPACE_::LogManager::Shutdown();

#define LOG_TRACEL(logger, message) LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT(message))
#define LOG_DEBUGL(logger, message) LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT(message))
#define LOG_INFOL(logger, message) LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT(message))
#define LOG_WARNL(logger, message) LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT(message))
#define LOG_ERRORL(logger, message) LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(message))
#define LOG_FATALL(logger, message) LOG4CPLUS_FATAL(logger, LOG4CPLUS_TEXT(message))

#define IMPLEMENT_STATIC_LOGGER(logger_name) \
  static auto logger = IMPL_LOGGER_CLASS_TYPE_::getInstance(LOG4CPLUS_TEXT(logger_name))

#define DECLARE_GET_LOGGER(logger_name)         \
  static IMPL_LOGGER_CLASS_TYPE_& GetLogger() { \
    IMPLEMENT_STATIC_LOGGER(logger_name);       \
    return logger;                              \
  }

#define DECLARE_GLOBAL_GET_LOGGER(logger_name) \
  namespace {                                  \
  DECLARE_GET_LOGGER(logger_name)              \
  }  // namespace

#define LOG_TRACE(message) LOG_TRACEL(GetLogger(), message)
#define LOG_DEBUG(message) LOG_DEBUGL(GetLogger(), message)
#define LOG_INFO(message) LOG_INFOL(GetLogger(), message)
#define LOG_WARN(message) LOG_WARNL(GetLogger(), message)
#define LOG_ERROR(message) LOG_ERRORL(GetLogger(), message)
#define LOG_FATAL(message) LOG_FATALL(GetLogger(), message)

#define LOG_AUTO_TRACEL(logger, message) LOG4CPLUS_TRACE_METHOD(logger, message)
#define LOG_AUTO_TRACE() LOG_AUTO_TRACEL(GetLogger(), LOG4CPLUS_TEXT(__func__))

#define LOG_TRACEF(text, ...) LOG_TRACEL(GetLogger(), rms::util::StringFmt(text).format(__VA_ARGS__))
#define LOG_DEBUGF(text, ...) LOG_DEBUGL(GetLogger(), rms::util::StringFmt(text).format(__VA_ARGS__))
#define LOG_INFOF(text, ...) LOG_INFOL(GetLogger(), rms::util::StringFmt(text).format(__VA_ARGS__))
#define LOG_WARNF(text, ...) LOG_WARNL(GetLogger(), rms::util::StringFmt(text).format(__VA_ARGS__))
#define LOG_ERRORF(text, ...) LOG_ERRORL(GetLogger(), rms::util::StringFmt(text).format(__VA_ARGS__))
#define LOG_FATALF(text, ...) LOG_FATALL(GetLogger(), rms::util::StringFmt(text).format(__VA_ARGS__))

#endif
