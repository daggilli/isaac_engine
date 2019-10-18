#define CATCH_CONFIG_MAIN

#include <catch/catch.hpp>
#include <random>

#include "../../isaac_engine.h"

// g++ -std=c++11 -Wall -o isaac_enging.test isaac_engine.test.cpp

// std::random_device rd;

TEST_CASE("Single random value from engine with zero key (pass)", "[single-file]") {
  IsaacEngine::IsaacEngine engine;
  REQUIRE(engine() == 0x182600f3);
}