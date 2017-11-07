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

#include <stdint.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <benchmark/benchmark.h>
#include "util.h"

#ifndef CLOCK_TAI  // This is a reserved value that may not land in headers.
#define CLOCK_TAI 11
#endif
// guaranteed unsupported and positive regardless of implementation
// details by both vdso and syscall.
#define CLOCK_unsupported (INT8_MAX | (INT8_MAX << 8))

static void BM_time_clock_gettime(benchmark::State& state) {
  // CLOCK_MONOTONIC is required supported in vdso
  timespec t;
  while (state.KeepRunning()) {
    clock_gettime(CLOCK_MONOTONIC, &t);
  }
}
BIONIC_BENCHMARK(BM_time_clock_gettime);

static void BM_time_clock_gettime_syscall(benchmark::State& state) {
  // CLOCK_MONOTONIC is required supported in vdso
  timespec t;
  while (state.KeepRunning()) {
    syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &t);
  }
}
BIONIC_BENCHMARK(BM_time_clock_gettime_syscall);

static void BM_time_clock_gettime_REALTIME(benchmark::State& state) {
  // CLOCK_REALTIME is required supported in vdso
  timespec t;
  while (state.KeepRunning()) {
    clock_gettime(CLOCK_REALTIME, &t);
  }
}
BIONIC_BENCHMARK(BM_time_clock_gettime_REALTIME);

static void BM_time_clock_gettime_BOOTTIME(benchmark::State& state) {
  // CLOCK_BOOTTIME is optionally supported in vdso
  timespec t;
  while (state.KeepRunning()) {
    clock_gettime(CLOCK_BOOTTIME, &t);
  }
}
BIONIC_BENCHMARK(BM_time_clock_gettime_BOOTTIME);

static void BM_time_clock_gettime_TAI(benchmark::State& state) {
  // Helps measure syscall-only supported overhead.
  timespec t;
  while (state.KeepRunning()) {
    clock_gettime(CLOCK_TAI, &t);
  }
}
BIONIC_BENCHMARK(BM_time_clock_gettime_TAI);

static void BM_time_clock_gettime_unsupported(benchmark::State& state) {
  // Helps measure fast error path, and baseline sys call overhead.
  timespec t;
  while (state.KeepRunning()) {
    clock_gettime(CLOCK_unsupported, &t);
  }
}
BIONIC_BENCHMARK(BM_time_clock_gettime_unsupported);

static void BM_time_clock_getres(benchmark::State& state) {
  // CLOCK_MONOTONIC is required supported in vdso
  timespec t;
  while (state.KeepRunning()) {
    clock_getres(CLOCK_MONOTONIC, &t);
  }
}
BIONIC_BENCHMARK(BM_time_clock_getres);

static void BM_time_clock_getres_syscall(benchmark::State& state) {
  // CLOCK_MONOTONIC is required supported in vdso
  timespec t;
  while (state.KeepRunning()) {
    syscall(__NR_clock_getres, CLOCK_MONOTONIC, &t);
  }
}
BIONIC_BENCHMARK(BM_time_clock_getres_syscall);

static void BM_time_clock_getres_REALTIME(benchmark::State& state) {
  // CLOCK_REALTIME is required supported in vdso
  timespec t;
  while (state.KeepRunning()) {
    clock_getres(CLOCK_REALTIME, &t);
  }
}
BIONIC_BENCHMARK(BM_time_clock_getres_REALTIME);

static void BM_time_clock_getres_BOOTTIME(benchmark::State& state) {
  // CLOCK_BOOTTIME is optionally supported in vdso
  timespec t;
  while (state.KeepRunning()) {
    clock_getres(CLOCK_BOOTTIME, &t);
  }
}
BIONIC_BENCHMARK(BM_time_clock_getres_BOOTTIME);

static void BM_time_clock_getres_TAI(benchmark::State& state) {
  // Helps measure syscall-only supported overhead.
  timespec t;
  while (state.KeepRunning()) {
    clock_getres(CLOCK_TAI, &t);
  }
}
BIONIC_BENCHMARK(BM_time_clock_getres_TAI);

static void BM_time_clock_getres_unsupported(benchmark::State& state) {
  // Helps measure fast error path, and baseline sys call overhead.
  timespec t;
  while (state.KeepRunning()) {
    clock_getres(CLOCK_unsupported, &t);
  }
}
BIONIC_BENCHMARK(BM_time_clock_getres_unsupported);

static void BM_time_gettimeofday(benchmark::State& state) {
  timeval tv;
  while (state.KeepRunning()) {
    gettimeofday(&tv, nullptr);
  }
}
BIONIC_BENCHMARK(BM_time_gettimeofday);

void BM_time_gettimeofday_syscall(benchmark::State& state) {
  timeval tv;
  while (state.KeepRunning()) {
    syscall(__NR_gettimeofday, &tv, nullptr);
  }
}
BIONIC_BENCHMARK(BM_time_gettimeofday_syscall);

void BM_time_time(benchmark::State& state) {
  while (state.KeepRunning()) {
    time(nullptr);
  }
}
BIONIC_BENCHMARK(BM_time_time);

void BM_time_localtime(benchmark::State& state) {
  time_t t = time(nullptr);
  while (state.KeepRunning()) {
    localtime(&t);
  }
}
BIONIC_BENCHMARK(BM_time_localtime);

void BM_time_localtime_r(benchmark::State& state) {
  time_t t = time(nullptr);
  while (state.KeepRunning()) {
    struct tm tm;
    localtime_r(&t, &tm);
  }
}
BIONIC_BENCHMARK(BM_time_localtime_r);
