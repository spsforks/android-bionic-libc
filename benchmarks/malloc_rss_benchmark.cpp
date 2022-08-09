/*
 * Copyright (C) 2022 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <malloc.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <vector>

#include <android-base/strings.h>
#if defined(__BIONIC__)
#include <malloc.h>
#include <meminfo/procmeminfo.h>
#include <procinfo/process_map.h>
#endif

constexpr size_t kMaxThreads = 8;
// The max number of bytes can be allocated by a thread. Note that Scudo has the
// 256 MB limit for each size-class in primary allocator. Be aware of the amount
// of memory allocated won't exceed this limit.
constexpr size_t kMaxBytes = 1 << 24;
// The minimum size-class in an allocator.
constexpr size_t kMinSizeClass = 8;
constexpr size_t kMaxLen = kMaxBytes / kMinSizeClass;
uintptr_t MemPool[kMaxThreads][kMaxLen];

void dirtyMem(uintptr_t ptr, size_t bytes) {
  memset(reinterpret_cast<void*>(ptr), 1U, bytes);
}

void ThreadTask(int id, size_t allocSize) {
  // In the following, we will first allocate blocks with amount kMaxBytes
  // memory and release all of them in random order. In the end, we will do
  // another round of allocation until it reaches 1/10 kMaxBytes.

  // Total number of blocks
  const size_t maxCounts = kMaxBytes / allocSize;
  // The number of blocks in the end
  const size_t finalCounts = maxCounts / 10;

  // Allocate `maxCounts` blocks and dirty the blocks.
  for (size_t i = 0; i < maxCounts; ++i) {
    MemPool[id][i] = reinterpret_cast<uintptr_t>(malloc(allocSize));
    if (MemPool[id][i] == 0) {
      std::cout << "Allocation failure."
                   "Please consider reduce the number of threads"
                << std::endl;
      return;
    }
    dirtyMem(MemPool[id][i], allocSize);
  }

  // Release all the blocks in random order.
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(MemPool[id], MemPool[id] + maxCounts, std::default_random_engine(seed));
  for (size_t i = 0; i < maxCounts; ++i) free(reinterpret_cast<void*>(MemPool[id][i]));

  // Allocate blocks until it reaches `finalCounts`.
  for (size_t i = 0; i < finalCounts; ++i) {
    MemPool[id][i] = reinterpret_cast<uintptr_t>(malloc(allocSize));
    dirtyMem(MemPool[id][i], allocSize);
  }
}

void StressSizeClass(size_t numThreads, size_t allocSize) {
  // We would like to see the minimum memory usage under aggressive page
  // releasing.
  mallopt(M_DECAY_TIME, 0);

  std::thread* threads[kMaxThreads];
  for (size_t i = 0; i < numThreads; ++i) threads[i] = new std::thread(ThreadTask, i, allocSize);

  for (size_t i = 0; i < numThreads; ++i) {
    threads[i]->join();
    delete threads[i];
  }

  // Do an explicit purge to ensure we will be more likely to get the actual
  // in-used memory.
  mallopt(M_PURGE, 0);

  android::meminfo::ProcMemInfo proc_mem(getpid());
  const std::vector<android::meminfo::Vma>& maps = proc_mem.MapsWithoutUsageStats();
  uint64_t rss_bytes = 0;
  uint64_t vss_bytes = 0;

  for (auto& vma : maps) {
    if (vma.name == "[anon:libc_malloc]" || android::base::StartsWith(vma.name, "[anon:scudo:") ||
        android::base::StartsWith(vma.name, "[anon:GWP-ASan")) {
      android::meminfo::Vma update_vma(vma);
      if (!proc_mem.FillInVmaStats(update_vma)) {
        std::cout << "Failed to parse VMA" << std::endl;
        exit(1);
      }
      rss_bytes += update_vma.usage.rss;
      vss_bytes += update_vma.usage.vss;
    }
  }

  std::cout << "RSS: " << rss_bytes / (1024.0 * 1024.0) << " MB" << std::endl;
  std::cout << "VSS: " << vss_bytes / (1024.0 * 1024.0) << " MB" << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " $NUM_THREADS $ALLOC_SIZE" << std::endl;
    exit(1);
  }

  size_t numThreads = atoi(argv[1]);
  size_t allocSize = atoi(argv[2]);

  if (numThreads > kMaxThreads) {
    std::cerr << "The max number of threads is " << kMaxThreads << std::endl;
    exit(1);
  }

  std::cout << "Running " << allocSize << "-bytes allocation benchmark with " << numThreads
            << " thread(s)" << std::endl;

  StressSizeClass(numThreads, allocSize);
}
