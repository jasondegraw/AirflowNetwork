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
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <array>
#include <string>
#include <fstream>
#include "fast.hpp"

int main(int argc, char* argv[])
{
  if (argc != 5) {
    std::cerr << "usage: pow_plots <power> <number> <start> <end>" << std::endl;
    return 1;
  }

  std::array<double, 2> F{ {0.0, 0.0} };
  std::array<double, 2> DF{ {0.0, 0.0} };

  double power = std::stod(argv[1]);
  int count = std::stoi(argv[2]);
  double start = std::stod(argv[3]);
  double end = std::stod(argv[4]);

  double delta = (end - start) / (double)count;

  std::ofstream out;
  out.open("pow_plots.csv");

  if (!out.good()) {
    std::cerr << "Failed to open \'pow_plots.csv\'" << std::endl;
    return EXIT_FAILURE;
  }

  double current = start;
  for (int i = 0; i < count; ++i) {
    out << current << ',' << std::pow(current, power) << ',' << fast_pow64(current, power) << std::endl;
    current += delta;
  }

  current = end;
  out << current << ',' << std::pow(current, power) << ',' << fast_pow64(current, power) << std::endl;

  out.close();

  return EXIT_SUCCESS;
}
