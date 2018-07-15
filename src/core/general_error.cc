// Copyright [2018] <Malinovsky Rodion>

#include "core/general_error.h"
#include "util/enum_util.h"

using rms::core::GeneralError;

template <>
rms::util::enum_util::EnumStrings<GeneralError>::DataType rms::util::enum_util::EnumStrings<GeneralError>::data = {
    "Success", "Internal error", "Wrong command line arguments", "Startup has failed"};

const std::error_category& rms::core::ErrorCategory::get() {
  static ErrorCategory instance;
  return instance;
}

std::error_condition rms::core::make_error_condition(GeneralError error) noexcept {
  using rms::util::enum_util::ToIntegral;
  return {ToIntegral(error), ErrorCategory::get()};
}

std::error_code rms::core::make_error_code(GeneralError error) noexcept {
  using rms::util::enum_util::ToIntegral;
  return {ToIntegral(error), ErrorCategory::get()};
}

const char* rms::core::ErrorCategory::name() const noexcept {
  return "GeneralError";
}

std::string rms::core::ErrorCategory::message(int error_value) const {
  using rms::util::enum_util::EnumToString;
  using rms::util::enum_util::FromIntegral;
  return EnumToString(FromIntegral<rms::core::GeneralError>(error_value));
}
