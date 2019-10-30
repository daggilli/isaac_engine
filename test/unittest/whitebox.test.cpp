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
  IsaacRNG::Isaac isa;

  std::ostringstream outStr;

  outStr << std::setbase(16);
  for (auto i = 0; i < 2; i++) {
    isa.isaac();
    for (auto j = 0; j < 256; j++) {
      outStr << std::setw(8) << std::setfill('0') << isa.randrsl.get()[j];
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
  IsaacRNG::Isaac isa("This is <i>not</i> the right mytext.", 36);

  std::vector<uint32_t> seedVec;
  std::copy(isa.randrsl.get(), isa.randrsl.get() + IsaacRNG::RANDOM_SEED_SIZE, std::back_inserter(seedVec));

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

TEST_CASE("Construct an Isaac engine with a random device (pass)", "[randeviceconstruct]") {
  std::random_device rd;

  IsaacRNG::IsaacEngine isasrd(rd);

  std::vector<uint32_t> randVec;
  for (uint32_t i = 0; i < IsaacRNG::RANDOM_SEED_SIZE; i++) {
    randVec.emplace_back(isasrd.prng.randrsl.get()[i]);
  }

  std::vector<uint32_t> testVec;
  testVec.reserve(IsaacRNG::RANDOM_SEED_SIZE);

  std::ifstream randfile("../testvectors/zerovect.txt");
  std::string line;
  for (auto i = 0; i < 32; i++) {
    std::getline(randfile, line);
    std::istringstream linestream(line);
    std::string nstr;
    while (linestream >> nstr) {
      testVec.emplace_back(std::stoul(nstr, nullptr, 16));
    }
  }

  REQUIRE(std::equal(randVec.begin(), randVec.end(), testVec.rbegin()));
}

TEST_CASE("Construct an Isaac engine with a vector of uint32_t (pass)", "[intvecconstruct]") {
  std::vector<uint32_t> seedVec;
  seedVec.reserve(IsaacRNG::RANDOM_SEED_SIZE);

  for (size_t i = 0; i < IsaacRNG::RANDOM_SEED_SIZE; i++) {
    seedVec.emplace_back(0xDEADBEEF + i);
  }

  IsaacRNG::IsaacEngine isaiv(seedVec);

  std::vector<uint32_t> randVec;
  for (uint32_t i = 0; i < IsaacRNG::RANDOM_SEED_SIZE; i++) {
    randVec.emplace_back(isaiv.prng.randrsl.get()[i]);
  }

  std::vector<uint32_t> testVec;

  std::ifstream randfile("../testvectors/intvecseed.txt");
  std::string line;
  while (std::getline(randfile, line)) {
    std::istringstream linestream(line);
    std::string nstr;
    while (linestream >> nstr) {
      testVec.emplace_back(std::stoul(nstr, nullptr, 16));
    }
  }

  REQUIRE(std::equal(randVec.begin(), randVec.end(), testVec.rbegin()));
}

TEST_CASE("Construct an Isaac engine with a string (pass)", "[stringconstruct]") {
  IsaacRNG::IsaacEngine isaiv(std::string("Sphinx of black quartz, judge my vow"));

  std::vector<uint32_t> randVec;
  for (uint32_t i = 0; i < IsaacRNG::RANDOM_SEED_SIZE; i++) {
    randVec.emplace_back(isaiv.prng.randrsl.get()[i]);
  }

  std::vector<uint32_t> testVec;

  std::ifstream randfile("../testvectors/stringseed.txt");
  std::string line;
  while (std::getline(randfile, line)) {
    std::istringstream linestream(line);
    std::string nstr;
    while (linestream >> nstr) {
      testVec.emplace_back(std::stoul(nstr, nullptr, 16));
    }
  }

  REQUIRE(std::equal(randVec.begin(), randVec.end(), testVec.rbegin()));
}

TEST_CASE("Construct an Isaac engine from another one (pass)", "[engcopyconstruct]") {
  IsaacRNG::IsaacEngine isa;
  IsaacRNG::IsaacEngine isb(isa);

  REQUIRE(std::equal(isa.prng.randrsl.get(), isa.prng.randrsl.get() + IsaacRNG::RANDOM_SEED_SIZE, isb.prng.randrsl.get()));
  REQUIRE(isa.prng.randa == isb.prng.randa);
  REQUIRE(isa.prng.randb == isb.prng.randb);
  REQUIRE(isa.prng.randc == isb.prng.randc);
  REQUIRE(isa.prng.randcnt == isb.prng.randcnt);
}

TEST_CASE("Move construct an Isaac engine from another one (pass)", "[engmoveconstruct]") {
  IsaacRNG::IsaacEngine isa;
  uint32_t ra = isa.prng.randa, rb = isa.prng.randb, rc = isa.prng.randc, rcnt = isa.prng.randcnt;

  uint32_t *rsl = new uint32_t[IsaacRNG::RANDOM_SEED_SIZE];
  std::copy(isa.prng.randrsl.get(), isa.prng.randrsl.get() + IsaacRNG::RANDOM_SEED_SIZE, rsl);

  IsaacRNG::IsaacEngine isb(std::move(isa));

  REQUIRE(std::equal(isb.prng.randrsl.get(), isb.prng.randrsl.get() + IsaacRNG::RANDOM_SEED_SIZE, rsl));
  REQUIRE(isa.prng.randa == 0);
  REQUIRE(isa.prng.randb == 0);
  REQUIRE(isa.prng.randc == 0);
  REQUIRE(isa.prng.randcnt == 0);
  REQUIRE(isa.prng.randrsl == nullptr);
  REQUIRE(isb.prng.randa == ra);
  REQUIRE(isb.prng.randb == rb);
  REQUIRE(isb.prng.randc == rc);
  REQUIRE(isb.prng.randcnt == rcnt);

  delete[] rsl;
}

TEST_CASE("Seed an Isaac engine with a random device (pass)", "[randeviceseed]") {
  std::random_device rd;

  IsaacRNG::IsaacEngine isasrd;

  isasrd.seed(rd);

  std::vector<uint32_t> randVec;
  for (uint32_t i = 0; i < IsaacRNG::RANDOM_SEED_SIZE; i++) {
    randVec.emplace_back(isasrd.prng.randrsl.get()[i]);
  }

  std::vector<uint32_t> testVec;
  testVec.reserve(IsaacRNG::RANDOM_SEED_SIZE);

  std::ifstream randfile("../testvectors/zerovect.txt");
  std::string line;
  for (auto i = 0; i < 32; i++) {
    std::getline(randfile, line);
    std::istringstream linestream(line);
    std::string nstr;
    while (linestream >> nstr) {
      testVec.emplace_back(std::stoul(nstr, nullptr, 16));
    }
  }

  REQUIRE(std::equal(randVec.begin(), randVec.end(), testVec.rbegin()));
}

TEST_CASE("Seed an Isaac engine with a vector of uint32_t (pass)", "[intvecseed]") {
  std::vector<uint32_t> seedVec;
  seedVec.reserve(IsaacRNG::RANDOM_SEED_SIZE);

  for (size_t i = 0; i < IsaacRNG::RANDOM_SEED_SIZE; i++) {
    seedVec.emplace_back(0xDEADBEEF + i);
  }

  IsaacRNG::IsaacEngine isaiv;

  isaiv.seed(seedVec);

  std::vector<uint32_t> randVec;
  for (uint32_t i = 0; i < IsaacRNG::RANDOM_SEED_SIZE; i++) {
    randVec.emplace_back(isaiv.prng.randrsl.get()[i]);
  }

  std::vector<uint32_t> testVec;

  std::ifstream randfile("../testvectors/intvecseed.txt");
  std::string line;
  while (std::getline(randfile, line)) {
    std::istringstream linestream(line);
    std::string nstr;
    while (linestream >> nstr) {
      testVec.emplace_back(std::stoul(nstr, nullptr, 16));
    }
  }

  REQUIRE(std::equal(randVec.begin(), randVec.end(), testVec.rbegin()));
}

TEST_CASE("Seed an Isaac engine with a string (pass)", "[stringseed]") {
  IsaacRNG::IsaacEngine isas;

  isas.seed(std::string("Sphinx of black quartz, judge my vow"));

  std::vector<uint32_t> randVec;
  for (uint32_t i = 0; i < IsaacRNG::RANDOM_SEED_SIZE; i++) {
    randVec.emplace_back(isas.prng.randrsl.get()[i]);
  }

  std::vector<uint32_t> testVec;

  std::ifstream randfile("../testvectors/stringseed.txt");
  std::string line;
  while (std::getline(randfile, line)) {
    std::istringstream linestream(line);
    std::string nstr;
    while (linestream >> nstr) {
      testVec.emplace_back(std::stoul(nstr, nullptr, 16));
    }
  }

  REQUIRE(std::equal(randVec.begin(), randVec.end(), testVec.rbegin()));
}