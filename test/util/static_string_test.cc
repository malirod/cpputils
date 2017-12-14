// Copyright [2017] <Malinovsky Rodion>

#include "util/static_string.h"
#include "gtest/gtest.h"

TEST(TestStaticString, BasicUsage) {
  constexpr rms::util::StaticString foo("foo");
  constexpr rms::util::StaticString foo_copy("foo");
  constexpr rms::util::StaticString fooo("fooo");
  constexpr rms::util::StaticString bar("bar");

  static_assert(foo.Size() == 3, "Invalid size of StaticString");
  static_assert(foo[0] == 'f', "Invalid data of StaticString");
  static_assert(foo[1] == 'o', "Invalid data of StaticString");
  static_assert(foo[2] == 'o', "Invalid data of StaticString");
  static_assert(foo[3] == '\0', "Invalid data of StaticString");
  static_assert(foo[3] == '\0', "Invalid data of StaticString");
  static_assert(!foo.IsEqual(bar), "Invalid data of StaticString");
  static_assert(!foo.IsEqual(fooo), "Invalid data of StaticString");
  static_assert(foo.IsEqual(foo_copy), "Invalid data of StaticString");

  auto str = foo.ToString();
  ASSERT_EQ(str, "foo");
}

TEST(TestStaticString, Substring) {
  constexpr rms::util::StaticString foo("foo");
  constexpr rms::util::StaticString bar("bar");
  constexpr rms::util::StaticString obar("obar");
  constexpr rms::util::StaticString foobar("foobar");
  constexpr rms::util::StaticString foo_substring(foobar, 0, 3);
  constexpr rms::util::StaticString bar_substring(foobar, 3, 3);
  constexpr rms::util::StaticString obar_substring(foobar, 2);

  static_assert(foo.IsEqual(foo_substring), "Invalid data of StaticString");
  static_assert(bar.IsEqual(bar_substring), "Invalid data of StaticString");
  static_assert(obar.IsEqual(obar_substring), "Invalid data of StaticString");
  static_assert(foo_substring.Size() == 3, "Invalid size of StaticString");
  static_assert(bar_substring.Size() == 3, "Invalid size of StaticString");
  static_assert(obar_substring.Size() == 4, "Invalid size of StaticString");

  auto foo_str = foo_substring.ToString();
  ASSERT_EQ(foo_str, "foo");
  auto bar_str = bar_substring.ToString();
  ASSERT_EQ(bar_str, "bar");
  auto obar_str = obar_substring.ToString();
  ASSERT_EQ(obar_str, "obar");
}
