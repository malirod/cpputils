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

namespace {

class IUser : public rms::util::IWithCloningOf<IUser> {
 public:
  virtual ~IUser() = default;
  virtual std::string GetName() const = 0;
  virtual void SetName(const std::string& name) = 0;
  virtual int GetId() const = 0;
  virtual void SetId(int id) = 0;
  virtual IUser* GetChild() = 0;
  virtual void SetChild(std::unique_ptr<IUser> child) = 0;
};

class MysqlUser : public rms::util::WithCloningOf<IUser, MysqlUser> {
 public:
  MysqlUser() = default;

  MysqlUser(std::string name, int id, std::unique_ptr<IUser> child)
      : name_(std::move(name)), id_(id), child_(std::move(child)) {}

  MysqlUser(std::string name, int id) : name_(std::move(name)), id_(id), child_() {}

  // Cloneable must copy constructable
  MysqlUser(const MysqlUser& rhs) : name_(rhs.name_), id_(rhs.id_) {
    if (rhs.child_) {
      // Just ensure that copy will have separate pointer
      child_ = std::make_unique<MysqlUser>(rhs.child_->GetName(), rhs.child_->GetId());
    }
  }

  MysqlUser& operator=(const MysqlUser& rhs) {
    using std::swap;
    MysqlUser tmp(rhs);
    swap(*this, tmp);
    return *this;
  }

  std::string GetName() const override {
    return name_;
  }
  void SetName(const std::string& name) override {
    name_ = name;
  }
  int GetId() const override {
    return id_;
  }
  void SetId(const int id) override {
    id_ = id;
  }
  IUser* GetChild() override {
    return child_.get();
  }
  void SetChild(std::unique_ptr<IUser> child) override {
    child_ = std::move(child);
  }

  // Implementation specific methods. Do some job.
  bool DoAuth(const std::string& key) {
    return !key.empty();
  }

 private:
  std::string name_;
  int id_ = 0;
  std::unique_ptr<IUser> child_;
};

std::unique_ptr<IUser> CreateUser(const std::string& name, const int id) {
  return std::make_unique<MysqlUser>(name, id, std::make_unique<MysqlUser>("child_" + name, id * 100));
}

std::unique_ptr<IUser> CreateUser(int choice) {
  static auto default_user_proto = CreateUser("default", 0);
  static auto user_1_proto = CreateUser("user_1", 1);
  static auto user_2_proto = CreateUser("user_2", 2);
  static auto user_3_proto = CreateUser("user_3", 3);

  switch (choice) {
    case 1:
      return rms::util::Clone(*user_1_proto);
    case 2:
      return rms::util::Clone(*user_2_proto);
    case 3:
      return rms::util::Clone(*user_3_proto);
    default:
      return rms::util::Clone(*default_user_proto);
  }
}

}  // namespace

TEST(TestCloneable, TestCloneUser) {
  // Create unique ptr to interface. No knowledge about concrete class.
  auto user1 = CreateUser("user_1", 1);
  static_assert(std::is_same<std::unique_ptr<IUser>, decltype(user1)>::value,
                "CreateUser result should return std::unique_ptr<IBaseUser>");
  EXPECT_EQ("user_1", user1->GetName());
  EXPECT_EQ(1, user1->GetId());
  EXPECT_EQ("child_user_1", user1->GetChild()->GetName());
  EXPECT_EQ(100, user1->GetChild()->GetId());

  // Clone concrete class from interface
  auto user2 = rms::util::Clone(*user1);
  static_assert(std::is_same<std::unique_ptr<IUser>, decltype(user2)>::value,
                "CreateUser result should return std::unique_ptr<IBaseUser>");

  // Change original values
  user1->SetName("new_user1");
  user1->SetId(2);
  user1->GetChild()->SetName("new_child");
  user1->GetChild()->SetId(22);

  // Ensure cloned object is not affected
  EXPECT_EQ("user_1", user2->GetName());
  EXPECT_EQ(1, user2->GetId());
  EXPECT_EQ("child_user_1", user2->GetChild()->GetName());
  EXPECT_EQ(100, user2->GetChild()->GetId());
}

TEST(TestCloneable, TestCloneUserPrototypePattern) {
  for (int i = 1; i <= 3; ++i) {
    auto user = CreateUser(i);
    static_assert(std::is_same<std::unique_ptr<IUser>, decltype(user)>::value,
                  "CreateUser result should return std::unique_ptr<IBaseUser>");
    EXPECT_EQ("user_" + std::to_string(i), user->GetName());
    EXPECT_EQ(i, user->GetId());
    EXPECT_EQ("child_user_" + std::to_string(i), user->GetChild()->GetName());
    EXPECT_EQ(i * 100, user->GetChild()->GetId());
  }
}
