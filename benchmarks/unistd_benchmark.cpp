/*
 * Copyright (C) 2014 The Android Open Source Project
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
#include "unistd_benchmark.h"

#include <sys/syscall.h>
#include <unistd.h>

#include <map>

#include <benchmark/benchmark.h>

static void BM_unistd_getpid(benchmark::State& state) {
  while (state.KeepRunning()) {
    getpid();
  }
}

static void BM_unistd_getpid_syscall(benchmark::State& state) {
  while (state.KeepRunning()) {
    syscall(__NR_getpid);
  }
}

#if defined(__BIONIC__)

// Stop GCC optimizing out our pure function.
/* Must not be static! */ pid_t (*gettid_fp)() = gettid;

static void BM_unistd_gettid(benchmark::State& state) {
  while (state.KeepRunning()) {
    gettid_fp();
  }
}

#endif

void BM_unistd_gettid_syscall(benchmark::State& state) {
  while (state.KeepRunning()) {
    syscall(__NR_gettid);
  }
}

void declare_unistd_benchmarks(std::map <std::string, benchmark_func_t>& str_to_func) {
  str_to_func.emplace(std::string("BM_unistd_getpid"), BM_unistd_getpid);
  str_to_func.emplace(std::string("BM_unistd_getpid_syscall"), BM_unistd_getpid_syscall);
#if defined(__BIONIC__)
  str_to_func.emplace(std::string("BM_unistd_gettid"), BM_unistd_gettid);
#endif
  str_to_func.emplace(std::string("BM_unistd_gettid_syscall"), BM_unistd_gettid_syscall);
}
