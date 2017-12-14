// Copyright [2017] <Malinovsky Rodion>

#include "util/smartptr_util.h"
#include "gtest/gtest.h"

namespace {

class Foo {
 public:
  Foo(int& data1, int& data2) : data_1_(data1), data_2_(data2) {
    ++data_1_;
    ++data_2_;
  }

  ~Foo() {
    ++data_1_;
    ++data_2_;
  }

 private:
  int& data_1_;
  int& data_2_;
};

}  // namespace

TEST(TestSmartPtr, TestScope) {
  int cnt1 = 0;
  int cnt2 = 1;
  { auto scoped = rms::util::make_unique<Foo>(cnt1, cnt2); }
  ASSERT_EQ(2, cnt1);
  ASSERT_EQ(3, cnt2);
}
