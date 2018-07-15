// Copyright [2018] <Malinovsky Rodion>

#include "core/general_error.h"
#include "util/enum_util.h"

#include <gtest/gtest.h>

using rms::core::GeneralError;
using rms::util::enum_util::EnumToString;
using rms::util::enum_util::ToIntegral;

TEST(TestGeneralErrorEnum, EnumToString) {
  EXPECT_EQ("Success", EnumToString(GeneralError::Success));
  EXPECT_EQ("Internal error", EnumToString(GeneralError::InternalError));
  EXPECT_EQ("Wrong command line arguments", EnumToString(GeneralError::WrongCommandLine));
  EXPECT_EQ("Startup has failed", EnumToString(GeneralError::StartupFailed));
}

TEST(TestGeneralErrorEnum, ToIntegral) {
  EXPECT_EQ(0, ToIntegral(GeneralError::Success));
  EXPECT_EQ(1, ToIntegral(GeneralError::InternalError));
  EXPECT_EQ(2, ToIntegral(GeneralError::WrongCommandLine));
  EXPECT_EQ(3, ToIntegral(GeneralError::StartupFailed));
}

TEST(TestGeneralErrorEnum, Makers) {
  EXPECT_TRUE(make_error_condition(GeneralError::InternalError));
  EXPECT_TRUE(make_error_code(GeneralError::InternalError));
  EXPECT_EQ("Internal error", make_error_code(GeneralError::InternalError).message());
  const std::string name{make_error_code(GeneralError::InternalError).category().name()};
  EXPECT_EQ("GeneralError", name);
}
