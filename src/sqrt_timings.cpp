// Copyright (c) 2019, Alliance for Sustainable Energy, LLC
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
#include "properties.hpp"
#include "element.hpp"

float fast_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)& y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);             // what the fuck? 
	y = *(float*)& i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
//	y = y * (threehalfs - (x2 * y * y));   // 2nd iteration, this can be removed

	return y;
}

float fast_rrsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)& y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);             // what the fuck? 
	y = *(float*)& i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
//	y = y * (threehalfs - (x2 * y * y));   // 2nd iteration, this can be removed

	return 1.0 / y;
}

float fast_sqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)& y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);             // what the fuck? 
	y = *(float*)& i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
//	y = y * (threehalfs - (x2 * y * y));   // 2nd iteration, this can be removed

	return y;
}

double fast_sqrt64(double number)
{
	uint64_t i;
	double x2, y;
	const double R{ 0x5fe6eb50c7b537a9 / 3.0 };
	x2 = number * 0.5;
	y = number;
	i = *(uint64_t*)& y;
	i = R + (i >> 1);
	y = *(double*)& i;
	y = 0.5 * y + x2 / y;
	return y;
}

double fast_pow64_065(double number)
{
	uint64_t i;
	double x2, y;
	const double R{ 0.7 * 0x5fe6eb50c7b537a9 / 3.0 };
	//x2 = number * number;
	y = number;
	i = *(uint64_t*)& y;
	i = R + 0.65 * i;
	y = *(double*)& i;
	//y = 0.35 * y + 0.65 * number / (std::sqrt(y) * std::pow(y, 0.0384615384615385));
	//y = 0.35 * y + 0.65 * number / std::sqrt(y);
	//y = 0.35 * y + 0.65 * number / std::pow(y, 0.5384615384615385);
	//y = 0.35 * y + 0.65 * number / y;
	//y = 1.35 * y;
	return y;
}

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "usage: sqrt_timings <number>" << std::endl;
    return 1;
  }

  std::array<double, 2> F{ {0.0, 0.0} };
  std::array<double, 2> DF{ {0.0, 0.0} };

  int count = std::stoi(argv[1]);

  std::vector<double> pdrop(count);
  std::vector<double> result0(count);
  std::vector<double> result1(count);

  std::seed_seq seed{ 1, 2, 3, 4, 5, 6, 7, 8 };
  std::mt19937 rng(seed);

  std::uniform_real_distribution<double> pressure_drop(1.0e-6, 50.0);

  // Set up the states
  for (size_t i = 0; i < count; i++) {
    pdrop[i] = pressure_drop(rng);
  }

  // Calculate with genericCrack0
  auto start0 = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < count; i++) {
    //result0[i] = std::sqrt(pdrop[i]);
	result0[i] = std::pow(pdrop[i], 0.5);
	//result0[i] = std::pow(pdrop[i], 0.65);
  }
  auto stop0 = std::chrono::high_resolution_clock::now();

  // Calculate with genericCrack
  auto start1 = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < count; i++) {
	//result1[i] = fast_sqrt64(pdrop[i]);
	//result1[i] = std::pow(pdrop[i], 0.5);
	//result1[i] = fast_pow64_065(pdrop[i]);
	result1[i] = std::sqrt(pdrop[i]);
  }
  auto stop1 = std::chrono::high_resolution_clock::now();

  
  auto duration0 = std::chrono::duration_cast<std::chrono::microseconds>(stop0 - start0);
  auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);

  std::cout << "std::sqrt: " << duration0.count() << " microseconds" << std::endl;
  std::cout << "fast_sqrt: " << duration1.count() << " microseconds" << std::endl;
  std::cout << double(duration0.count()- duration1.count()) / double(duration0.count()) << std::endl;

  double rel_errmax = 0.0;
  for (size_t i = 0; i < count; i++) {
	  rel_errmax = std::max(rel_errmax, std::abs((result1[i] - result0[i])/result0[i]));
  }
  std::cout << rel_errmax << std::endl;

  return 0;
}