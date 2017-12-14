// Copyright [2017] <Malinovsky Rodion>

#pragma once

#include <cstddef>
#include <string>

namespace rms {
namespace util {

class StaticString {
 public:
  template <std::size_t N>
  constexpr explicit StaticString(const char (&data)[N])
      : data_(data), size_(N - 1) {
    static_assert(N > 0, "Invalid string literal! Length is zero!");
  }

  constexpr StaticString(const StaticString& other,
                         const std::size_t start_pos,
                         const std::size_t number)
      : data_(other.data_ + start_pos), size_(other.Size() - number) {}

  constexpr StaticString(const StaticString& other, const std::size_t start_pos)
      : data_(other.data_ + start_pos), size_(other.Size() - start_pos) {}

  constexpr char operator[](const std::size_t n) const {
    return n < size_ ? data_[n] : '\0';
  }

  constexpr std::size_t Size() const {
    return size_;
  }

  constexpr const char* Data() const {
    return data_;
  }

  constexpr bool IsEqual(const StaticString& other) const {
    return (this == &other) ||
           ((Size() == other.Size()) &&
            (IsStringsEqual(Data(), other.Data(), Size(), 0u)));
  }

  std::string ToString() const {
    return std::string{data_, size_};
  }

 private:
  constexpr bool IsStringsEqual(const char* rhs,
                                const char* lhs,
                                const std::size_t size,
                                const std::size_t index) const {
    return (*lhs == *rhs) &&
           ((index == size - 1)
                ? true
                : IsStringsEqual(lhs + 1, rhs + 1, size, index + 1));
  }

  const char* const data_;

  const std::size_t size_;
};

}  // namespace util
}  // namespace rms
