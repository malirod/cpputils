// Copyright [2018] <Malinovsky Rodion>

#include <gtest/gtest.h>

#include "util/cloneable.h"

namespace {

class DefaultBase : public rms::util::IWithCloningOf<DefaultBase> {
 public:
  virtual ~DefaultBase() = default;

  virtual int GetData() const = 0;
  virtual void SetData(int value) = 0;

 protected:
  DefaultBase() = default;
  DefaultBase(const DefaultBase&) = default;
  DefaultBase(DefaultBase&&) = default;
  DefaultBase& operator=(const DefaultBase&) = default;
  DefaultBase& operator=(DefaultBase&&) = default;
};

class DefaultDerived : public rms::util::WithCloningOf<DefaultBase, DefaultDerived> {
 public:
  int GetData() const override {
    return data;
  }
  void SetData(int value) override {
    data = value;
  }

 private:
  int data = 0;
};

class CustomBase : public rms::util::IWithCloningOf<CustomBase> {
 public:
  virtual ~CustomBase() = default;

  virtual int GetData() const = 0;
  virtual void SetData(int value) = 0;

 protected:
  CustomBase() = default;
  CustomBase(const CustomBase&) = default;
  CustomBase(CustomBase&&) = default;
  CustomBase& operator=(const CustomBase&) = default;
  CustomBase& operator=(CustomBase&&) = default;
};

class CustomDerived : public rms::util::WithCloningOf<CustomBase, CustomDerived> {
 public:
  ~CustomDerived() override {}

  int GetData() const override {
    return data;
  }
  void SetData(int value) override {
    data = value;
  }

 private:
  int data = 0;
};

class DefaultSharedBase : public rms::util::IWithCloningOf<DefaultSharedBase, std::shared_ptr<DefaultSharedBase>> {
 public:
  virtual ~DefaultSharedBase() = default;

  virtual int GetData() const = 0;
  virtual void SetData(int value) = 0;

 protected:
  DefaultSharedBase() = default;
  DefaultSharedBase(const DefaultSharedBase&) = default;
  DefaultSharedBase(DefaultSharedBase&&) = default;
  DefaultSharedBase& operator=(const DefaultSharedBase&) = default;
  DefaultSharedBase& operator=(DefaultSharedBase&&) = default;
};

class DefaultSharedDerived
    : public rms::util::WithCloningOf<DefaultSharedBase, DefaultSharedDerived, std::shared_ptr<DefaultSharedBase>> {
 public:
  int GetData() const override {
    return data;
  }
  void SetData(int value) override {
    data = value;
  }

 private:
  int data = 0;
};

}  // namespace

static_assert(!std::is_default_constructible<::DefaultBase>::value, "::DefaultBase is not default constructible");
static_assert(!std::is_copy_constructible<::DefaultBase>::value, "::DefaultBase is not copy constructible");
static_assert(!std::is_move_constructible<::DefaultBase>::value, "::DefaultBase is not move constructible");
static_assert(!std::is_copy_assignable<::DefaultBase>::value, "::DefaultBase is not copy assignable");
static_assert(!std::is_move_assignable<::DefaultBase>::value, "::DefaultBase is not move assignable");

static_assert(std::is_default_constructible<::DefaultDerived>::value, "::DefaultDerived is not default constructible");
static_assert(std::is_copy_constructible<::DefaultDerived>::value, "::DefaultDerived is not copy constructible");
static_assert(std::is_move_constructible<::DefaultDerived>::value, "::DefaultDerived is not move constructible");
static_assert(std::is_copy_assignable<::DefaultDerived>::value, "::DefaultDerived is not copy assignable");
static_assert(std::is_move_assignable<::DefaultDerived>::value, "::DefaultDerived is not move assignable");

static_assert(!std::is_default_constructible<::CustomBase>::value, "::CustomBase is not default constructible");
static_assert(!std::is_copy_constructible<::CustomBase>::value, "::CustomBase is not copy constructible");
static_assert(!std::is_move_constructible<::CustomBase>::value, "::CustomBase is not move constructible");
static_assert(!std::is_copy_assignable<::CustomBase>::value, "::CustomBase is not copy assignable");
static_assert(!std::is_move_assignable<::CustomBase>::value, "::CustomBase is not move assignable");

static_assert(std::is_default_constructible<::CustomDerived>::value, "::CustomDerived is not default constructible");
static_assert(std::is_copy_constructible<::CustomDerived>::value, "::CustomDerived is not copy constructible");
static_assert(std::is_move_constructible<::CustomDerived>::value, "::CustomDerived is not move constructible");
static_assert(std::is_copy_assignable<::CustomDerived>::value, "::CustomDerived is not copy assignable");
static_assert(std::is_move_assignable<::CustomDerived>::value, "::CustomDerived is not move assignable");

static_assert(!std::is_default_constructible<::DefaultSharedBase>::value,
              "::DefaultSharedBase is not default constructible");
static_assert(!std::is_copy_constructible<::DefaultSharedBase>::value, "::DefaultSharedBase is not copy constructible");
static_assert(!std::is_move_constructible<::DefaultSharedBase>::value, "::DefaultSharedBase is not move constructible");
static_assert(!std::is_copy_assignable<::DefaultSharedBase>::value, "::DefaultSharedBase is not copy assignable");
static_assert(!std::is_move_assignable<::DefaultSharedBase>::value, "::DefaultSharedBase is not move assignable");

static_assert(std::is_default_constructible<::DefaultSharedDerived>::value,
              "::DefaultSharedDerived is not default constructible");
static_assert(std::is_copy_constructible<::DefaultSharedDerived>::value,
              "::DefaultSharedDerived is not copy constructible");
static_assert(std::is_move_constructible<::DefaultSharedDerived>::value,
              "::DefaultSharedDerived is not move constructible");
static_assert(std::is_copy_assignable<::DefaultSharedDerived>::value, "::DefaultSharedDerived is not copy assignable");
static_assert(std::is_move_assignable<::DefaultSharedDerived>::value, "::DefaultSharedDerived is not move assignable");

TEST(TestCloneable, TestWithDefaultDTor) {
  const int test_data1 = 10;
  const int test_data2 = 20;

  auto a = std::make_unique<::DefaultDerived>();
  a->SetData(test_data1);
  EXPECT_EQ(test_data1, a->GetData());

  auto b = rms::util::Clone(*a);
  static_assert(std::is_same<std::unique_ptr<::DefaultBase>, decltype(b)>::value,
                "clone result should return std::unique_ptr<::DefaultBase>");
  static_assert(!std::is_same<std::shared_ptr<::DefaultBase>, decltype(b)>::value,
                "clone result should not return std::shared_ptr<::DefaultBase>");
  static_assert(!std::is_same<std::unique_ptr<::DefaultDerived>, decltype(b)>::value,
                "clone result should not return std::unique_ptr<::DefaultDerived>");
  EXPECT_EQ(test_data1, b->GetData());
  b->SetData(test_data2);
  EXPECT_EQ(test_data2, b->GetData());

  auto c = std::make_unique<::DefaultDerived>();
  EXPECT_EQ(0, c->GetData());
  c->Clone(*b);
  EXPECT_EQ(test_data2, c->GetData());
}

TEST(TestCloneable, TestWithCustomDTor) {
  const int test_data1 = 10;
  const int test_data2 = 20;

  auto a = std::make_unique<::CustomDerived>();
  a->SetData(test_data1);
  EXPECT_EQ(test_data1, a->GetData());

  auto b = rms::util::Clone(*a);
  EXPECT_EQ(test_data1, b->GetData());
  b->SetData(test_data2);
  EXPECT_EQ(test_data2, b->GetData());

  auto c = std::make_unique<::CustomDerived>();
  EXPECT_EQ(0, c->GetData());
  c->Clone(*b);
  EXPECT_EQ(test_data2, c->GetData());
}

TEST(TestCloneable, TestSharedWithDefaultDTor) {
  const int test_data1 = 10;
  const int test_data2 = 20;

  auto a = std::make_shared<::DefaultSharedDerived>();
  a->SetData(test_data1);
  EXPECT_EQ(test_data1, a->GetData());

  auto b = rms::util::Clone(*a);
  static_assert(std::is_same<std::shared_ptr<::DefaultSharedBase>, decltype(b)>::value,
                "clone result should return std::shared_ptr<::DefaultSharedBase>");
  static_assert(!std::is_same<std::unique_ptr<::DefaultSharedBase>, decltype(b)>::value,
                "clone result should not return std::unique_ptr<::DefaultSharedBase>");
  static_assert(!std::is_same<std::shared_ptr<::DefaultSharedDerived>, decltype(b)>::value,
                "clone result should not return std::shared_ptr<::DefaultSharedDerived>");
  EXPECT_EQ(test_data1, b->GetData());
  b->SetData(test_data2);
  EXPECT_EQ(test_data2, b->GetData());

  auto c = std::make_shared<::DefaultSharedDerived>();
  EXPECT_EQ(0, c->GetData());
  c->Clone(*b);
  EXPECT_EQ(test_data2, c->GetData());
}
