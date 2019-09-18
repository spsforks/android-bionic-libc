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

#pragma once

#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <benchmark/benchmark.h>

typedef void (*benchmark_func_t)(benchmark::State& state);

struct BionicBenchmark {
  benchmark_func_t func;
  std::string arg;
  std::function<void(benchmark::internal::Benchmark*)> config_func;
};

extern std::map<std::string, BionicBenchmark> g_bionic_benchmarks;

__attribute__((unused))
static int RegisterBionicBenchmark(const std::string& fn_name, benchmark_func_t fn_ptr,
                                   std::function<void(benchmark::internal::Benchmark*)> config_func={},
                                   const std::string& arg="") {
  g_bionic_benchmarks[fn_name] = {
    .func = fn_ptr,
    .arg = arg,
    .config_func = std::move(config_func),
  };
  return 0;
}

#define DECLARE_BIONIC_BENCHMARK(n, ...) \
  int _bionic_benchmark_##n __attribute__((unused)) = RegisterBionicBenchmark(#n, __VA_ARGS__)

#define BIONIC_BENCHMARK(n) \
  DECLARE_BIONIC_BENCHMARK(n, n)

#define BIONIC_BENCHMARK_WITH_ARG(n, arg) \
  DECLARE_BIONIC_BENCHMARK(n, n, {}, arg)


constexpr auto KB = 1024;

typedef struct {
  int cpu_to_lock = -1;
  long num_iterations = 0;
  std::string xmlpath;
  std::vector<std::string> extra_benchmarks;
} bench_opts_t;

// This function returns a pointer less than 2 * alignment + or_mask bytes into the array.
char* GetAlignedMemory(char* orig_ptr, size_t alignment, size_t or_mask);

char* GetAlignedPtr(std::vector<char>* buf, size_t alignment, size_t nbytes);

wchar_t* GetAlignedPtr(std::vector<wchar_t>* buf, size_t alignment, size_t nbytes);

char* GetAlignedPtrFilled(std::vector<char>* buf, size_t alignment, size_t nbytes, char fill_byte);

bool LockToCPU(int cpu_to_lock);
