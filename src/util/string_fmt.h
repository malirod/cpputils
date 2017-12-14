// Copyright [2017] <Malinovsky Rodion>

#pragma once

#include <string>
#include <utility>
#include "boost/format.hpp"

namespace rms {
namespace util {

class StringFmt {
 public:
  template <typename T>
  explicit StringFmt(T&& fmt);

  template <typename T>
  StringFmt& operator<<(const T& arg);

  template <typename T>
  std::string format(T arg);

  template <typename T, typename... Args>
  std::string format(T arg, Args... args);

  operator std::string() const;

 private:
  boost::format fmt_;
};

}  // namespace util
}  // namespace rms

template <typename T>
inline rms::util::StringFmt::StringFmt(T&& fmt) : fmt_(std::forward<T>(fmt)) {}

template <typename T>
rms::util::StringFmt& rms::util::StringFmt::operator<<(const T& arg) {
  fmt_ % arg;
  return *this;
}

template <typename T>
std::string rms::util::StringFmt::format(T arg) {
  *this << arg;

  return fmt_.str();
}

template <typename T, typename... Args>
std::string rms::util::StringFmt::format(T arg, Args... args) {
  *this << arg;

  return format(args...);
}

inline rms::util::StringFmt::operator std::string() const {
  return fmt_.str();
}
