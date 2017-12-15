// Copyright [2017] <Malinovsky Rodion>

#include "util/enum_util.h"
#include <sstream>
#include "gtest/gtest.h"

namespace {

enum class EnumDefaultInit {
  Foo,
  Bar,
};

enum class EnumCustomInit {
  Foo = 1,
  Bar,
};

}  // namespace

using rms::util::enum_util::EnumFromStream;
using rms::util::enum_util::EnumStrings;
using rms::util::enum_util::EnumToChars;
using rms::util::enum_util::EnumToStream;
using rms::util::enum_util::EnumToString;
using rms::util::enum_util::FromIntegral;
using rms::util::enum_util::ToIntegral;

template <>
EnumStrings<EnumDefaultInit>::DataType EnumStrings<EnumDefaultInit>::data = {"FooDef", "BarDef"};

template <>
EnumStrings<EnumCustomInit>::DataType EnumStrings<EnumCustomInit>::data = {"Dummy", "FooCustom", "BarCustom"};

TEST(TestEnumUtil, ToIntergal) {
  EXPECT_EQ(ToIntegral(EnumDefaultInit::Foo), 0);
  EXPECT_EQ(ToIntegral(EnumDefaultInit::Bar), 1);
  EXPECT_EQ(ToIntegral(EnumCustomInit::Foo), 1);
  EXPECT_EQ(ToIntegral(EnumCustomInit::Bar), 2);
}

TEST(TestEnumUtil, FromIntergal) {
  EXPECT_EQ(EnumDefaultInit::Foo, FromIntegral<EnumDefaultInit>(0));
  EXPECT_EQ(EnumDefaultInit::Bar, FromIntegral<EnumDefaultInit>(1));
  EXPECT_EQ(EnumCustomInit::Foo, FromIntegral<EnumCustomInit>(1));
  EXPECT_EQ(EnumCustomInit::Bar, FromIntegral<EnumCustomInit>(2));
}

TEST(TestEnumUtil, ToStringStreamWithoutInit) {
  std::stringstream sstream;
  sstream << EnumToStream(EnumDefaultInit::Foo) << "," << EnumToStream(EnumDefaultInit::Bar);

  EXPECT_STREQ("FooDef,BarDef", sstream.str().c_str());
}

TEST(TestEnumUtil, ToStringStreamWithInit) {
  std::stringstream sstream;
  sstream << EnumToStream(EnumCustomInit::Foo) << "," << EnumToStream(EnumCustomInit::Bar);

  EXPECT_STREQ("FooCustom,BarCustom", sstream.str().c_str());
}

TEST(TestEnumUtil, FromStringStreamDefaultInit) {
  std::stringstream enum_string("BarDef");
  auto value = EnumCustomInit::Bar;
  enum_string >> EnumFromStream(value);

  EXPECT_TRUE(value == EnumCustomInit::Bar);
}

TEST(TestEnumUtil, ToStringCustomInit) {
  EXPECT_EQ(std::string("FooDef"), EnumToString(EnumDefaultInit::Foo));
  EXPECT_EQ(std::string("BarDef"), EnumToString(EnumDefaultInit::Bar));
  EXPECT_STREQ("FooDef", EnumToChars(EnumDefaultInit::Foo));
  EXPECT_STREQ("BarDef", EnumToChars(EnumDefaultInit::Bar));
}

TEST(TestEnumUtil, ToStringForInvalidEnumItem) {
  EXPECT_EQ(std::string("Dummy"), EnumToString(FromIntegral<EnumCustomInit>(0)));
  EXPECT_STREQ("Dummy", EnumToChars(FromIntegral<EnumCustomInit>(0)));
  EXPECT_TRUE(EnumToString(FromIntegral<EnumCustomInit>(3)).empty());
  EXPECT_STREQ("", EnumToChars(FromIntegral<EnumCustomInit>(3)));
}
