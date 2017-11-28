// Copyright [2017] <Malinovsky Rodion>

#include "util/singleton.h"
#include "gmock/gmock.h"

namespace {

class CustomSchedulerFooTag;

class ISchedulerFoo {
 public:
  virtual ~ISchedulerFoo() = default;

  virtual int GetId() const = 0;

  virtual void SetId(int id) = 0;
};

class SchedulerFoo : public ISchedulerFoo {
 public:
  int GetId() const override {
    return id_;
  }

  void SetId(int id) override {
    id_ = id;
  }

 private:
  int id_ = 0;
};

class MockISchedulerFoo : public ISchedulerFoo {
 public:
  MOCK_CONST_METHOD0(GetId, int());
  MOCK_METHOD1(SetId, void(int));
};

using SchedulerFooAccessor = cppecho::util::SingleAccessor<ISchedulerFoo>;
SchedulerFooAccessor& GetSchedulerFooAccessorInstance() {
  return cppecho::util::single<SchedulerFooAccessor>();
}

}  // namespace

TEST(TestSingleton, DifferentSingletonsOfTheSameType) {
  {
    auto& foo = cppecho::util::single<SchedulerFoo>();
    ASSERT_EQ(foo.GetId(), 0);
    foo.SetId(5);
    auto& custom_foo =
        cppecho::util::single<SchedulerFoo, CustomSchedulerFooTag>();
    ASSERT_EQ(custom_foo.GetId(), 0);
    custom_foo.SetId(7);
  }
  auto& foo = cppecho::util::single<SchedulerFoo>();
  ASSERT_EQ(foo.GetId(), 5);
  auto& custom_foo =
      cppecho::util::single<SchedulerFoo, CustomSchedulerFooTag>();
  ASSERT_EQ(custom_foo.GetId(), 7);
}

TEST(TestSingleton, SingletonAccessorWithRealClass) {
  auto& single_accessor = GetSchedulerFooAccessorInstance();

  SchedulerFoo scheduler_foo;

  ASSERT_FALSE(single_accessor.GetIsAttached());
  single_accessor.Attach(scheduler_foo);
  ASSERT_TRUE(single_accessor.GetIsAttached());

  ASSERT_EQ(scheduler_foo.GetId(), 0);
  scheduler_foo.SetId(5);

  auto& scheduler_foo_new = cppecho::util::single<SchedulerFoo>();
  ASSERT_EQ(scheduler_foo_new.GetId(), 5);

  single_accessor.Detach();
  ASSERT_FALSE(single_accessor.GetIsAttached());
}

TEST(TestSingleton, SingletonAccessorWithMockClass) {
  using ::testing::Invoke;
  using ::testing::_;

  int fake_id = 10;

  auto& single_accessor = GetSchedulerFooAccessorInstance();

  MockISchedulerFoo fake_scheduler_foo;

  EXPECT_CALL(fake_scheduler_foo, GetId()).WillRepeatedly(Invoke([&fake_id]() {
    return fake_id;
  }));
  EXPECT_CALL(fake_scheduler_foo, SetId(_))
      .WillRepeatedly(Invoke([&fake_id](int id) { fake_id = id; }));

  ASSERT_FALSE(single_accessor.GetIsAttached());
  single_accessor.Attach(fake_scheduler_foo);
  ASSERT_TRUE(single_accessor.GetIsAttached());

  ISchedulerFoo& scheduler_foo = single_accessor;

  ASSERT_EQ(scheduler_foo.GetId(), fake_id);
  scheduler_foo.SetId(11);
  ASSERT_EQ(fake_id, 11);

  auto& auto_scheduler_foo = single_accessor.GetRef();

  ASSERT_EQ(auto_scheduler_foo.GetId(), fake_id);
  auto_scheduler_foo.SetId(12);
  ASSERT_EQ(fake_id, 12);

  single_accessor.Detach();
  ASSERT_FALSE(single_accessor.GetIsAttached());
}
