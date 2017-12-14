// Copyright [2017] <Malinovsky Rodion>

#pragma once

/*
 * Based on
 * http://codereview.stackexchange.com/questions/14309/conversion-between-enum-and-string-in-c-class-header
 */

#include <algorithm>
#include <sstream>
#include <string>
#include <type_traits>

namespace rms {
namespace util {
namespace enum_util {

template <typename E>
constexpr inline auto ToIntegral(E e) noexcept ->
    typename std::underlying_type<E>::type {
  return static_cast<typename std::underlying_type<E>::type>(e);
}

template <typename E, typename T>
constexpr inline typename std::
    enable_if<std::is_enum<E>::value && std::is_integral<T>::value, E>::type
    FromIntegral(T value) noexcept {
  return static_cast<E>(value);
}

  /*
   * Reason for warning ignore.
   * Recently introduced warning in clang (-Wundefined-var-template)
   * prevents main idea of enum util: split enum utils and filling Storage
   * in enum's cpp file via template specialization.
   */

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"

template <typename T>
struct Storage {
  template <typename... ArgTypes>
  // cppcheck-suppress noExplicitConstructor
  Storage(ArgTypes... args)  // NOLINT(runtime/explicit)
      : size(sizeof...(ArgTypes)) {
    static const char* static_data[sizeof...(ArgTypes)] = {args...};
    data = static_data;
  }

  const char** data;
  const int size;
};

// Holds all strings.
// Each enumeraition must declare its own specialization.
template <typename T>
struct EnumStrings {
  using DataType = Storage<T>;
  static DataType data;
};

template <typename T>
struct EnumRefHolder {
  T& enum_value;
  explicit EnumRefHolder(T& enum_value) : enum_value(enum_value) {}
};

template <typename T>
struct EnumConstRefHolder {
  T const& enum_value;
  explicit EnumConstRefHolder(T const& enum_value) : enum_value(enum_value) {}
};

// Actual enum to string conversion
template <typename T>
std::ostream& operator<<(std::ostream& stream,
                         const EnumConstRefHolder<T>& data) {
  const auto index = ToIntegral(data.enum_value);
  const auto data_size = EnumStrings<T>::data.size;
  if (index >= 0 && index < data_size) {
    stream << EnumStrings<T>::data.data[index];
  }
  return stream;
}

// Actual enum from string conversion
template <typename T>
std::istream& operator>>(std::istream& stream, const EnumRefHolder<T>& data) {
  std::string value;
  stream >> value;

  static auto begin = EnumStrings<T>::data.data;
  static auto end = EnumStrings<T>::data.data + EnumStrings<T>::data.size;

  auto find = std::find(begin, end, value);
  if (find != end) {
    data.enum_value = static_cast<T>(std::distance(begin, find));
  }

  return stream;
}

// This is the public interface:
// use the ability of function to deuce their template type without
// being explicitly told to create the correct type of enumRefHolder<T>
template <typename T>
EnumConstRefHolder<T> EnumToStream(const T& e) {
  return EnumConstRefHolder<T>(e);
}

template <typename T>
EnumRefHolder<T> EnumFromStream(T& e) {
  return EnumRefHolder<T>(e);
}

template <typename T>
const char* EnumToChars(const T& e) {
  const auto index = ToIntegral(e);
  const auto dataSize = EnumStrings<T>::data.size;
  if (index >= 0 && index < dataSize) {
    return EnumStrings<T>::data.data[index];
  }
  return "";
}

#pragma clang diagnostic pop

template <typename T>
std::string EnumToString(const T& e) {
  return std::string(EnumToChars<T>(e));
}

// http://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
struct EnumClassHash {
  template <typename T>
  std::size_t operator()(T t) const {
    return static_cast<std::size_t>(t);
  }
};

}  // namespace enum_util
}  // namespace util
}  // namespace rms
