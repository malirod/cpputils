// Copyright [2016] <Malinovsky Rodion>

#pragma once

#include <tuple>
#include <type_traits>

namespace cppecho {
namespace util {

template <typename T>
struct FunctionTraits : public FunctionTraits<decltype(&T::operator())> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct FunctionTraits<ReturnType (ClassType::*)(Args...) const> {
  typedef ReturnType resultType;

  constexpr static const std::size_t argsCount = sizeof...(Args);

  template <std::size_t N>
  struct arg {
    // The i-th argument is equivalent to the i-th tuple element of a tuple
    // composed of those arguments.
    typedef typename std::decay<
        typename std::tuple_element<N, std::tuple<Args...>>::type>::type type;
  };
};

}  // namespace util
}  // namespace cppecho
