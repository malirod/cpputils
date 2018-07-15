// Copyright [2018] <Malinovsky Rodion>

#include "util/boost_string_fmt.h"
#include <gtest/gtest.h>

using rms::util::BoostStringFmt;

TEST(TestBoostStringFmt, FormatInt) {
  const std::string expected = "Number #17, #18";
  const int index1 = 17;
  const int index2 = 18;
  auto actual = BoostStringFmt("Number #%1%, #%2%").format(index1, index2);
  EXPECT_EQ(expected, actual);
}

TEST(TestBoostStringFmt, FormatString) {
  const std::string expected = "Strings: Hello World!";
  const auto hello = "Hello";
  const auto world = "World";

  auto actual = BoostStringFmt("Strings: %1% %2%!").format(hello, world);
  EXPECT_EQ(expected, actual);
}

TEST(TestBoostStringFmt, FormatWithRepeat) {
  const std::string expected = "str1 100500 str1 100500";
  const auto str = "str1";
  const int num = 100500;

  auto actual = BoostStringFmt("%1% %2% %1% %2%").format(str, num);
  EXPECT_EQ(expected, actual);
}
