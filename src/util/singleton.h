// Copyright [2017] <Malinovsky Rodion>

#pragma once

#include <cassert>

namespace rms {
namespace util {

template <typename T, typename Tag = T>
class SingleAccessor {
 public:
  void Attach(T& single);

  void Detach();

  bool GetIsAttached() const;

  operator T&();  // NOLINT

  T& GetRef();

 private:
  T* ptr_single_ = nullptr;
};

template <typename T, typename Tag = T>
T& single() {
  static T singleton;
  return singleton;
}

template <typename T, typename Tag>
void SingleAccessor<T, Tag>::Attach(T& single) {
  ptr_single_ = &single;
}

template <typename T, typename Tag>
void SingleAccessor<T, Tag>::Detach() {
  ptr_single_ = nullptr;
}

template <typename T, typename Tag>
SingleAccessor<T, Tag>::operator T&() {
  return GetRef();
}

template <typename T, typename Tag>
T& SingleAccessor<T, Tag>::GetRef() {
  assert(ptr_single_ != nullptr && "Single is not attached");
  return *ptr_single_;
}

template <typename T, typename Tag>
bool SingleAccessor<T, Tag>::GetIsAttached() const {
  return ptr_single_ != nullptr;
}

}  // namespace util
}  // namespace rms
