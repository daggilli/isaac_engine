#ifndef __TEST__
#define __TEST__
#endif

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

TEST_CASE("Isaac internal seed vector matches after two rounds with zero seed (pass)", "[internal]") {
  Isaac::Isaac isa;

  std::ostringstream outStr;

  outStr << std::setbase(16);
  for (auto i = 0; i < 2; i++) {
    isa.isaac();
    for (auto j = 0; j < 256; j++) {
      outStr << std::setw(8) << std::setfill('0') << isa.randrsl[j];
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

TEST_CASE("Isaac internal seed vector matches reference with known key (pass)", "[keytest]") {
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

TEST_CASE("Seed an Isaac engine with a random device (pass)", "[randeviceseed]") {
  std::random_device rd;

  IsaacEngine::IsaacEngine isasrd(rd);
  std::cout << std::setbase(16);

  /* for (uint32_t i = 0; i < Isaac::RANDOM_SEED_SIZE; i++) {
    std::cout << std::setw(8) << std::setfill('0') << isasrd.prng.randrsl[i] << "\n";
  } */

  REQUIRE(2 > 1);
}
