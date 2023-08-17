/*
 * Copyright (C) 2023 The Android Open Source Project
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

#include "util.h"

static size_t page_sz = getpagesize();

// mmap syscall tests
static void MmapTest(benchmark::State& state, int prot, int flags, void* area, int fd) {
  size_t alloc_sz = state.range(0);
  char* addr;

  while (state.KeepRunning()) {
    addr = (char*)mmap(area, alloc_sz, prot, flags, fd, 0);
    if (addr == MAP_FAILED) {
      state.SkipWithError(android::base::StringPrintf("mmap failed: %s", strerror(errno)).c_str());
      break;
    }
    if (prot & PROT_WRITE) {
      size_t page_cnt = (alloc_sz + page_sz - 1) / page_sz;
      static char val = 0;
      size_t i;

      // populate allocated memory
      for (i = 0; i < page_cnt; i++) {
        addr[i * page_sz] = val++;
      }
    }
    if (munmap(addr, alloc_sz) != 0) {
      state.SkipWithError(
          android::base::StringPrintf("munmap failed: %s", strerror(errno)).c_str());
      break;
    }
  }
}

static void MmapFixedTest(benchmark::State& state, int prot, int flags, int fd) {
  size_t alloc_sz = state.range(0);
  char* area;

  // allocate a 3x larger area
  area = (char*)mmap(0, alloc_sz * 3, prot, flags, fd, 0);
  if (area == MAP_FAILED) {
    state.SkipWithError(android::base::StringPrintf("mmap failed1: %s", strerror(errno)).c_str());
    return;
  }
  // mmap at the beginning of the area
  MmapTest(state, prot, flags, area, fd);
  // mmap in the middle of the area
  MmapTest(state, prot, flags, area + alloc_sz, fd);
  // mmap at the end of the area
  MmapTest(state, prot, flags, area + 2 * alloc_sz, fd);

  if (munmap(area, alloc_sz * 3) != 0) {
    state.SkipWithError(android::base::StringPrintf("munmap failed: %s", strerror(errno)).c_str());
    return;
  }
}

static void MmapFileTest(benchmark::State& state, int prot, int flags) {
  size_t alloc_sz = state.range(0);
  bool fixed = flags & MAP_FIXED;
  TemporaryFile tf;
  int fd;

  fd = open(tf.path, O_RDWR | O_CREAT, 0666);
  if (fd < 0) {
    state.SkipWithError(android::base::StringPrintf("open failed: %s", strerror(errno)).c_str());
    return;
  }
  if (prot & PROT_WRITE) {
    if (ftruncate(fd, fixed ? alloc_sz * 3 : alloc_sz)) {
      state.SkipWithError(
          android::base::StringPrintf("ftruncate failed: %s", strerror(errno)).c_str());
      goto cleanup;
    }
  }
  if (flags & MAP_FIXED) {
    MmapFixedTest(state, prot, flags, fd);
  } else {
    MmapTest(state, prot, flags, NULL, fd);
  }
cleanup:
  close(fd);
  remove(tf.path);
}

// anon mmap
static void BM_syscall_mmap_anon_rw(benchmark::State& state) {
  MmapTest(state, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, NULL, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_rw, "AT_COMMON_SIZES");

static void BM_syscall_mmap_anon_noreserve(benchmark::State& state) {
  MmapTest(state, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, NULL, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_noreserve, "AT_COMMON_SIZES");

static void BM_syscall_mmap_anon_none(benchmark::State& state) {
  MmapTest(state, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, NULL, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_none, "AT_COMMON_SIZES");

// anon fixed mmap
static void BM_syscall_mmap_anon_rw_fixed(benchmark::State& state) {
  MmapFixedTest(state, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_rw_fixed, "AT_COMMON_SIZES");

static void BM_syscall_mmap_anon_none_fixed(benchmark::State& state) {
  MmapFixedTest(state, PROT_NONE, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_none_fixed, "AT_COMMON_SIZES");

// file mmap
static void BM_syscall_mmap_file_rd_priv(benchmark::State& state) {
  MmapFileTest(state, PROT_READ, MAP_PRIVATE);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_file_rd_priv, "AT_COMMON_SIZES");

static void BM_syscall_mmap_file_rw_shared(benchmark::State& state) {
  MmapFileTest(state, PROT_READ | PROT_WRITE, MAP_SHARED);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_file_rw_shared, "AT_COMMON_SIZES");

// file fixed mmap
static void BM_syscall_mmap_file_rw_priv_fixed(benchmark::State& state) {
  MmapFileTest(state, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_file_rw_priv_fixed, "AT_COMMON_SIZES");
