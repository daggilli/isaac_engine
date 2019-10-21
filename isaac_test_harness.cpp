#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "isaac.h"
#include "isaac_engine.h"

int main() {
#if 0
  // std::random_device rd;

  IsaacEngine::IsaacEngine iseng;

  std::cout << std::setfill('0') << std::hex << std::setw(8) << iseng() << "\n";

  exit(0);
#endif

  uint32_t seed[Isaac::RANDOM_SEED_SIZE];

  std::fill(seed, seed + Isaac::RANDOM_SEED_SIZE, 0);

  Isaac::Isaac isa(seed, Isaac::RANDOM_SEED_SIZE);

  std::cout << std::hex << std::setw(8) << std::setfill('0') << isa.rand() << "\n";

#if 0
  std::random_device rd;

  /* Isaac::Isaac isard(rd);

  for (auto i = 0; i < 10; i++) {
    std::cout << std::hex << std::setw(8) << std::setfill('0') << isard.rand() << "\n";
  } */

  IsaacEngine::IsaacEngine iseng(rd);

  std::uniform_int_distribution<> dis(10, 35);

  for (auto i = 0; i < 10; i++) {
    std::cout << dis(iseng) << "\n";
  }

  /* std::normal_distribution<double> nd(0, 1);

  std::ofstream ndf("nd.dat");

  for (auto i = 0; i < 250000; i++) {
    ndf << std::setprecision(12) << nd(iseng) << "\n";
  } */

  IsaacEngine::IsaacEngine isenga, isengb;
  isenga.discard(1);
  isengb.discard(2);

  std::cout << std::boolalpha << (isenga == isengb) << "\n";

  std::string sstr("1234567");

  isenga.seed(sstr);

  std::vector<uint32_t> svec{1, 2, 4, 5, 6, 7, 8};

  isengb.seed(svec);

  /*
  for (auto i = 0; i < 2560; i++) isa.rand();

  std::cout << isa << "\n";
   */

#endif

  std::vector<uint32_t> randVec;
  auto ix = 0;

#if 0
  Isaac::Isaac isc("This is <i>not</i> the right mytext.", 36);

  std::cout << "\n";

  for (auto i = 0; i < 3; i++) {
    for (auto j = 0; j < 8; j++) {
      auto rval = isc.rand();
      // std::cout << std::setbase(16) << std::setw(8) << std::setfill('0') << rval << " ";
      randVec.emplace_back(rval);
    }
    std::cout << "\n";
  }

  ix = 0;
  for (auto r : randVec) {
    std::cout << std::setbase(16) << std::setw(8) << std::setfill('0') << r << " ";
    if (++ix % 8 == 0) std::cout << "\n";
  }
  std::cout << "\n" << std::setbase(10) << randVec.size() << "\n";
#endif

  std::cout << "\n";

#if 0
  randVec.clear();

  IsaacEngine::IsaacEngine iseng;

  iseng.seed("This is <i>not</i> the right mytext.");

  for (auto i = 0; i < 3; i++) {
    for (auto j = 0; j < 8; j++) {
      auto rval = iseng();
      // std::cout << std::setbase(16) << std::setw(8) << std::setfill('0') << rval << " ";
      randVec.emplace_back(rval);
    }
    std::cout << "\n";
  }

  ix = 0;
  for (auto r : randVec) {
    std::cout << std::setbase(16) << std::setw(8) << std::setfill('0') << r << " ";
    if (++ix % 8 == 0) std::cout << "\n";
  }
  std::cout << "\n" << std::setbase(10) << randVec.size() << "\n";
#endif

  // auto g = Isaac::GOLDEN_RATIO;

  return 0;
}
