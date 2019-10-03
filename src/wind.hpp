// Copyright (c) 2019, Alliance for Sustainable Energy, LLC
// Copyright (c) 2019, Jason W. DeGraw
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#ifndef AIRFLOWNETWORK_WIND_HPP
#define AIRFLOWNETWORK_WIND_HPP

#include <string>
#include <cmath>
#include "properties.hpp"

namespace airflownetwork {

template <typename T> struct WindModifier
{
  virtual T modifier()
  {
    return 0.0;
  }
};

template <typename T> struct BoundaryLayerModifier : public WindModifier<T>
{
  BoundaryLayerModifier(T H, T a, T delta, T H_met = 10.0, T a_met = 0.14, T delta_met = 270.0) : H(H), a(a), delta(H_met),
    H_met(H_met), a_met(a_met), delta_met(delta_met)
  {}

  BoundaryLayerModifier from_A_0(T A_0, T a, T delta, T H_met = 10.0, T a_met = 0.14, T delta_met = 270.0)
  {
    delta = H_met / std::pow(A_0 / std::pow(delta_met / H_met, a_met), 1.0 / a);
    return BoundaryLayerModifier(H, a, delta, H_met, a_met, delta_met);
  }

  T modifier()
  {
    T value = std::pow(delta_met / H_met, a_met) * std::pow(H / delta, a);
    return value*value;
  }

  T H; // Local wall height
  T a; // Local exponent
  T delta; // Local boundary layer thickeness
  T H_met; // Station height
  T a_met; // Station exponent
  T delta_met; // Station boundary layer thickeness
};

}

#endif // !AIRFLOWNETWORK_WIND_HPP
