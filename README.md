# ISAAC C++ random number engine

## A C++ implementation of the ISAAC cryptographically secure pseudorandom number generator designed by Robert J. Jenkins Jr.

### Introduction
The ISAAC CSPRNG (<https://burtleburtle.net/bob/rand/isaacafa.html>) is a *pseudorandom number generator*. It generates a sequence of data whose values appear to be distributed at random within its range *i.e.* all values within the range are generated with (approximately) equal probability. It is *cryptographically secure* in the sense that possessing a sequence of previously emitted values does not allow one to predict subsequent values (at least not in a computationally feasible manner). ISAAC has a very long cycle time. In practice, if it is seeded with random keys, it will never produce the same sequence of values twice. It is very fast, easily capable of producing more than 100 megabytes of random values per second using modest hardware. It can be used as a high quality source of random numbers for Monte Carlo simulations, for generating crytographic nonces and as a stream cipher.

### Implementation
There are many implentations of ISAAC in many languages including C, Java, Go and Haskell. This imnplementation is in C++. In particular it provides a class which is conformant to the *RandomNumberEngine* named requirements of the C++ standard normative text.

### The classes
#### Isaac
There are two classes: `Isaac` and `IsaacEngine`. They are both in the namespace `IsaacRNG`.

Basic usage:
```c++
#include <iostream>
#include <random>
#include "isaac"

std::random_device rd;

IsaacRNG::Isaac isdefault;
IsaacRNG::Isaac isrd(rd);

std::cout << isrd.rand() << "\n";
```

#### IsaacEngine
Basic usage:
```c++
#include <iostream>
#include <random>
#include "isaac_engine"

std::random_device rd;

IsaacRNG::IsaacEngine isengdefault;
IsaacRNG::IsaacEngine isengrd(rd);

std::cout << isrd.rand() << "\n";
```
