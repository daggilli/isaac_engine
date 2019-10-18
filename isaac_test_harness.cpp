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

#if 0
  uint32_t seed[Isaac::RANDOM_SEED_SIZE];

  std::fill(seed, seed + Isaac::RANDOM_SEED_SIZE, 0);

  Isaac::Isaac isa(seed, Isaac::RANDOM_SEED_SIZE);

  // std::cout << std::hex << std::setw(8) << std::setfill('0') << isa.rand() << "\n";

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

#if 1
  Isaac::Isaac isc("This is <i>not</i> the right mytext.", 36);

  std::cout << "\n";

  for (auto i = 0; i < 32; i++) {
    for (auto j = 0; j < 8; j++) {
      std::cout << std::setbase(16) << std::setw(8) << std::setfill('0') << isc.rand() << " ";
    }
    std::cout << "\n";
  }
#endif
  return 0;
}
