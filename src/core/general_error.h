// Copyright [2017] <Malinovsky Rodion>

#pragma once

#include <string>
#include <system_error>

namespace cppecho {
namespace core {

enum class GeneralError {
  Success,
  InternalError,
  WrongCommandLine,
  StartupFailed,
};

class ErrorCategory : public std::error_category {
 public:
  const char* name() const noexcept override;

  std::string message(int error_value) const override;

  static const std::error_category& get();

 protected:
  ErrorCategory() = default;
};

std::error_condition make_error_condition(GeneralError error) noexcept;
std::error_code make_error_code(GeneralError error) noexcept;

}  // namespace core
}  // namespace cppecho

// Register for implicit conversion to error_condition
namespace std {
template <>
struct is_error_condition_enum<cppecho::core::GeneralError> : public true_type {
};
}  // namespace std
