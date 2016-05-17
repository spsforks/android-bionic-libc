/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <stdio.h>

#include <gtest/gtest.h>

#include "backtrace.h"
#include "log_fake.h"

#include <thread>

class MallocDebugBacktraceTest : public ::testing::Test {
 protected:
  void SetUp() override {
    backtrace_startup();
  }

  void TearDown() override {
    backtrace_shutdown();
  }
};

void level_four() {
  uintptr_t frames[16];
  size_t num_frames = backtrace_get(frames, 16);
  ASSERT_NE(0U, num_frames);
  //printf("Num frames %zu\n", num_frames);
#if 0
  for (size_t i = 0; i < num_frames; i++) {
    printf("%zu: %p\n", i, (void*)frames[i]);
  }
#endif

// ASSERT_STREQ("", getFakeLogBuf().c_str());
// ASSERT_STREQ("", getFakeLogPrint().c_str());
}

void level_three() {
  level_four();
}

void level_two() {
  level_three();
}

void level_one() {
  level_two();
}

TEST_F(MallocDebugBacktraceTest, general) {
  level_one();
}

TEST_F(MallocDebugBacktraceTest, threads) {
  signal(SIGSEGV, SIG_DFL);
  for (size_t i = 0; i < 4096; i++) {
    std::thread thread([](){
      level_one();
    });
    thread.detach();
  }
}

#include <time.h>

#define NS_PER_SEC 1000000000

// Get the current time in nanoseconds.
uint64_t nanoTime() {
  struct timespec t;

  t.tv_sec = t.tv_nsec = 0;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return static_cast<uint64_t>(t.tv_sec) * NS_PER_SEC + t.tv_nsec;
}

#include <sched.h>

bool raisePriorityAndLock(int cpu_to_lock) {
  cpu_set_t cpuset;

  if (setpriority(PRIO_PROCESS, 0, -20)) {
    perror("Unable to raise priority of process.\n");
    return false;
  }

  CPU_ZERO(&cpuset);
  if (sched_getaffinity(0, sizeof(cpuset), &cpuset) != 0) {
    perror("sched_getaffinity failed");
    return false;
  }

  if (cpu_to_lock < 0) {
    // Lock to the last active core we find.
    for (int i = 0; i < CPU_SETSIZE; i++) {
      if (CPU_ISSET(i, &cpuset)) {
        cpu_to_lock = i;
      }
    }
  } else if (!CPU_ISSET(cpu_to_lock, &cpuset)) {
    printf("Cpu %d does not exist.\n", cpu_to_lock);
    return false;
  }

  if (cpu_to_lock < 0) {
    printf("Cannot find any valid cpu to lock.\n");
    return false;
  }

  CPU_ZERO(&cpuset);
  CPU_SET(cpu_to_lock, &cpuset);
  if (sched_setaffinity(0, sizeof(cpuset), &cpuset) != 0) {
    perror("sched_setaffinity failed");
    return false;
  }

  return true;
}

TEST_F(MallocDebugBacktraceTest, benchmark) {
  raisePriorityAndLock(1);
  for (size_t i = 0; i < 1000; i++) {
    level_one();
  }

  uint64_t start = nanoTime();
  for (size_t i = 0; i < 1000000; i++) {
    level_one();
  }
  uint64_t total = nanoTime() - start;

  printf("%.06f seconds\n", (double)total/1000000000);
}
