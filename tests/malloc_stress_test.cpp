/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <inttypes.h>
#include <malloc.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <thread>
#include <vector>

#include <android-base/strings.h>
#if defined(__BIONIC__)
#include <meminfo/procmeminfo.h>
#include <procinfo/process_map.h>
#endif

#if defined(__BIONIC__)
class Stats {
 public:
  Stats() = default;
  ~Stats() = default;

  void Gather();

  void Print(bool print_min_max = false);

 private:
  uint64_t rss_bytes_;
  uint64_t vss_bytes_;

  uint64_t rss_min_bytes_ = UINT64_MAX;
  uint64_t rss_max_bytes_ = 0;
  uint64_t vss_min_bytes_ = UINT64_MAX;
  uint64_t vss_max_bytes_ = 0;
};

void Stats::Gather() {
  android::meminfo::ProcMemInfo proc_mem(getpid());
  const std::vector<android::meminfo::Vma>& maps = proc_mem.MapsWithoutUsageStats();
  rss_bytes_ = 0;
  vss_bytes_ = 0;
  for (auto& vma : maps) {
    if (vma.name == "[anon:libc_malloc]" || android::base::StartsWith(vma.name, "[anon:scudo:") ||
        android::base::StartsWith(vma.name, "[anon:GWP-ASan")) {
      android::meminfo::Vma update_vma(vma);
      ASSERT_TRUE(proc_mem.FillInVmaStats(update_vma));
      rss_bytes_ += update_vma.usage.rss;
      vss_bytes_ += update_vma.usage.vss;
    }
  }
  if (rss_bytes_ < rss_min_bytes_) {
    rss_min_bytes_ = rss_bytes_;
  }
  if (rss_bytes_ > rss_max_bytes_) {
    rss_max_bytes_ = rss_bytes_;
  }
  if (vss_bytes_ < vss_min_bytes_) {
    vss_min_bytes_ = vss_bytes_;
  }
  if (vss_bytes_ > vss_max_bytes_) {
    vss_max_bytes_ = vss_bytes_;
  }
}

void Stats::Print(bool print_min_max) {
  printf("RSS %" PRIu64 " %0.2fMB\n", rss_bytes_, rss_bytes_ / (1024.0 * 1024.0));
  if (print_min_max) {
    printf("  Min %" PRIu64 " %0.2fMB\n", rss_min_bytes_, rss_min_bytes_ / (1024.0 * 1024.0));
    printf("  Max %" PRIu64 " %0.2fMB\n", rss_max_bytes_, rss_max_bytes_ / (1024.0 * 1024.0));
  }
  printf("VSS %" PRIu64 " %0.2f MB\n", vss_bytes_, vss_bytes_ / (1024.0 * 1024.0));
  if (print_min_max) {
    printf("  Min %" PRIu64 " %0.2fMB\n", vss_min_bytes_, vss_min_bytes_ / (1024.0 * 1024.0));
    printf("  Max %" PRIu64 " %0.2fMB\n", vss_max_bytes_, vss_max_bytes_ / (1024.0 * 1024.0));
  }
}
#endif

TEST(malloc_stress, multiple_threads_forever) {
  constexpr size_t kMaxThreads = 256;
  constexpr size_t kAllocSize = 4096;
#if defined(__BIONIC__)
  Stats stats;
  ASSERT_EQ(1, mallopt(M_DECAY_TIME, 1));
#endif
  uint64_t mallinfo_min = UINT64_MAX;
  uint64_t mallinfo_max = 0;
  for (size_t i = 0; ; i++) {
    printf("Pass %zu\n", i);

    std::vector<std::thread*> threads;
    for (size_t i = 0; i < kMaxThreads; i++) {
      threads.push_back(new std::thread([]() {
        void* buf = malloc(4096);
        if (buf == nullptr) {
          printf("Failed to allocate memory\n");
          _exit(1);
        }
        memset(buf, 0, kAllocSize);
        sleep(1);
        free(buf);
      }));
    }

    for (auto thread : threads) {
      thread->join();
      delete thread;
    }
    threads.clear();

#if defined(__BIONIC__)
    stats.Gather();
    stats.Print(true);
#endif

    size_t mallinfo_bytes = mallinfo().uordblks;
    if (mallinfo_bytes < mallinfo_min) {
      mallinfo_min = mallinfo_bytes;
    }
    if (mallinfo_bytes > mallinfo_max) {
      mallinfo_max = mallinfo_bytes;
    }
    printf("Allocated memory %zu %0.2fMB\n", mallinfo_bytes, mallinfo_bytes / (1024.0 * 1024.0));
    printf("  Min %" PRIu64 " %0.2fMB\n", mallinfo_min, mallinfo_min / (1024.0 * 1024.0));
    printf("  Max %" PRIu64 " %0.2fMB\n", mallinfo_max, mallinfo_max / (1024.0 * 1024.0));
  }
}

#if defined(__BIONIC__)
#include <map>
#include <unordered_map>

template <size_t StructSize>
void MapBenchmark(size_t step, size_t max) {
  struct SizedStruct {
    char bytes[StructSize];
  } __attribute__((packed));
  std::map<uint64_t, SizedStruct> map;

  static_assert(sizeof(SizedStruct) == StructSize);
  printf("Struct size %zu\n", sizeof(SizedStruct));
  Stats stats;
  for (size_t i = 1; i <= max; i++) {
    map[i] = SizedStruct{};
    if ((i % step) == 0) {
#if 0
      printf("Num elements %zu\n", i);
      stats.Gather();
      stats.Print(false);
#endif
    }
  }
  printf("Total number of elements added: %zu\n", max);
  stats.Gather();
  stats.Print(false);
};

TEST(malloc_stress, map_memory_8) {
  MapBenchmark<8>(100000, 4000000);
}

TEST(malloc_stress, map_memory_16) {
  MapBenchmark<16>(100000, 2000000);
}

TEST(malloc_stress, map_memory_32) {
  MapBenchmark<32>(100000, 2000000);
}

TEST(malloc_stress, map_memory_64) {
  MapBenchmark<64>(100000, 2000000);
}

TEST(malloc_stress, map_memory_96) {
  MapBenchmark<96>(100000, 1000000);
}

TEST(malloc_stress, map_memory_128) {
  MapBenchmark<128>(20000, 1000000);
}

TEST(malloc_stress, map_memory_256) {
  MapBenchmark<256>(20000, 500000);
}

TEST(malloc_stress, map_memory_512) {
  MapBenchmark<512>(20000, 200000);
}

template <size_t StructSize>
void UnorderedMapBenchmark(size_t step, size_t max) {
  struct SizedStruct {
    char bytes[StructSize];
  } __attribute__((packed));
  std::unordered_map<uint64_t, SizedStruct> map;

  static_assert(sizeof(SizedStruct) == StructSize);
  printf("Struct size %zu\n", sizeof(SizedStruct));
  Stats stats;
  for (size_t i = 1; i <= max; i++) {
    map[i] = SizedStruct{};
    if ((i % step) == 0) {
#if 0
      printf("Num elements %zu\n", i);
      stats.Gather();
      stats.Print(false);
#endif
    }
  }
  printf("Total number of elements added: %zu\n", max);
  stats.Gather();
  stats.Print(false);
};

TEST(malloc_stress, unordered_map_memory_8) {
  UnorderedMapBenchmark<8>(100000, 4000000);
}

TEST(malloc_stress, unordered_map_memory_16) {
  UnorderedMapBenchmark<16>(100000, 2000000);
}

TEST(malloc_stress, unordered_map_memory_32) {
  UnorderedMapBenchmark<32>(100000, 2000000);
}

TEST(malloc_stress, unordered_map_memory_64) {
  UnorderedMapBenchmark<64>(100000, 2000000);
}

TEST(malloc_stress, unordered_map_memory_96) {
  UnorderedMapBenchmark<96>(100000, 1000000);
}

TEST(malloc_stress, unordered_map_memory_128) {
  UnorderedMapBenchmark<128>(20000, 1000000);
}

TEST(malloc_stress, unordered_map_memory_256) {
  UnorderedMapBenchmark<256>(20000, 500000);
}

TEST(malloc_stress, unordered_map_memory_512) {
  UnorderedMapBenchmark<512>(20000, 200000);
}
#endif
