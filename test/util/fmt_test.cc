// Copyright [2017] <Malinovsky Rodion>

#include <fmt/format.h>
#include <gtest/gtest.h>
#include <string>

TEST(TestFmt, FormatInt) {
  const std::string expected = "Number #17, #18";
  const int index1 = 17;
  const int index2 = 18;
  auto actual = fmt::format("Number #{}, #{}", index1, index2);
  EXPECT_EQ(expected, actual);
}

TEST(TestFmt, FormatString) {
  const std::string expected = "Strings: Hello World!";

  const auto hello = "Hello";
  const auto world = "World";

  auto actual = fmt::format("Strings: {} {}!", hello, world);
  EXPECT_EQ(expected, actual);
}

TEST(TestFmt, FormatWithRepeat) {
  const std::string expected = "str1 100500 str1 100500";
  const auto str = "str1";
  const int num = 100500;

  auto actual = fmt::format("{0} {1} {0} {1}", str, num);
  EXPECT_EQ(expected, actual);
}
