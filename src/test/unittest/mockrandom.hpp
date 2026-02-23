// Copyright (c) 2019–2026 David Gillies
// SPDX-License-Identifier: Unlicense
#pragma once

/**********************************************************************************

  This is a mock version of std::random_device to allow repeateable seeding of the
  random result vector so that whitebox testing can be performed.

  Written by David Gillies

  Released into the public domain. See LICENSE for details

  --

  N.B. a C++ compiler capable of generating C++11 compliant
  code is REQUIRED. g++ 4.7 will work, although g++ 4.8 or higher
  is preferred.

**********************************************************************************/
#include <cstdint>

namespace IsaacRNG {
  class mock_entropy_source {
   public:
    using result_type = uint32_t;
    static constexpr result_type(min)() { return 0; }
    static constexpr result_type(max)() { return UINT32_MAX; }

    result_type operator()() { return 0; }
    double entropy() { return 0.0; }
  };
}  // namespace IsaacRNG
