// Copyright [2018] <Malinovsky Rodion>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "util/cloneable.h"

namespace {

class IUser : public rms::util::IWithCloningOf<IUser> {
 public:
  virtual ~IUser() = default;
  virtual std::string GetName() const = 0;
  virtual void SetName(std::string name) = 0;
  virtual int GetId() const = 0;
  virtual void SetId(int id) = 0;
};

class MysqlUser : public rms::util::WithCloningOf<IUser, MysqlUser> {
 public:
  MysqlUser() = default;

  MysqlUser(std::string name, int id) : name_(std::move(name)), id_(id) {}

  MysqlUser(const MysqlUser& rhs) : name_(rhs.name_), id_(rhs.id_) {}

  MysqlUser& operator=(const MysqlUser& rhs) {
    using std::swap;
    MysqlUser tmp(rhs);
    swap(*this, tmp);
    return *this;
  }

  std::string GetName() const override {
    return name_;
  }

  void SetName(std::string name) override {
    name_ = std::move(name);
  }

  int GetId() const override {
    return id_;
  }

  void SetId(const int id) override {
    id_ = id;
  }

  bool DoAuth(const std::string& key) {
    if (key.empty()) {
      return false;
    }
    return key.find("secret") != std::string::npos;
  }

 private:
  std::string name_;
  int id_ = 0;
};

using UserPtr = std::unique_ptr<IUser>;

class IDBManager {
 public:
  virtual ~IDBManager() = default;

  virtual std::vector<UserPtr> GetAuthUsers() = 0;

  virtual bool AuthUsers(std::vector<UserPtr>& users, const std::string& key) = 0;
};

class DBManager : public IDBManager {
 public:
  std::vector<UserPtr> GetAuthUsers() override {
    std::vector<UserPtr> result;
    result.reserve(3);
    result.emplace_back(std::make_unique<MysqlUser>("User_1", 1));
    result.emplace_back(std::make_unique<MysqlUser>("User_2", 2));
    result.emplace_back(std::make_unique<MysqlUser>("User_3", 3));
    return result;
  }

  bool AuthUsers(std::vector<UserPtr>& users, const std::string& key) override {
    for (auto& user : users) {
      // Use hack instead of double dispatch to save place
      assert(dynamic_cast<MysqlUser*>(user.get()));
      auto& mysql_user = static_cast<MysqlUser&>(*user);  // NOLINT
      auto authenticated = mysql_user.DoAuth(key);
      if (!authenticated) {
        return false;
      }
    }
    return true;
  }
};

class MockIDBManager : public IDBManager {
 public:
  MOCK_METHOD0(GetAuthUsers, std::vector<UserPtr>());

  MOCK_METHOD2(AuthUsers, bool(std::vector<UserPtr>&, const std::string&));
};

class BusinessUserManager {
 public:
  static bool CreateNewAuthUser(IDBManager& db_manager) {
    auto users = db_manager.GetAuthUsers();
    assert(!users.empty());

    auto& user = users[0];
    auto new_user = user->Clone();

    new_user->SetId(10);
    new_user->SetName("New_User");

    users.push_back(std::move(new_user));

    return db_manager.AuthUsers(users, "secret_key");
  }
};

}  // namespace

TEST(TestInterLayer, TestRealAuthUsers) {
  DBManager db_manager;

  auto users = db_manager.GetAuthUsers();
  ASSERT_EQ(users.size(), 3);
  for (std::size_t i = 1; i <= 3; ++i) {
    auto& user = users[i - 1];
    EXPECT_EQ("User_" + std::to_string(i), user->GetName());
    EXPECT_EQ(i, user->GetId());
  }

  auto new_user = users[1]->Clone();
  EXPECT_EQ("User_2", new_user->GetName());
  EXPECT_EQ(2, new_user->GetId());

  users.push_back(std::move(new_user));

  auto authenticated = db_manager.AuthUsers(users, "secret_key");
  EXPECT_TRUE(authenticated);

  auto not_authenticated = db_manager.AuthUsers(users, "invalid_key");
  EXPECT_FALSE(not_authenticated);
}

TEST(TestInterLayer, TestAuthUsersWithFakeDBManager) {
  // Arrange
  using ::testing::_;
  using ::testing::Invoke;

  MockIDBManager fake_db_manager;
  EXPECT_CALL(fake_db_manager, GetAuthUsers()).WillRepeatedly(Invoke([]() {
    std::vector<UserPtr> result;
    result.reserve(2);
    result.emplace_back(std::make_unique<MysqlUser>("A", 1));
    result.emplace_back(std::make_unique<MysqlUser>("B", 2));
    return result;
  }));

  EXPECT_CALL(fake_db_manager, AuthUsers(_, _))
      .WillRepeatedly(Invoke([](std::vector<UserPtr>& users, const std::string& key) {
        EXPECT_EQ(3u, users.size());
        EXPECT_TRUE(key.find("secret") != std::string::npos);

        EXPECT_EQ("A", users[0]->GetName());
        EXPECT_EQ(1, users[0]->GetId());

        EXPECT_EQ("B", users[1]->GetName());
        EXPECT_EQ(2, users[1]->GetId());

        EXPECT_EQ("New_User", users[2]->GetName());
        EXPECT_EQ(10, users[2]->GetId());
        return true;
      }));

  // Act
  auto done = BusinessUserManager::CreateNewAuthUser(fake_db_manager);

  // Assert
  EXPECT_TRUE(done);
}
