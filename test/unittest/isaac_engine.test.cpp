#define CATCH_CONFIG_MAIN

#ifndef __USE_MOCKRANDOM__
#define __USE_MOCKRANDOM__
#endif

#include <algorithm>
#include <catch/catch.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#ifndef __NO_ACCESS_CONTROL__
#define __NO_ACCESS_CONTROL__
#define private public
#endif

#include "../../isaac.h"
#include "../../isaac_engine.h"

// g++ -std=c++11 -Wall -o isaac_engine.test isaac_engine.test.cpp

TEST_CASE("Single random value from engine with zero key (pass)", "[single]") {
  IsaacRNG::IsaacEngine engine;
  REQUIRE(engine() == 0x182600f3);
}

TEST_CASE("First 2560 values with known key match test vector (pass)", "[match-test]") {
  std::vector<uint32_t> randVec;
  IsaacRNG::IsaacEngine iseng;

  iseng.seed("This is <i>not</i> the right mytext.");

  for (auto i = 0; i < 320; i++) {
    for (auto j = 0; j < 8; j++) {
      auto rval = iseng();
      randVec.emplace_back(rval);
    }
  }

  std::vector<uint32_t> testVec;

  std::ifstream randfile("../testvectors/randseed.txt");
  std::string line;
  while (std::getline(randfile, line)) {
    std::istringstream linestream(line);
    std::string nstr;
    while (linestream >> nstr) {
      testVec.emplace_back(std::stoul(nstr, nullptr, 16));
    }
  }

  REQUIRE(std::equal(randVec.begin(), randVec.end(), testVec.begin()));
}

TEST_CASE("First 2560 values with zero key match test vector (pass)", "[match-zero]") {
  std::vector<uint32_t> randVec;
  IsaacRNG::IsaacEngine iseng;

  for (auto i = 0; i < 320; i++) {
    for (auto j = 0; j < 8; j++) {
      auto rval = iseng();
      randVec.emplace_back(rval);
    }
  }

  std::vector<uint32_t> testVec;

  std::ifstream randfile("../testvectors/zerovect.txt");
  std::string line;
  while (std::getline(randfile, line)) {
    std::istringstream linestream(line);
    std::string nstr;
    while (linestream >> nstr) {
      testVec.emplace_back(std::stoul(nstr, nullptr, 16));
    }
  }

  REQUIRE(std::equal(randVec.begin(), randVec.end(), testVec.begin()));
}

TEST_CASE("Copy construcor (pass])", "[copyctor]") {
  IsaacRNG::IsaacEngine isa;
  IsaacRNG::IsaacEngine isb(isa);

  bool matches = true;

  for (auto i = 0; i < 16; i++) {
    matches &= (isa() == isb());
  }

  REQUIRE(matches);
}

TEST_CASE("Assignment operator (pass])", "[assign]") {
  IsaacRNG::IsaacEngine isa;
  IsaacRNG::IsaacEngine isb = isa;

  bool matches = true;

  for (auto i = 0; i < 16; i++) {
    matches &= (isa() == isb());
  }

  REQUIRE(matches);
}

TEST_CASE("Assignment operator (fail])", "[assignbad]") {
  IsaacRNG::IsaacEngine isa;
  IsaacRNG::IsaacEngine isb = isa;

  isb();

  bool matches = true;

  for (auto i = 0; i < 16; i++) {
    matches &= (isa() == isb());
  }

  REQUIRE(!matches);
}
