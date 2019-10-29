#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "isaac.h"
#include "isaac_engine.h"

int main() {
  IsaacRNG::Isaac iscDefault;  // empty ctor - seed all zeroes

  // draw one random value - should see 182600f3
  std::cout << std::setfill('0') << std::hex << std::setw(8) << iscDefault.rand() << "\n";

  // char* ctor
  const char *const charSeed = "Sphinx of black quartz, judge my vow";
  IsaacRNG::Isaac iscString(charSeed, std::strlen(charSeed));

  // uint32_t* ctor
  const uint32_t uintSeed[] = {0xDEADBEEF, 0xCABBAFEE, 0xA5A5A5A5};
  IsaacRNG::Isaac iscUint(uintSeed, sizeof(uintSeed) / sizeof(uint32_t));

  // random_device ctor
  std::random_device rd;
  IsaacRNG::Isaac iscRanDev(rd);

  // copy ctor
  IsaacRNG::Isaac iscCopy(iscUint);

  // move ctor - iscRanDev is now invalid
  IsaacRNG::Isaac iscMove(std::move(iscRanDev));

  // assign
  iscDefault = iscString;

  IsaacRNG::Isaac iscToBeMoved;

  // move assign - isengToBeMoved is now invalid
  IsaacRNG::Isaac iscMoveAssign = std::move(iscToBeMoved);

  // equality
  std::cout << std::boolalpha << (iscDefault == iscString) << "\n";

  // inequality
  std::cout << std::boolalpha << (iscDefault != iscUint) << "\n";

  // reseed

  iscDefault.seed("The five boxing wizards jump quickly", 36);
  iscDefault.seed(uintSeed, 3);
  iscUint.seed(rd);
  iscString.seed();  // all zeroes

  // I/O
  std::cout << iscUint << "\n";

  // provide randa, randdb, randc, randcnt and 256 uint32_t seed values
  std::cin >> iscString;

  IsaacRNG::IsaacEngine isengDefault;  // empty ctor - seed all zeroes

  // draw one random value - should see 182600f3
  std::cout << std::setfill('0') << std::hex << std::setw(8) << isengDefault() << "\n";

  // string ctor
  const std::string stringSeed("Pack my red box with five dozen quality jugs");
  IsaacRNG::IsaacEngine isengString(stringSeed);

  // vector<uint32_t> ctor
  const std::vector<uint32_t> uintVecSeed = {0xDEADBEEF, 0xCABBAFEE, 0xA5A5A5A5};
  IsaacRNG::IsaacEngine isengUint(uintVecSeed);

  // random_device ctor
  IsaacRNG::IsaacEngine isengRanDev(rd);

  // copy ctor
  IsaacRNG::IsaacEngine isengCopy(isengUint);

  // move ctor - isengRanDev is now invalid
  IsaacRNG::IsaacEngine isengMove(std::move(isengRanDev));

  // assign
  isengDefault = isengString;

  IsaacRNG::IsaacEngine isengToBeMoved;

  // move assign - isengToBeMoved is now invalid
  IsaacRNG::IsaacEngine isengMoveAssign = std::move(isengToBeMoved);

  // equality
  std::cout << std::boolalpha << (isengDefault == isengString) << "\n";

  // inequality
  std::cout << std::boolalpha << (isengDefault != isengUint) << "\n";

  // reseed

  isengDefault.seed("Jack quietly moved up front and seized the big ball of wax");
  isengDefault.seed(uintVecSeed);
  isengUint.seed(rd);
  isengString.seed();  // all zeroes

  // I/O
  std::cout << isengUint << "\n";

  // provide randa, randdb, randc, randcnt and 256 uint32_t seed values
  std::cin >> isengString;

  // advance the state of the RNG by n (n = 5 here) iterations
  isengString.discard(5);

  // seed an engine randomly, create a normal distribition
  // with mean zero and standard deviation one, and use the
  // engine to generate 10 deviates distributed according to
  // this distribition

  IsaacRNG::IsaacEngine isengNormDist(rd);

  std::normal_distribution<double> normDist(0, 1);

  for (auto i = 0; i < 10; i++) {
    std::cout << std::setprecision(12) << normDist(isengNormDist) << "\n";
  }

  return 0;
}
