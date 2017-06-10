// Copyright [2016] <Malinovsky Rodion>

#pragma once

#include <string>
#include <utility>
#include "boost/format.hpp"

namespace cppecho {
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

  operator const char*() const;

 private:
  boost::format fmt_;
};

}  // namespace util
}  // namespace cppecho

template <typename T>
inline cppecho::util::StringFmt::StringFmt(T&& fmt)
    : fmt_(std::forward<T>(fmt)) {}

template <typename T>
cppecho::util::StringFmt& cppecho::util::StringFmt::operator<<(const T& arg) {
  fmt_ % arg;
  return *this;
}

template <typename T>
std::string cppecho::util::StringFmt::format(T arg) {
  *this << arg;

  return fmt_.str();
}

template <typename T, typename... Args>
std::string cppecho::util::StringFmt::format(T arg, Args... args) {
  *this << arg;

  return format(args...);
}

inline cppecho::util::StringFmt::operator std::string() const {
  return fmt_.str();
}

inline cppecho::util::StringFmt::operator const char*() const {
  return fmt_.str().c_str();
}
