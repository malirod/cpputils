// Copyright [2016] <Malinovsky Rodion>

#include "core/general_error.h"
#include "gtest/gtest.h"
#include "util/enum_util.h"

TEST(TestGeneralErrorEnum, EnumToString) {
  using cppecho::core::GeneralError;
  using cppecho::util::enum_util::EnumToString;
  EXPECT_EQ("Success", EnumToString(GeneralError::Success));
  EXPECT_EQ("Internal error", EnumToString(GeneralError::InternalError));
  EXPECT_EQ("Wrong command line arguments",
            EnumToString(GeneralError::WrongCommandLine));
  EXPECT_EQ("Startup has failed", EnumToString(GeneralError::StartupFailed));
}

TEST(TestGeneralErrorEnum, ToIntegral) {
  using cppecho::util::enum_util::ToIntegral;
  using cppecho::core::GeneralError;
  EXPECT_EQ(0, ToIntegral(GeneralError::Success));
  EXPECT_EQ(1, ToIntegral(GeneralError::InternalError));
  EXPECT_EQ(2, ToIntegral(GeneralError::WrongCommandLine));
  EXPECT_EQ(3, ToIntegral(GeneralError::StartupFailed));
}
