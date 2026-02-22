// Copyright (c) 2019–2026 David Gillies
// SPDX-License-Identifier: Unlicense
#pragma once

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
#include <random>
#ifdef __USE_MOCKRANDOM__
#include "test/unittest/mockrandom.hpp"
using entropy_source = IsaacRNG::mock_entropy_source;
#else
using entropy_source = std::random_device;
#endif
#include <string>
#include <vector>

#include "isaac.hpp"

namespace IsaacRNG {
  class IsaacEngine {
   public:
    friend std::ostream &operator<<(std::ostream &os, const IsaacEngine &ise) { return os << ise.prng; }
    friend std::istream &operator>>(std::istream &is, IsaacEngine &ise) { return is >> ise.prng; }
    using result_type = uint32_t;
    static constexpr result_type(min)() { return 0; }
    static constexpr result_type(max)() { return UINT32_MAX; }

    IsaacEngine() : prng() {}
    IsaacEngine(entropy_source &rd) : prng(rd) {}
    IsaacEngine(const std::vector<uint32_t> &seedVec) : prng(seedVec.data(), seedVec.size()) {}
    IsaacEngine(const std::string &seedStr) : prng(seedStr.data(), seedStr.length()) {}
    IsaacEngine(const IsaacEngine &iseng) : prng(iseng.prng) {}
    IsaacEngine(IsaacEngine &&iseng) noexcept : prng(std::move(iseng.prng)) {}

    IsaacEngine &operator=(const IsaacEngine &iseng) {
      if (this != &iseng) prng = iseng.prng;

      return *this;
    }
    IsaacEngine &operator=(IsaacEngine &&iseng) noexcept {
      if (this != &iseng) prng = std::move(iseng.prng);

      return *this;
    }

    void seed() { prng.seed(static_cast<uint32_t *>(nullptr), 0); }
    void seed(entropy_source &rd) { prng.seed(rd); }
    void seed(const std::vector<uint32_t> &seedVec) { prng.seed(seedVec.data(), seedVec.size()); }
    void seed(const std::string &seedStr) { prng.seed(seedStr.data(), seedStr.length()); }
    void seed(const IsaacEngine &iseng) {
      if (this != &iseng) prng.seed(iseng.prng);
    }

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
