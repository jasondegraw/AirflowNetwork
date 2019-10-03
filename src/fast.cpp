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
#include <stdint.h>
#include "fast.hpp"


double fast_pow64(double number, double power)
{
  uint64_t i;
  //double R{ (1.0-power) * 2.0 * 0x5fe6eb50c7b537a9 / 3.0 };
  double R{ (1.0 - power) * 0x3FEF478B2FCE251A };
  i = *(uint64_t*)& number;
  i = R + power * i;
  double y = *(double*)& i;
  return y;
}

double fast_pow64_065(double number)
{
  uint64_t i;
  double R{ 0.7 * 0x5fe6eb50c7b537a9 / 3.0 };
  i = *(uint64_t*)& number;
  i = R + 0.65 * i;
  double y = *(double*)& i;
  //y = 0.35 * y + 0.65 * number / (std::sqrt(y) * std::pow(y, 0.0384615384615385));
  //y = 0.35 * y + 0.65 * number / std::sqrt(y);
  //y = 0.35 * y + 0.65 * number / std::pow(y, 0.5384615384615385);
  //y = 0.35 * y + 0.65 * number / y;
  //y = 1.35 * y;
  return y;
}