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

std::cout << isrd.rand() << "\n";
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

In addition, both classes have an empty constructor (without parameters) that initialises the internal seed to an array of 256 32-bit integers of value zero. Passing `nullptr` or a zero length seed vector/string/array to the other constructors will have the same effect.

Both classes have a copy constructor to create a new instance from an existing one. They also implement the assignment operator `operator=` so that the value of one instance can be assigned to another.