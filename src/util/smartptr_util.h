// Copyright [2017] <Malinovsky Rodion>

/*
 * Based on
 * http://stackoverflow.com/questions/7038357/make-unique-and-perfect-forwarding
 * https://isocpp.org/blog/2013/04/n3656-make-unique-revision-1
 * Actual impl taken from https://isocpp.org/files/papers/N3656.txt
 *
 */

#pragma once

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

namespace rms {
namespace util {

template <class T>
struct _Unique_if {
  typedef std::unique_ptr<T> _Single_object;
};

template <class T>
struct _Unique_if<T[]> {
  typedef std::unique_ptr<T[]> _Unknown_bound;
};

template <class T, size_t N>
struct _Unique_if<T[N]> {
  typedef void _Known_bound;
};

template <class T, class... Args>
typename _Unique_if<T>::_Single_object make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
typename _Unique_if<T>::_Unknown_bound make_unique(size_t n) {
  typedef typename std::remove_extent<T>::type U;
  return std::unique_ptr<T>(new U[n]());
}

template <class T, class... Args>
typename _Unique_if<T>::_Known_bound make_unique(Args&&...) = delete;

}  // namespace util
}  // namespace rms
