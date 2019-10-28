#ifndef __ISAAC_ENGINE_H__
#define __ISAAC_ENGINE_H__

/**********************************************************************************

  C++ implementation of ISAAC CSPRNG as a Standard Template Library
  RandomNumberEngine type as per the named requirements used in the
  normative text of the C++ standard.

  ISAAC algorithm by Robert J. Jenkins Jr.
  http://www.burtleburtle.net/bob/rand/isaacafa.html

  Explicitly released into the public domain by Jenkins 26 Jun 2001
  see http://www.burtleburtle.net/bob/c/rand.h

  Adapted by David Gillies

  Adaptation released into the public domain. See LICENSE for details

  --

  N.B. a C++ compiler capable of generating C++11 compliant
  code is REQUIRED. g++ 4.7 will work, although g++ 4.8 or higher
  is preferred.

**********************************************************************************/

#include <algorithm>
#include <cstdint>
#ifndef __USE_MOCKRANDOM__
#include <random>
#endif
#include <string>
#include <vector>
#include "isaac.h"

namespace IsaacRNG {
  class IsaacEngine {
   public:
    friend std::ostream &operator<<(std::ostream &os, const IsaacEngine &is) { return os << is.prng; }
    using result_type = uint32_t;
    static constexpr result_type(min)() { return 0; }
    static constexpr result_type(max)() { return UINT32_MAX; }

    IsaacEngine() : prng() {}
    IsaacEngine(std::random_device &rd) : prng(rd) {}
    IsaacEngine(const std::vector<uint32_t> &seedVec) : prng(seedVec.data(), seedVec.size()) {}
    IsaacEngine(const std::string &seedStr) : prng(seedStr.data(), seedStr.length()) {}
    IsaacEngine(const IsaacEngine &iseng) : prng(iseng.prng) {}

    IsaacEngine &operator=(const IsaacEngine &iseng) {
      if (this != &iseng) prng = iseng.prng;

      return *this;
    }

    void seed() { prng.seed(static_cast<uint32_t *>(nullptr), 0); }
    void seed(std::random_device &rd) { prng.seed(rd); }
    void seed(const std::vector<uint32_t> &seedVec) { prng.seed(seedVec.data(), seedVec.size()); }
    void seed(const std::string &seedStr) { prng.seed(seedStr.data(), seedStr.length()); }
    result_type operator()() { return prng.rand(); }

    bool operator==(const IsaacEngine &rhs) { return prng == rhs.prng; }

    bool operator!=(const IsaacEngine &rhs) { return !(prng == rhs.prng); }

    void discard(unsigned long long n) {
      for (unsigned long long i = 0; i < n; i++) operator()();
    }

   private:
    Isaac prng;
  };
}  // namespace IsaacRNG
#endif