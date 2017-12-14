// Copyright [2017] <Malinovsky Rodion>

#include "gmock/gmock.h"
#include "util/logger.h"

int main(int argc, char** argv) {
  INIT_LOGGER("logger.cfg");
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
