// Copyright [2017] <Malinovsky Rodion>

#pragma once

#include <string>
#include <utility>

#include <boost/format.hpp>

namespace rms {
namespace util {

class BoostStringFmt {
 public:
  // Deleted because of clang-tidy warning: misc-forwarding-reference-overload
  BoostStringFmt(const BoostStringFmt&) = delete;
  BoostStringFmt(BoostStringFmt&&) = delete;

  template <typename T>
  explicit BoostStringFmt(T&& fmt);

  template <typename T>
  BoostStringFmt& operator<<(const T& arg);

  template <typename T>
  std::string format(T arg);

  template <typename T, typename... Args>
  std::string format(T arg, Args... args);

  explicit operator std::string() const;

 private:
  boost::format fmt_;
};

}  // namespace util
}  // namespace rms

template <typename T>
rms::util::BoostStringFmt::BoostStringFmt(T&& fmt) : fmt_(std::forward<T>(fmt)) {}

template <typename T>
rms::util::BoostStringFmt& rms::util::BoostStringFmt::operator<<(const T& arg) {
  fmt_ % arg;
  return *this;
}

template <typename T>
std::string rms::util::BoostStringFmt::format(T arg) {
  *this << arg;

  return fmt_.str();
}

template <typename T, typename... Args>
std::string rms::util::BoostStringFmt::format(T arg, Args... args) {
  *this << arg;

  return format(args...);
}

inline rms::util::BoostStringFmt::operator std::string() const {
  return fmt_.str();
}
