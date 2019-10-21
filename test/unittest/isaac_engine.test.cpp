#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <catch/catch.hpp>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

#define private public
#include "../../isaac.h"
#include "../../isaac_engine.h"

// g++ -std=c++11 -Wall -o isaac_engine.test isaac_engine.test.cpp

TEST_CASE("Single random value from engine with zero key (pass)", "[single]") {
  IsaacEngine::IsaacEngine engine;
  REQUIRE(engine() == 0x182600f3);
}

TEST_CASE("First 2560 values with known key match test vector (pass)", "[match-test]") {
  std::vector<uint32_t> randVec;
  IsaacEngine::IsaacEngine iseng;

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
  IsaacEngine::IsaacEngine iseng;

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

TEST_CASE("Isaac internal seed vector matches after two rounds with zero seed (pass)", "[internal]") {
  Isaac::Isaac isa;

  std::ostringstream outStr;

  for (auto i = 0; i < 2; i++) {
    isa.isaac();
    for (auto j = 0; j < 256; j++) {
      outStr << std::setbase(16) << std::setw(8) << std::setfill('0') << isa.randrsl[j];
      if ((j & 7) == 7) outStr << "\n";
    }
  }

  std::ifstream randfile("../testvectors/randvect.txt");
  std::string inStr;
  randfile.seekg(0, std::ios::end);
  inStr.reserve(randfile.tellg());
  randfile.seekg(0, std::ios::beg);

  inStr.assign(std::istreambuf_iterator<char>(randfile), std::istreambuf_iterator<char>());

  REQUIRE(outStr.str() == inStr);
}

TEST_CASE("Isaac internal seed vector matahces reference with known key (pass)", "[keytest]") {
  Isaac::Isaac isa("This is <i>not</i> the right mytext.", 36);

  std::vector<uint32_t> seedVec;
  std::copy(isa.randrsl, isa.randrsl + Isaac::RANDOM_SEED_SIZE, std::back_inserter(seedVec));

  std::vector<uint32_t> testVec;

  std::ifstream randfile("../testvectors/keytest.txt");
  std::string line;
  while (std::getline(randfile, line)) {
    std::istringstream linestream(line);
    std::string nstr;
    while (linestream >> nstr) {
      testVec.emplace_back(std::stoul(nstr, nullptr, 16));
    }
  }

  REQUIRE(std::equal(seedVec.begin(), seedVec.end(), testVec.begin()));
}
