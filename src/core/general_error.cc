// Copyright [2017] <Malinovsky Rodion>

#include "core/general_error.h"
#include "util/enum_util.h"

using cppecho::core::GeneralError;

template <>
cppecho::util::enum_util::EnumStrings<GeneralError>::DataType
    cppecho::util::enum_util::EnumStrings<GeneralError>::data = {
        "Success",
        "Internal error",
        "Wrong command line arguments",
        "Startup has failed"};

const std::error_category& cppecho::core::ErrorCategory::get() {
  static ErrorCategory instance;
  return instance;
}

std::error_condition cppecho::core::make_error_condition(
    GeneralError error) noexcept {
  using cppecho::util::enum_util::ToIntegral;
  return std::error_condition(ToIntegral(error), ErrorCategory::get());
}

std::error_code cppecho::core::make_error_code(GeneralError error) noexcept {
  using cppecho::util::enum_util::ToIntegral;
  return std::error_code(ToIntegral(error), ErrorCategory::get());
}

const char* cppecho::core::ErrorCategory::name() const noexcept {
  return "GenenalError";
}

std::string cppecho::core::ErrorCategory::message(int error_value) const {
  using cppecho::util::enum_util::EnumToString;
  using cppecho::util::enum_util::FromIntegral;
  return EnumToString(FromIntegral<cppecho::core::GeneralError>(error_value));
}
