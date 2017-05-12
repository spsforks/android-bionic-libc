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

#include <err.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/user.h>
#include <unistd.h>

#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <vector>

#include <android-base/macros.h>

#include "utils.h"

using namespace std::chrono_literals;

static size_t GetMappingSize() {
  std::vector<map_record> maps;
  if (!Maps::parse_maps(&maps)) {
    err(1, "failed to parse maps");
  }

  size_t result = 0;
  for (const map_record& map : maps) {
    result += map.addr_end - map.addr_start;
  }

  return result;
}

#define LEAK_TEST(test_case_name, test_name)                                                 \
  static void __leak_test__##test_case_name##__##test_name();                                \
  TEST(test_case_name, test_name) {                                                          \
    auto previous_size = GetMappingSize();                                                   \
    __leak_test__##test_case_name##__##test_name();                                          \
    auto current_size = GetMappingSize();                                                    \
    if (current_size > previous_size) {                                                      \
      FAIL() << "increase in process map size: " << previous_size << " -> " << current_size; \
    }                                                                                        \
  }                                                                                          \
  static void __leak_test__##test_case_name##__##test_name()

LEAK_TEST(leak, smoke) {
  // Do nothing.
}

LEAK_TEST(leak, xfail) {
  UNUSED(mmap(nullptr, PAGE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
}

// http://b/36045112
LEAK_TEST(pthread_leak, join) {
  for (int i = 0; i < 100; ++i) {
    pthread_t thread;
    ASSERT_EQ(0, pthread_create(&thread, nullptr, [](void*) -> void* { return nullptr; }, nullptr));
    ASSERT_EQ(0, pthread_join(thread, nullptr));
  }
}

static size_t GetMappingSizeAfterDetach() {
  constexpr int thread_count = 100;
  static pthread_barrier_t barrier;
  EXPECT_EQ(pthread_barrier_init(&barrier, nullptr, thread_count + 1), 0);

  // Start child threads.
  pid_t tids[thread_count];
  for (int i = 0; i < thread_count; ++i) {
    pthread_t thread;
    const auto thread_function = +[](void* tid) -> void* {
      *(static_cast<pid_t*>(tid)) = gettid();
      pthread_barrier_wait(&barrier);
      return nullptr;
    };
    EXPECT_EQ(pthread_create(&thread, nullptr, thread_function, &tids[i]), 0);
    EXPECT_EQ(pthread_detach(thread), 0);
  }

  pthread_barrier_wait(&barrier);

  // Wait until all children have exited.
  bool alive = true;
  while (alive) {
    alive = false;
    for (int i = 0; i < thread_count; ++i) {
      if (tids[i] != 0) {
        if (kill(tids[i], 0) == 0) {
          alive = true;
        } else {
          EXPECT_EQ(errno, ESRCH);
          tids[i] = 0;  // Skip in next loop.
        }
      }
    }
  }

  EXPECT_EQ(pthread_barrier_destroy(&barrier), 0);

  return GetMappingSize();
}


// http://b/36045112
TEST(pthread_leak, detach) {
  auto first_size = GetMappingSizeAfterDetach();
  ASSERT_NE(first_size, 0U);
  auto second_size = GetMappingSizeAfterDetach();
  ASSERT_NE(second_size, 0U);
  if (first_size < second_size) {
    FAIL() << "mismatched process map size: " << first_size << " -> " << second_size;
  }
}


