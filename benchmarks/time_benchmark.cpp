/*
 * Copyright (C) 2013 The Android Open Source Project
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
#include "time_benchmark.h"

#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <map>

#include <benchmark/benchmark.h>

static void BM_time_clock_gettime(benchmark::State& state) {
  timespec t;
  while (state.KeepRunning()) {
    clock_gettime(CLOCK_MONOTONIC, &t);
  }
}

static void BM_time_clock_gettime_syscall(benchmark::State& state) {
  timespec t;
  while (state.KeepRunning()) {
    syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &t);
  }
}

static void BM_time_gettimeofday(benchmark::State& state) {
  timeval tv;
  while (state.KeepRunning()) {
    gettimeofday(&tv, nullptr);
  }
}

void BM_time_gettimeofday_syscall(benchmark::State& state) {
  timeval tv;
  while (state.KeepRunning()) {
    syscall(__NR_gettimeofday, &tv, nullptr);
  }
}

void BM_time_time(benchmark::State& state) {
  while (state.KeepRunning()) {
    time(nullptr);
  }
}

void BM_time_localtime(benchmark::State& state) {
  time_t t = time(nullptr);
  while (state.KeepRunning()) {
    localtime(&t);
  }
}

void BM_time_localtime_r(benchmark::State& state) {
  time_t t = time(nullptr);
  while (state.KeepRunning()) {
    struct tm tm;
    localtime_r(&t, &tm);
  }
}

void declare_time_benchmarks(std::map <std::string, benchmark_func_t>& str_to_func) {
  str_to_func.emplace(std::string("BM_time_clock_gettime"), BM_time_clock_gettime);
  str_to_func.emplace(std::string("BM_time_clock_gettime_syscall"), BM_time_clock_gettime_syscall);
  str_to_func.emplace(std::string("BM_time_gettimeofday"), BM_time_gettimeofday);
  str_to_func.emplace(std::string("BM_time_gettimeofday_syscall"), BM_time_gettimeofday_syscall);
  str_to_func.emplace(std::string("BM_time_time"), BM_time_time);
  str_to_func.emplace(std::string("BM_time_localtime"), BM_time_localtime);
  str_to_func.emplace(std::string("BM_time_localtime_r"), BM_time_localtime_r);
}
