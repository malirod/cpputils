// Copyright [2017] <Malinovsky Rodion>

#include "util/scope_guard.h"
#include <gtest/gtest.h>

TEST(TestScopeGuard, BasicUsage) {
  int hit_count = 0;
  {
    auto scope_guard = rms::util::MakeScopeGuard([&hit_count]() { ++hit_count; });
    EXPECT_EQ(0, hit_count);
  }
  EXPECT_EQ(1, hit_count);
}
