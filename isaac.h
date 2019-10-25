#ifndef __ISAAC_H__
#define __ISAAC_H__

/**********************************************************************************

  C++ implementation of ISAAC CSPRNG

  Algorithm by Robert J. Jenkins Jr.
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
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <ios>
#ifdef __USE_MOCKRANDOM__
#include "test/unittest/mockrandom.h"
#else
#include <random>
#endif

static_assert(__cplusplus >= 201103, "C++ version must be C++11 or greater");

namespace {
  const uint32_t GOLDEN_RATIO = 0x9e3779b9;
}

namespace Isaac {
  const uint32_t kRandSizeBits = 8;
  const std::size_t kRandSize = 1 << kRandSizeBits;
  const std::size_t RANDOM_SEED_SIZE = kRandSize;  // alias for use in user programs

  class Isaac {
   public:
    friend std::ostream& operator<<(std::ostream& os, const Isaac& isc) {
      {
        Isaac::FormatSaver saver(os);
        os << std::setbase(10) << std::left;
        os << isc.randa << " " << isc.randb << " " << isc.randc << " ";
        os << isc.randcnt << " ";
        for (size_t i = 0; i < kRandSize; i++) os << isc.randrsl[i] << " ";
      }
      return os;
    }
    friend std::istream& operator>>(std::istream& is, Isaac& isc) {
      is >> isc.randa >> isc.randb >> isc.randc;
      is >> isc.randcnt;
      for (size_t i = 0; i < kRandSize; i++) is >> randrsl[i];
      return is;
    }
    explicit Isaac() : Isaac(static_cast<uint32_t*>(nullptr), 0) {}

    Isaac(const uint32_t* const seedArr, const std::size_t seedlen) { seed(seedArr, seedlen); }
    Isaac(const char* const seedArr, const std::size_t seedlen) { seed(seedArr, seedlen); }
    Isaac(std::random_device& rd) { seed(rd); }

    void seed(const uint32_t* const seedArr, const std::size_t seedlen) {
      std::fill(randrsl, randrsl + kRandSize, 0);
      if (seedArr != nullptr) {
        std::size_t tlen = std::min(seedlen, kRandSize);
        std::copy(seedArr, seedArr + tlen, randrsl);
      }
      randinit(true);
    }

    void seed(const char* const seedArr, const std::size_t seedlen) {
      std::fill(randrsl, randrsl + kRandSize, 0);
      if (seedArr != nullptr) {
        std::size_t tlen = std::min(seedlen, kRandSize * sizeof(uint32_t));
        std::memcpy(reinterpret_cast<char*>(randrsl), seedArr, tlen);
      }
      randinit(true);
    }

    void seed(std::random_device& rd) {
      std::generate(randrsl, randrsl + kRandSize, [&rd]() -> uint32_t { return static_cast<uint32_t>(rd()); });
      /* std::cout << std::setbase(16);
      for (uint32_t i = 0; i < RANDOM_SEED_SIZE; i++) {
        std::cout << std::setw(8) << std::setfill('0') << randrsl[i] << "\n";
      } */
      randinit(true);
    }

    uint32_t rand() {
      if (randcnt-- == 0) {
        isaac();
        randcnt = kRandSize - 1;
      }
      return randrsl[randcnt];
    }

    bool operator==(const Isaac& rhs) {
      return randcnt == rhs.randcnt && randa == rhs.randa && randb == rhs.randb && randc == rhs.randc &&
             std::equal(randrsl, randrsl + kRandSize, rhs.randrsl);
    }

    bool operator!=(const Isaac& rhs) { return !(*this == rhs); }

    std::string dump() {
      std::ostringstream outStr;

      outStr << std::setbase(16);
      outStr.fill('0');
      outStr << "  randa: " << std::setw(8) << randa << "\n";
      outStr << "  randb: " << std::setw(8) << randb << "\n";
      outStr << "  randc: " << std::setw(8) << randc << "\n";
      outStr << "randcnt: " << std::setw(8) << randcnt << "\n";

      return outStr.str();
      ;
    }

   private:
    void isaac() {
      uint32_t a, b, x, y, *m, *mm, *m2, *r, *mend;

      mm = randmem;
      r = randrsl;
      a = randa;
      b = randb + (++randc);
      for (m = mm, mend = m2 = m + (kRandSize / 2); m < mend;) {
        rngstep(a << 13, a, b, mm, m, m2, r, x, y);
        rngstep(a >> 6, a, b, mm, m, m2, r, x, y);
        rngstep(a << 2, a, b, mm, m, m2, r, x, y);
        rngstep(a >> 16, a, b, mm, m, m2, r, x, y);
      }
      for (m2 = mm; m2 < mend;) {
        rngstep(a << 13, a, b, mm, m, m2, r, x, y);
        rngstep(a >> 6, a, b, mm, m, m2, r, x, y);
        rngstep(a << 2, a, b, mm, m, m2, r, x, y);
        rngstep(a >> 16, a, b, mm, m, m2, r, x, y);
      }
      randb = b;
      randa = a;
    }

    void randinit(const bool flag) {
      uint32_t a, b, c, d, e, f, g, h;
      a = b = c = d = e = f = g = h = GOLDEN_RATIO;
      size_t i;
      uint32_t *m = randmem, *r = randrsl;
      randa = randb = randc = 0;

      mix(a, b, c, d, e, f, g, h);
      mix(a, b, c, d, e, f, g, h);
      mix(a, b, c, d, e, f, g, h);
      mix(a, b, c, d, e, f, g, h);

      if (flag) {
        for (i = 0; i < kRandSize; i += 8) {
          a += r[i];
          b += r[i + 1];
          c += r[i + 2];
          d += r[i + 3];
          e += r[i + 4];
          f += r[i + 5];
          g += r[i + 6];
          h += r[i + 7];
          mix(a, b, c, d, e, f, g, h);
          m[i] = a;
          m[i + 1] = b;
          m[i + 2] = c;
          m[i + 3] = d;
          m[i + 4] = e;
          m[i + 5] = f;
          m[i + 6] = g;
          m[i + 7] = h;
        }

        for (i = 0; i < kRandSize; i += 8) {
          a += m[i];
          b += m[i + 1];
          c += m[i + 2];
          d += m[i + 3];
          e += m[i + 4];
          f += m[i + 5];
          g += m[i + 6];
          h += m[i + 7];
          mix(a, b, c, d, e, f, g, h);
          m[i] = a;
          m[i + 1] = b;
          m[i + 2] = c;
          m[i + 3] = d;
          m[i + 4] = e;
          m[i + 5] = f;
          m[i + 6] = g;
          m[i + 7] = h;
        }
      } else {
        for (i = 0; i < kRandSize; i += 8) {
          mix(a, b, c, d, e, f, g, h);
          m[i] = a;
          m[i + 1] = b;
          m[i + 2] = c;
          m[i + 3] = d;
          m[i + 4] = e;
          m[i + 5] = f;
          m[i + 6] = g;
          m[i + 7] = h;
        }
      }

      isaac();
      randcnt = kRandSize;
    }

    uint32_t ind(uint32_t* mm, uint32_t x) {
      return *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(mm) + (x & ((kRandSize - 1) << 2)));
    }

    void rngstep(uint32_t mixit, uint32_t& a, uint32_t& b, uint32_t*& mm, uint32_t*& m, uint32_t*& m2, uint32_t*& r, uint32_t& x,
                 uint32_t& y) {
      x = *m;
      a = (a ^ (mixit)) + *(m2++);
      *(m++) = y = ind(mm, x) + a + b;
      *(r++) = b = ind(mm, y >> kRandSizeBits) + x;
    }

    void mix(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d, uint32_t& e, uint32_t& f, uint32_t& g, uint32_t& h) {
      a ^= b << 11;
      d += a;
      b += c;
      b ^= c >> 2;
      e += b;
      c += d;
      c ^= d << 8;
      f += c;
      d += e;
      d ^= e >> 16;
      g += d;
      e += f;
      e ^= f << 10;
      h += e;
      f += g;
      f ^= g >> 4;
      a += f;
      g += h;
      g ^= h << 8;
      b += g;
      h += a;
      h ^= a >> 9;
      c += h;
      a += b;
    }

    uint32_t randcnt;
    uint32_t randrsl[kRandSize];
    uint32_t randmem[kRandSize];
    uint32_t randa, randb, randc;
    // uint32_t a, b, c, d, e, f, g, h;

    class FormatSaver {
     public:
      FormatSaver(std::ostream& stream) : strm(stream), state(nullptr) { state.copyfmt(stream); }
      ~FormatSaver() { strm.copyfmt(state); }

     private:
      std::ostream& strm;
      std::ios state;
    };
  };
}  // namespace Isaac

#endif
