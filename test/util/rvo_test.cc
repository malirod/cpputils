// Copyright [2017] <Malinovsky Rodion>

#include <gtest/gtest.h>
#include <algorithm>
#include <cstddef>
#include <ctime>
#include <memory>
#include <vector>
#include "util/smartptr_util.h"

namespace {

class Foo {
 public:
  explicit Foo(int data) : data_(data) {
    ++ctor_cnt;
  }

  Foo() {
    ++ctor_cnt;
  }

  Foo(const Foo& /*unused*/) {
    ++copy_ctor_cnt;
  }

  ~Foo() {
    ++dtor_cnt;
  }

  Foo(Foo&& rhs) noexcept : data_(rhs.data_) {
    ++move_ctor_cnt;
  }

  Foo& operator=(Foo&& rhs) noexcept {
    data_ = rhs.data_;
    ++move_assign_cnt;
    return *this;
  }

  void SetData(int value) {
    data_ = value;
  }

  int GetData() const {
    return data_;
  }

  static int ctor_cnt;
  static int copy_ctor_cnt;
  static int move_ctor_cnt;
  static int move_assign_cnt;
  static int dtor_cnt;

 private:
  int data_ = 0;
};

using UniqueFooType = std::vector<std::unique_ptr<Foo>>;
using SharedFooType = std::vector<std::shared_ptr<Foo>>;

class Bar {
 public:
  static Foo CreateFoo(int data) {
    Foo result;
    result.SetData(data);
    return result;
  }

  UniqueFooType CreateFooVector(int data, std::size_t size) {
    auto result = UniqueFooType();
    result.reserve(size);
    for (std::size_t i = 0u; i < size; ++i) {
      auto item = rms::util::make_unique<Foo>();
      item->SetData(data);
      result.emplace_back(std::move(item));
    }
    return result;
  }

  SharedFooType CreateFooSharedVector(int data, std::size_t size) {
    auto result = SharedFooType();
    result.reserve(size);
    for (std::size_t i = 0u; i < size; ++i) {
      auto item = std::make_shared<Foo>();
      item->SetData(data);
      result.emplace_back(std::move(item));
    }
    return result;
  }
};

class TestRVO : public ::testing::Test {
 public:
  TestRVO() {
    Foo::ctor_cnt = 0;
    Foo::copy_ctor_cnt = 0;
    Foo::move_ctor_cnt = 0;
    Foo::move_assign_cnt = 0;
    Foo::dtor_cnt = 0;
  }

  void ExpectZeroCounters() {
    EXPECT_EQ(0, Foo::copy_ctor_cnt);
    EXPECT_EQ(0, Foo::move_ctor_cnt);
    EXPECT_EQ(0, Foo::move_assign_cnt);
    EXPECT_EQ(0, Foo::dtor_cnt);
  }
};

int Foo::ctor_cnt = 0;
int Foo::copy_ctor_cnt = 0;
int Foo::move_ctor_cnt = 0;
int Foo::move_assign_cnt = 0;
int Foo::dtor_cnt = 0;

}  // namespace

TEST_F(TestRVO, RVOWithOneReturn) {
  auto result = []() { return Bar::CreateFoo(100); }();
  EXPECT_EQ(100, result.GetData());
  EXPECT_EQ(1, Foo::ctor_cnt);
  ExpectZeroCounters();
}

TEST_F(TestRVO, RVOWithManyReturnsFails) {
  auto result = []() {
    auto value = int{100};
    // get some random value to prevent optimization
    const Foo some_object{static_cast<int>(time(nullptr))};
    if (some_object.GetData() == 0) {
      value += 1;
      return Foo(value);
    } else if (some_object.GetData() > 0) {  // NOLINT
      value += 3;
      return Foo(value);
    }
    return Bar::CreateFoo(value);
  }();
  EXPECT_GE(result.GetData(), 100);
  EXPECT_EQ(2, Foo::ctor_cnt);
  EXPECT_EQ(0, Foo::copy_ctor_cnt);
  EXPECT_EQ(0, Foo::move_ctor_cnt);
  EXPECT_EQ(0, Foo::move_assign_cnt);
  EXPECT_EQ(1, Foo::dtor_cnt);
}

TEST_F(TestRVO, RVOWithUniquePtr) {
  const std::size_t Size = 50u;
  const int Data = 200;
  auto result = [&]() { return Bar().CreateFooVector(Data, Size); }();
  EXPECT_EQ(Size, result.size());
  EXPECT_EQ(Size, static_cast<std::size_t>(Foo::ctor_cnt));
  ExpectZeroCounters();
  for (const auto& item : result) {
    EXPECT_EQ(Size, result.size());
    EXPECT_EQ(Data, item->GetData());
  }
}

TEST_F(TestRVO, RVOWithSharedPtr) {
  const std::size_t Size = 50u;
  const int Data = 200;
  auto result = [&]() { return Bar().CreateFooSharedVector(Data, Size); }();
  EXPECT_EQ(Size, result.size());
  EXPECT_EQ(Size, static_cast<std::size_t>(Foo::ctor_cnt));
  ExpectZeroCounters();
  for (const auto& item : result) {
    EXPECT_EQ(Size, result.size());
    EXPECT_EQ(Data, item->GetData());
  }
}

TEST_F(TestRVO, NRVOWithOneReturn) {
  auto bar = Bar();
  auto foo = Bar::CreateFoo(100);
  EXPECT_EQ(100, foo.GetData());
  EXPECT_EQ(1, Foo::ctor_cnt);
  ExpectZeroCounters();
}

TEST_F(TestRVO, MoveInsteadOfNRVO) {
  auto result = []() {
    auto bar = Bar();
    auto value = int{100};
    // get some random value to prevent optimization
    const Foo some_object{static_cast<int>(time(nullptr))};
    if (some_object.GetData() == 0) {
      value += 1;
      auto other_object = Foo(value);
      return other_object;
    }
    auto result = Bar::CreateFoo(value);
    return result;
  }();
  EXPECT_GE(result.GetData(), 100);
  EXPECT_EQ(2, Foo::ctor_cnt);
  EXPECT_EQ(0, Foo::copy_ctor_cnt);
  EXPECT_EQ(1, Foo::move_ctor_cnt);
  EXPECT_EQ(0, Foo::move_assign_cnt);
  EXPECT_EQ(2, Foo::dtor_cnt);
}
