#ifndef __ISAAC_H__
#define __ISAAC_H__

/**********************************************************************

   C++ implementation of ISAAC CSPRNG

   Algorithm by Robert J. Jenkins Jr.

   Adapted by David Gillies

   N.B. a C++ compiler capable of generating C++11 compliant
   code is REQUIRED. g++ 4.7 will work, although g++ 4.8 or higher
   is preferred.

**********************************************************************/

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <ios>
#include <random>

static_assert(__cplusplus >= 201103, "C++ version must be C++11 or greater");

namespace Isaac {
  const uint32_t kRandSizeBits = 8;
  const std::size_t kRandSize = 1 << kRandSizeBits;
  const std::size_t RANDOM_SEED_SIZE = kRandSize;  // alias for use in user programs

  class Isaac {
   public:
    friend bool operator==(const Isaac& lhs, const Isaac& rhs);
    friend bool operator!=(const Isaac& lhs, const Isaac& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Isaac& is);
    explicit Isaac() : Isaac(static_cast<uint32_t*>(nullptr), 0) {}

    Isaac(const uint32_t* const seedArr, const std::size_t seedlen)
        : randa(0),
          randb(0),
          randc(0),
          a(0x9e3779b9),
          b(0x9e3779b9),
          c(0x9e3779b9),
          d(0x9e3779b9),
          e(0x9e3779b9),
          f(0x9e3779b9),
          g(0x9e3779b9),
          h(0x9e3779b9) {
      seed(seedArr, seedlen);
    }

    Isaac(const char* const seedArr, const std::size_t seedlen)
        : randa(0),
          randb(0),
          randc(0),
          a(0x9e3779b9),
          b(0x9e3779b9),
          c(0x9e3779b9),
          d(0x9e3779b9),
          e(0x9e3779b9),
          f(0x9e3779b9),
          g(0x9e3779b9),
          h(0x9e3779b9) {
      seed(seedArr, seedlen);
    }

    Isaac(std::random_device& rd)
        : randa(0),
          randb(0),
          randc(0),
          a(0x9e3779b9),
          b(0x9e3779b9),
          c(0x9e3779b9),
          d(0x9e3779b9),
          e(0x9e3779b9),
          f(0x9e3779b9),
          g(0x9e3779b9),
          h(0x9e3779b9) {
      seed(rd);
    }

    void seed(const uint32_t* const seedArr, const std::size_t seedlen) {
      // std::cout << "SEED UINT32\n";
      std::fill(randrsl, randrsl + kRandSize, 0);
      if (seedArr != nullptr) {
        std::size_t tlen = std::min(seedlen, kRandSize);
        std::copy(seedArr, seedArr + tlen, randrsl);
      }
      /* for (auto i = 0; i < 8; i++) {
        for (auto j = 0; j < 8; j++) {
          std::cout << std::setw(8) << std::hex << std::setfill('0') << randrsl[i * 8 + j] << " ";
        }
        std::cout << "\n";
      } */
      randinit(true);
      /*isaac();
      for (auto i = 0; i < 32; i++) {
        for (auto j = 0; j < 8; j++) {
          std::cout << std::setw(8) << std::hex << std::setfill('0') << randrsl[i * 8 + j] << " ";
        }
        std::cout << "\n";
      }
      std::cout << "\n";
      isaac();
      for (auto i = 0; i < 32; i++) {
        for (auto j = 0; j < 8; j++) {
          std::cout << std::setw(8) << std::hex << std::setfill('0') << randrsl[i * 8 + j] << " ";
        }
        std::cout << "\n";
      } */
    }

    void seed(const char* const seedArr, const std::size_t seedlen) {
      // std::cout << "SEED CHAR\n";
      std::fill(randrsl, randrsl + kRandSize, 0);
      if (seedArr != nullptr) {
        std::size_t tlen = std::min(seedlen, kRandSize * sizeof(uint32_t));
        std::memcpy(reinterpret_cast<char*>(randrsl), seedArr, tlen);
      }
      /* for (auto i = 0; i < 32; i++) {
        for (auto j = 0; j < 8; j++) {
          std::cout << std::setw(8) << std::hex << std::setfill('0') << randrsl[i * 8 + j] << " ";
        }
        std::cout << "\n";
      }
      std::cout << "\nAFTER 1st RANDINIT\n"; */
      randinit(true);
      /* for (auto i = 0; i < 32; i++) {
        for (auto j = 0; j < 8; j++) {
          std::cout << std::setw(8) << std::hex << std::setfill('0') << randrsl[i * 8 + j] << " ";
        }
        std::cout << "\n";
      } */
    }

    void seed(std::random_device& rd) {
      std::generate(randrsl, randrsl + kRandSize, [&rd]() -> uint32_t { return static_cast<uint32_t>(rd()); });
      randinit(true);
    }

    uint32_t rand() {
      if (randcnt-- == 0) {
        isaac();
        randcnt = kRandSize - 1;
      }
      return randrsl[randcnt];
    }

   private:
    void isaac() {
      uint32_t x, y, *m, *mm, *m2, *r, *mend;

      mm = randmem;
      r = randrsl;
      a = randa;
      b = randb + (++randc);
      for (m = mm, mend = m2 = m + (kRandSize / 2); m < mend;) {
        rngstep(a << 13, mm, m, m2, r, x, y);
        rngstep(a >> 6, mm, m, m2, r, x, y);
        rngstep(a << 2, mm, m, m2, r, x, y);
        rngstep(a >> 16, mm, m, m2, r, x, y);
      }
      for (m2 = mm; m2 < mend;) {
        rngstep(a << 13, mm, m, m2, r, x, y);
        rngstep(a >> 6, mm, m, m2, r, x, y);
        rngstep(a << 2, mm, m, m2, r, x, y);
        rngstep(a >> 16, mm, m, m2, r, x, y);
      }
      randb = b;
      randa = a;
    }

    void randinit(const bool flag) {
      size_t i;
      uint32_t *m = randmem, *r = randrsl;

      for (i = 0; i < 4; ++i) /* scramble it */
        mix();

      if (flag) {
        /* initialize using the contents of r[] as the seed */
        for (i = 0; i < kRandSize; i += 8) {
          a += r[i];
          b += r[i + 1];
          c += r[i + 2];
          d += r[i + 3];
          e += r[i + 4];
          f += r[i + 5];
          g += r[i + 6];
          h += r[i + 7];
          mix();
          m[i] = a;
          m[i + 1] = b;
          m[i + 2] = c;
          m[i + 3] = d;
          m[i + 4] = e;
          m[i + 5] = f;
          m[i + 6] = g;
          m[i + 7] = h;
        }

        /* do a second pass to make all of the seed affect all of m */
        for (i = 0; i < kRandSize; i += 8) {
          a += m[i];
          b += m[i + 1];
          c += m[i + 2];
          d += m[i + 3];
          e += m[i + 4];
          f += m[i + 5];
          g += m[i + 6];
          h += m[i + 7];
          mix();
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
        /* fill in m[] with messy stuff */
        for (i = 0; i < kRandSize; i += 8) {
          mix();
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

      isaac();             /* fill in the first set of results */
      randcnt = kRandSize; /* prepare to use the first set of results */
    }

    uint32_t ind(uint32_t* mm, uint32_t x) {
      return *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(mm) + (x & ((kRandSize - 1) << 2)));
    }

    void rngstep(uint32_t mixit, uint32_t*& mm, uint32_t*& m, uint32_t*& m2, uint32_t*& r, uint32_t& x, uint32_t& y) {
      x = *m;
      a = (a ^ (mixit)) + *(m2++);
      *(m++) = y = ind(mm, x) + a + b;
      *(r++) = b = ind(mm, y >> kRandSizeBits) + x;
    }

    void mix() {
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
    uint32_t a, b, c, d, e, f, g, h;

    class FormatSaver {
     public:
      FormatSaver(std::ostream& stream) : strm(stream), state(nullptr) { state.copyfmt(stream); }
      ~FormatSaver() { strm.copyfmt(state); }

     private:
      std::ostream& strm;
      std::ios state;
    };
  };

  bool operator==(const Isaac& lhs, const Isaac& rhs) {
    return lhs.randcnt == rhs.randcnt && lhs.a == rhs.a && lhs.b == rhs.b && lhs.c == rhs.c && lhs.d == rhs.d && lhs.e == rhs.e &&
           lhs.f == rhs.f && lhs.g == rhs.g && lhs.h == rhs.h && lhs.randa == rhs.randa && lhs.randb == rhs.randb &&
           lhs.randc == rhs.randc && std::equal(lhs.randrsl, lhs.randrsl + kRandSize, rhs.randrsl);
  }

  bool operator!=(const Isaac& lhs, const Isaac& rhs) { return !(lhs == rhs); }

  std::ostream& operator<<(std::ostream& os, const Isaac& is) {
    {
      Isaac::FormatSaver saver(os);
      os << std::setbase(16);
      os.fill('0');
      os << "      a: " << std::setw(8) << is.a << "\n";
      os << "      b: " << std::setw(8) << is.b << "\n";
      os << "      c: " << std::setw(8) << is.c << "\n";
      os << "      d: " << std::setw(8) << is.d << "\n";
      os << "      e: " << std::setw(8) << is.e << "\n";
      os << "      f: " << std::setw(8) << is.f << "\n";
      os << "      g: " << std::setw(8) << is.g << "\n";
      os << "      h: " << std::setw(8) << is.h << "\n";
      os << "  randa: " << std::setw(8) << is.randa << "\n";
      os << "  randb: " << std::setw(8) << is.randb << "\n";
      os << "  randc: " << std::setw(8) << is.randc << "\n";
      os << "randcnt: " << std::setw(8) << is.randcnt << "\n";
    }
    return os;
  }
}  // namespace Isaac

#endif
