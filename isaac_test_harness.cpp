#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include "isaac.h"
#include "isaac_engine.h"

int main() {
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
  return 0;
}
