// Copyright [2017] <Malinovsky Rodion>

#include "util/logger.h"
#ifndef DISABLE_LOGGER

#include <fstream>
#include <vector>

#include "log4cplus/configurator.h"

namespace {

void InitLogging(std::istream& log_config) {
  log4cplus::initialize();

  log4cplus::PropertyConfigurator configurator(log_config);
  configurator.configure();
}

}  // namespace

IMPL_LOGGER_NAMESPACE_::LogManager::LogManager(const char* config_file_path) {
  std::ifstream log_config(config_file_path);
  if (!log_config.is_open()) {
    return;
  }
  InitLogging(log_config);
}

IMPL_LOGGER_NAMESPACE_::LogManager::LogManager(std::istream& log_config) {
  InitLogging(log_config);
}

IMPL_LOGGER_NAMESPACE_::LogManager::~LogManager() {
  Shutdown();
}

void IMPL_LOGGER_NAMESPACE_::LogManager::Shutdown() {
  log4cplus::Logger::shutdown();
}

#endif
