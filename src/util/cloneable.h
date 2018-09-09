// Copyright [2018] <Malinovsky Rodion>

#pragma once

#include <memory>

namespace rms {
namespace util {

template <typename T>
auto Clone(const T& value) -> decltype(value.Clone()) {
  return value.Clone();
}

template <typename Base, typename Derived>
void Clone(Derived& lhs, const Base& rhs) {
  static_assert(std::is_base_of<Base, Derived>::value, "Derived is expected to be derived from Base");
  static_assert(std::is_copy_assignable<Derived>::value, "Derived is expected to be copy assignable");

  const Derived& srcDerived = static_cast<const Derived&>(rhs);
  lhs = srcDerived;
}

template <typename Derived, typename ResultType = std::unique_ptr<Derived>>
class IWithCloningOf {
 public:
  ResultType Clone() const {
    return ResultType(DoClone());
  }

  virtual void Clone(const Derived& other) = 0;

 protected:
  // Prevent destruction via Cloneable.
  // This way no need in virtual destructor.
  // cppcheck-suppress virtualDestructor
  ~IWithCloningOf() = default;

 private:
  virtual Derived* DoClone() const = 0;
};

template <typename Base, typename Derived, typename ResultType = std::unique_ptr<Base>>
class WithCloningOf : public Base {
 public:
  using Base::Base;

  ResultType Clone() const {
    return ResultType(DoClone());
  }

  void Clone(const Base& other) override {
    static_assert(std::is_base_of<Base, Derived>::value, "Derived is expected to be derived from Base");
    util::Clone<Base, Derived>(static_cast<Derived&>(*this), other);
  }

 protected:
  ~WithCloningOf() = default;

 private:
  Base* DoClone() const override {
    return new Derived(*static_cast<Derived const*>(this));
  }
};

}  // namespace util
}  // namespace rms
