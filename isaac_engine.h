#ifndef __ISAAC_ENGINE_H__
#define __ISAAC_ENGINE_H__

#include <algorithm>
#include <cstdint>
#include <random>
#include <string>
#include <vector>
#include "isaac.h"

namespace IsaacEngine {
  class IsaacEngine {
   public:
    using result_type = uint32_t;
    static constexpr result_type(min)() { return 0; }
    static constexpr result_type(max)() { return UINT32_MAX; }
    friend bool operator==(const IsaacEngine &lhs, const IsaacEngine &rhs);
    friend bool operator!=(const IsaacEngine &lhs, const IsaacEngine &rhs);

    IsaacEngine() : prng() {}
    explicit IsaacEngine(std::random_device &rd) : prng(rd) {}

    void seed() { prng.seed(static_cast<uint32_t *>(nullptr), 0); }
    void seed(std::random_device &rd) { prng.seed(rd); }
    void seed(std::vector<uint32_t> &seedVec) { prng.seed(seedVec.data(), seedVec.size()); }
    void seed(const std::string &seedStr) { prng.seed(seedStr.data(), seedStr.length()); }
    result_type operator()() { return prng.rand(); }

    void discard(unsigned long long n) {
      for (unsigned long long i = 0; i < n; i++) operator()();
    }

   private:
    Isaac::Isaac prng;
  };

  bool operator==(const IsaacEngine &lhs, const IsaacEngine &rhs) { return lhs.prng == rhs.prng; }

  bool operator!=(const IsaacEngine &lhs, const IsaacEngine &rhs) { return !(lhs.prng == rhs.prng); }
}  // namespace IsaacEngine
#endif