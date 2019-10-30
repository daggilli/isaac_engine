# ISAAC C++ random number engine

## A C++ implementation of the ISAAC cryptographically secure pseudorandom number generator designed by Robert J. Jenkins Jr.

### Introduction
The ISAAC CSPRNG (<https://burtleburtle.net/bob/rand/isaacafa.html>) is a *pseudorandom number generator*. It generates a sequence of data whose values appear to be distributed at random within its range *i.e.* all values within the range are generated with (approximately) equal probability. It is *cryptographically secure* in the sense that possessing a sequence of previously emitted values does not allow one to predict subsequent values (at least not in a computationally feasible manner). ISAAC has a very long cycle time. In practice, if it is seeded with random keys, it will never produce the same sequence of values twice. It is very fast, easily capable of producing more than 100 megabytes of random values per second using modest hardware. It can be used as a high quality source of random numbers for Monte Carlo simulations, for generating crytographic nonces and as a stream cipher.

### Implementation
There are many implementations of ISAAC in many languages including C, Java, Go and Haskell. This imnplementation is in C++. In particular it provides a class which is conformant to the *RandomNumberEngine* [named requirements](https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine) of the C++ standard normative text. It also therefore satisfies the named requirements of *UniformRandomBitGenerator*.

### The classes
There are two classes: `Isaac` and `IsaacEngine`. They are both in the namespace `IsaacRNG`.

#### `Isaac`
`Isaac` is the low-level class that implements the ISAAC random number generator itself.

Basic usage:
```c++
#include <iostream>
#include <random>
#include "isaac"

std::random_device rd;

IsaacRNG::Isaac isrd(rd);

std::cout << isrd.rand() << "\n";
```

#### IsaacEngine
`IsaacEngine` implements a class that is conformant to the C++ *RandomNumberEngine* named requirement. It uses an instance of `Isaac` internally. It also implements *UniformRandomBitGenerator*.

Basic usage:
```c++
#include <iostream>
#include <random>
#include "isaac_engine"

std::random_device rd;

IsaacRNG::IsaacEngine isengrd(rd);

std::cout << isengrd() << "\n";
```
### Further details
Both `Isaac` and `IsaacEngine` can be seeded on construction and re-seeded later with the `::seed()` function. Seeding either class with a known key will cause the same sequence of values to be emitted. Acceptable seeds at construction are:

`Isaac`
* `uint32_t*, size_t` - an array of 32-bit integers and its length. The array can have up to 256 elements. If the array has fewer than 256 elements the seed will be padded with zeroes. If it has more the excess will be silently discarded.
* `char*, size_t` - an array of char and its length. The array can have up to 1024 elements. If the array has fewer than 1024 elements the seed will be padded with zeroes. If it has more the excess will be silently discarded.
* `std::random_device&` - the C++ standard library interface to a system-level random number device. Ideally this will be non-deterministic although this is implementation-dependent and not guaranteed. Unix-like systems with `/dev/random` will probably expose this through `std::random_device`. The ISAAC internal state will be set by reading 256 32-bit integers from this source.

`IsaacEngine`
* `std::vector<uint32_t>&` of up to 256 elements. If the vector has fewer than 256 elements the seed will be padded with zeroes. If it has more the excess will be silently discarded.
* `std::string&` of length up to 1024 characters. If the string has fewer than 1024 characters the seed will be padded with zero bytes. If it has more the string will be silently truncated.
* `std::random_device&` - as per `Isaac`.

In both cases, the random number generator can be reseeded (`IsaacRNG::Isaac::seed()` and `IsaacRNG::IsaacEngine::seed()`) with the same set of arguments as the respective constructors.

In addition, both classes have an empty constructor (without parameters) that initialises the internal seed to an array of 256 32-bit integers of value zero. Passing `nullptr` or a zero length seed vector/string/array to the other constructors will have the same effect. Similarly, calling `::seed()` with no parameters resets the seed to an array of zeroes.

Both classes have a copy constructor to create a new instance from an existing one. They also implement the assignment operator `operator=` so that the value of one instance can be assigned to another. Additionally, there are move constructors and move assignment operators in both cases.

### Example code
This code is found in [example.cpp](example.cpp)
```c++
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
```

### Testing
Tests are found in the [test](test) directory. The directory [test/unittest](test/unittest) contains unit and whitebox tests.

This project uses the [Catch2](https://github.com/catchorg/Catch2) library for testing. The `catch.hpp` include file will need to be in your compiler's include path. Catch is available as a package for Debian-like Linux distros and as a Homebrew formula (catch2) for macos, among others.

To build and run the tests under GNU `make`, navigate to the `test/unittest` directory and run `make test`. If the `OPT` flag is specified (*e.g.* `make OPT=1 test`) then the tests will be compiled with the optimisation level set to `-O4`. If the `LIBCWD` flag is specified and the [cwd](http://libcwd.sourceforge.net/) C++ debugging library is found, then minimal support for runtime debugging checks will be enabled. No additional instrumenting of the code is neeeded; `libcwd` out of the box will detect things like null pointer dereferences and double deletes.
