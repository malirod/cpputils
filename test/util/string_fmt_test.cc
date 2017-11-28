// Copyright [2017] <Malinovsky Rodion>

#include "util/string_fmt.h"
#include "gtest/gtest.h"

using cppecho::util::StringFmt;

TEST(TestStringFmt, BasicUsage) {
  const std::string expected = "Number #17, #18";
  const std::size_t index1 = 17;
  const std::size_t index2 = 18;
  auto actual = StringFmt("Number #%1%, #%2%").format(index1, index2);
  EXPECT_EQ(expected, actual);
}
