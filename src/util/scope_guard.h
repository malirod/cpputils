// Copyright [2016] <Malinovsky Rodion>

#pragma once

#include <memory>
#include <utility>

namespace cppecho {
namespace util {

template <typename CleanupAction>
class ScopeGuard {
 public:
  explicit ScopeGuard(CleanupAction&& cleanup_action)
      : cleanup_action_(std::move(cleanup_action)) {}

  ~ScopeGuard() {
    cleanup_action_();
  }

 private:
  CleanupAction cleanup_action_;
};

template <typename CleanupAction>
ScopeGuard<CleanupAction> MakeScopeGuard(CleanupAction cleanup_action) {
  return ScopeGuard<CleanupAction>(std::move(cleanup_action));
}

}  // namespace util
}  // namespace cppecho
