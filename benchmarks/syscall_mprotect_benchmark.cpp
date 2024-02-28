/*
 * Copyright (C) 2024 The Android Open Source Project
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

#include <sys/mman.h>
#include <sys/syscall.h>

#include <android-base/file.h>
#include <android-base/stringprintf.h>
#include <benchmark/benchmark.h>

#include <unistd.h>

#include "util.h"

struct MprotectParams {
  int prot;
  int64_t size;
};

static void MprotectBenchmarkImpl(benchmark::State& state, const struct MprotectParams& params,
                                  void* addr, int original_prot) {
  for (auto _ : state) {
    state.ResumeTiming();
    if (mprotect(addr, params.size, params.prot) != 0) {
      state.SkipWithError(android::base::StringPrintf("mprotect failed: %m"));
      break;
    }

    state.PauseTiming();
    // Revert back to the original protection
    if (mprotect(addr, params.size, original_prot) != 0) {
      state.SkipWithError(
          android::base::StringPrintf("mprotect failed to revert to original prot: %m"));
      break;
    }
  }
}

static void MprotectBenchmarkWithAnonMmap(benchmark::State& state,
                                          const struct MprotectParams& params, int mmap_prot) {
  void* addr = mmap(nullptr, params.size, mmap_prot, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
  if (addr == MAP_FAILED) {
    state.SkipWithError(android::base::StringPrintf("mmap failed: %m"));
    return;
  }

  /*
   * Guarantee that physical memory pages are allocated for this region to prevent
   * segmentation fault when using mprotect to change permissions.
   */
  if (mmap_prot & PROT_WRITE) {
    MakeAllocationResident(addr, params.size, getpagesize());
  }

  MprotectBenchmarkImpl(state, params, addr, mmap_prot);

  if (munmap(addr, params.size) != 0) {
    state.SkipWithError(android::base::StringPrintf("munmap failed: %m"));
    return;
  }
}

// Read/Write to Read-only transition.
static void BM_syscall_mprotect_read_write_to_read_only(benchmark::State& state) {
  struct MprotectParams params = {
      .prot = PROT_READ,
      .size = state.range(0),
  };
  MprotectBenchmarkWithAnonMmap(state, params, PROT_READ | PROT_WRITE);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mprotect_read_write_to_read_only, "AT_All_PAGE_SIZES");

// Read/Write to non-accessible transition.
static void BM_syscall_mprotect_read_write_to_none(benchmark::State& state) {
  struct MprotectParams params = {
      .prot = PROT_NONE,
      .size = state.range(0),
  };
  MprotectBenchmarkWithAnonMmap(state, params, PROT_READ | PROT_WRITE);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mprotect_read_write_to_none, "AT_All_PAGE_SIZES");

// Read/Write to write-only transition.
static void BM_syscall_mprotect_read_write_to_write_only(benchmark::State& state) {
  struct MprotectParams params = {
      .prot = PROT_WRITE,
      .size = state.range(0),
  };
  MprotectBenchmarkWithAnonMmap(state, params, PROT_READ | PROT_WRITE);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mprotect_read_write_to_write_only, "AT_All_PAGE_SIZES");

// Read-only to non-accessible transition.
static void BM_syscall_mprotect_read_only_to_none(benchmark::State& state) {
  struct MprotectParams params = {
      .prot = PROT_NONE,
      .size = state.range(0),
  };
  MprotectBenchmarkWithAnonMmap(state, params, PROT_READ);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mprotect_read_only_to_none, "AT_All_PAGE_SIZES");

// Read-only to write-only transition.
static void BM_syscall_mprotect_read_only_to_write_only(benchmark::State& state) {
  struct MprotectParams params = {
      .prot = PROT_WRITE,
      .size = state.range(0),
  };
  MprotectBenchmarkWithAnonMmap(state, params, PROT_READ);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mprotect_read_only_to_write_only, "AT_All_PAGE_SIZES");

// Write-only to read-only transition.
static void BM_syscall_mprotect_write_only_to_read_only(benchmark::State& state) {
  struct MprotectParams params = {
      .prot = PROT_READ,
      .size = state.range(0),
  };
  MprotectBenchmarkWithAnonMmap(state, params, PROT_WRITE);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mprotect_write_only_to_read_only, "AT_All_PAGE_SIZES");

// Write-only to non-accessible transition.
static void BM_syscall_mprotect_write_only_to_none(benchmark::State& state) {
  struct MprotectParams params = {
      .prot = PROT_NONE,
      .size = state.range(0),
  };
  MprotectBenchmarkWithAnonMmap(state, params, PROT_WRITE);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mprotect_write_only_to_none, "AT_All_PAGE_SIZES");