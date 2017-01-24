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

// Our goal is to measure the cost of various C++ atomic operations.
// Android doesn't really control those. But since some of these operations can be quite
// expensive, this may be useful input for development of higher level code.

#include <benchmark/benchmark.h>
#include <atomic>
#include <mutex>

// We time atomic operations separated by a volatile (not atomic!) increment.  This ensures that
// there will be an outstanding memory operation when fences are encountered.

// In most respects, we compute best case values. Since there is only one thread, there are no
// coherence misses.

// We assume that the compiler is not smart enough to optimize away fences in a single-threaded
// program. If that changes, we'll need to add a second thread.

volatile unsigned counter;

std::atomic<int> test_loc(0);

volatile unsigned sink;

std::mutex mtx;

void BM_empty(benchmark::State& state) {
  while (state.KeepRunning()) {
    ++counter;
  }
}
BENCHMARK(BM_empty);

static void BM_load_relaxed(benchmark::State& state) {
  unsigned result = 0;
  while (state.KeepRunning()) {
    result += test_loc.load(std::memory_order_relaxed);
    ++counter;
  }
  sink = result;
}
BENCHMARK(BM_load_relaxed);

static void BM_load_acquire(benchmark::State& state) {
  unsigned result = 0;
  while (state.KeepRunning()) {
    result += test_loc.load(std::memory_order_acquire);
    ++counter;
  }
  sink = result;
}
BENCHMARK(BM_load_acquire);

static void BM_store_release(benchmark::State& state) {
  int i = counter;
  while (state.KeepRunning()) {
    test_loc.store(++i, std::memory_order_release);
    ++counter;
  }
}
BENCHMARK(BM_store_release);

static void BM_store_seq_cst(benchmark::State& state) {
  int i = counter;
  while (state.KeepRunning()) {
    test_loc.store(++i, std::memory_order_seq_cst);
    ++counter;
  }
}
BENCHMARK(BM_store_seq_cst);

static void BM_fetch_add_relaxed(benchmark::State& state) {
  unsigned result = 0;
  while (state.KeepRunning()) {
    result += test_loc.fetch_add(1, std::memory_order_relaxed);
    ++counter;
  }
  sink = result;
}
BENCHMARK(BM_fetch_add_relaxed);

static void BM_fetch_add_seq_cst(benchmark::State& state) {
  unsigned result = 0;
  while (state.KeepRunning()) {
    result += test_loc.fetch_add(1, std::memory_order_seq_cst);
    ++counter;
  }
  sink = result;
}
BENCHMARK(BM_fetch_add_seq_cst);

// The fence benchmarks include a relaxed load to make it much harder to optimize away
// the fence.

static void BM_acquire_fence(benchmark::State& state) {
  unsigned result = 0;
  while (state.KeepRunning()) {
    result += test_loc.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);
    ++counter;
  }
  sink = result;
}
BENCHMARK(BM_acquire_fence);

static void BM_seq_cst_fence(benchmark::State& state) {
  unsigned result = 0;
  while (state.KeepRunning()) {
    result += test_loc.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_seq_cst);
    ++counter;
  }
  sink = result;
}
BENCHMARK(BM_seq_cst_fence);

// For comparison, also throw in a critical section version:

static void BM_fetch_add_cs(benchmark::State& state) {
  unsigned result = 0;
  while (state.KeepRunning()) {
    {
      std::lock_guard<std::mutex> _(mtx);
      result += ++counter;
    }
  }
  sink = result;
}
BENCHMARK(BM_fetch_add_cs);
