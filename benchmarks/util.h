/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _BIONIC_BENCHMARKS_UTIL_H_
#define _BIONIC_BENCHMARKS_UTIL_H_

#include <map>
#include <string>
#include <vector>

#include <benchmark/benchmark.h>

constexpr auto KB = 1024;

constexpr auto AT_SRC_ALIGN = 8;
constexpr auto AT_DST_ALIGN = 8;

static std::map<std::string, std::vector<std::vector<int>>> args_shorthand {
  {"AT_MACRO_TWOBUF", std::vector<std::vector<int>>{ {8, AT_SRC_ALIGN, AT_DST_ALIGN},
                                                       {64, AT_SRC_ALIGN, AT_DST_ALIGN},
                                                       {512, AT_SRC_ALIGN, AT_DST_ALIGN},
                                                       {1 * KB, AT_SRC_ALIGN, AT_DST_ALIGN},
                                                       {8 * KB, AT_SRC_ALIGN, AT_DST_ALIGN},
                                                       {16 * KB, AT_SRC_ALIGN, AT_DST_ALIGN},
                                                       {32 * KB, AT_SRC_ALIGN, AT_DST_ALIGN},
                                                       {64 * KB, AT_SRC_ALIGN, AT_DST_ALIGN} }},

  {"AT_MACRO_ONEBUF", std::vector<std::vector<int>>{ {(8), AT_SRC_ALIGN},
                                                       {(64), AT_SRC_ALIGN},
                                                       {(512), AT_SRC_ALIGN},
                                                       {(1*KB), AT_SRC_ALIGN},
                                                       {(8*KB), AT_SRC_ALIGN},
                                                       {(16*KB), AT_SRC_ALIGN},
                                                       {(32*KB), AT_SRC_ALIGN},
                                                       {(64*KB), AT_SRC_ALIGN}}},
  {"AT_ALIGNED_TWOBUF", std::vector<std::vector<int>>{ {8, 0, 0},
                                                       {64, 0, 0},
                                                       {512, 0, 0},
                                                       {1 * KB, 0, 0},
                                                       {8 * KB, 0, 0},
                                                       {16 * KB, 0, 0},
                                                       {32 * KB, 0, 0},
                                                       {64 * KB, 0, 0} }},

  {"AT_ALIGNED_ONEBUF", std::vector<std::vector<int>>{ {(8), 0},
                                                       {(64), 0},
                                                       {(512), 0},
                                                       {(1*KB), 0},
                                                       {(8*KB), 0},
                                                       {(16*KB), 0},
                                                       {(32*KB), 0},
                                                       {(64*KB), 0}}},
  {"AT_COMMON_SIZES", std::vector<std::vector<int>>{ {8}, {64}, {512}, {1*KB}, {8*KB}, {16*KB},
                                                     {32*KB}, {64*KB}}},

  // Do not exceed 512. that is about the largest number of properties
  // that can be created with the current property area size.
  {"NUM_PROPS", std::vector<std::vector<int>>{ {1}, {4}, {16}, {64}, {128}, {256}, {512} }},
  {"MATH_COMMON", std::vector<std::vector<int>>{ {0}, {1}, {2}, {3} }}
};

typedef struct {
  int cpu_to_lock;
  int num_iterations;
  std::string xmlpath;
  std::vector<std::string> extra_benchmarks;
} bench_opts_t;

typedef void (*benchmark_func_t) (benchmark::State&);

std::map<std::string, benchmark_func_t> DeclareBenchmarks();

// This function returns a pointer less than 2 * alignment + or_mask bytes into the array.
char *GetAlignedMemory(char *orig_ptr, size_t alignment, size_t or_mask);

char *GetAlignedPtr(std::vector<char>* buf, size_t alignment, size_t nbytes);

char *GetAlignedPtrFilled(std::vector<char>* buf, size_t alignment, size_t nbytes, char fill_byte);

bool LockToCPU(int cpu_to_lock);

std::string trim(std::string src);

#endif // _BIONIC_BENCHMARKS_UTIL_H
