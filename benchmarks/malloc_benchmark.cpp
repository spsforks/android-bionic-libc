/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <malloc.h>
#include <stdlib.h>

#include <benchmark/benchmark.h>
#include "util.h"

#if defined(__BIONIC__)

enum AllocEnum : uint8_t {
  MALLOC = 0,
  REALLOC = 1,
  FREE = 2,
};

struct MallocEntry {
  AllocEnum type;
  size_t idx;
  size_t size;
  size_t idx2;
};

void BenchmarkMalloc(MallocEntry entries[], size_t total_entries, size_t max_allocs) {
  void* ptrs[max_allocs];

  for (size_t i = 0; i < total_entries; i++) {
    switch (entries[i].type) {
    case MALLOC:
      ptrs[entries[i].idx] = malloc(entries[i].size);
      // Touch at least one byte of the allocation to make sure that
      // PSS for this allocation is counted.
      reinterpret_cast<uint8_t*>(ptrs[entries[i].idx])[0] = 10;
      break;
    case FREE:
      free(ptrs[entries[i].idx]);
      break;
    case REALLOC:
      if (entries[i].idx2 == 0) {
        ptrs[entries[i].idx] = realloc(nullptr, entries[i].size);
      } else {
        ptrs[entries[i].idx] = realloc(ptrs[entries[i].idx2 - 1], entries[i].size);
      }
      // Touch at least one byte of the allocation to make sure that
      // PSS for this allocation is counted.
      reinterpret_cast<uint8_t*>(ptrs[entries[i].idx])[0] = 20;
      break;
    }
  }
}

#include "malloc_sql.h"

static void BM_malloc_sql_trace_decay_time_0(benchmark::State& state) {
  mallopt(M_DECAY_TIME, 0);
  for (auto _ : state) {
    BenchmarkMalloc(g_sql_entries, sizeof(g_sql_entries) / sizeof(MallocEntry),
                    kMaxSqlAllocEntries);
  }
}
BIONIC_BENCHMARK(BM_malloc_sql_trace_decay_time_0);

static void BM_malloc_sql_trace_decay_time_1(benchmark::State& state) {
  mallopt(M_DECAY_TIME, 1);
  for (auto _ : state) {
    BenchmarkMalloc(g_sql_entries, sizeof(g_sql_entries) / sizeof(MallocEntry),
                    kMaxSqlAllocEntries);
  }
}
BIONIC_BENCHMARK(BM_malloc_sql_trace_decay_time_1);

#endif
