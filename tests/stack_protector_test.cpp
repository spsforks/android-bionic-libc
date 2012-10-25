/*
 * Copyright (C) 2012 The Android Open Source Project
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

/*
 * Contributed by: Intel Corporation
 */

#include <gtest/gtest.h>

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <set>

#ifdef __GLIBC__

// glibc doesn't expose gettid(2).
pid_t gettid() { return syscall(__NR_gettid); }

#endif

#ifdef __i386__

// For x86, bionic and glibc have per-thread stack guard values.

static uint32_t GetGuardFromTls() {
  uint32_t guard;
  asm ("mov %%gs:0x14, %0": "=d" (guard));
  return guard;
}

struct state_t {
  std::set<pid_t> tids;
  std::set<uint32_t> guards;
};

static void* ThreadGuardHelper(void* arg) {
  pid_t tid = gettid();
  uint32_t guard = GetGuardFromTls();

  printf("[thread %d] %%gs:0x14 = 0x%08x\n", tid, guard);

  state_t* state = reinterpret_cast<state_t*>(arg);
  if (state->tids.find(tid) != state->tids.end()) {
    // Duplicate tid. gettid(2) bug? Seeing this would be very upsetting.
    return reinterpret_cast<void*>(1);
  }
#ifndef __GLIBC__
  // glibc uses the same guard for every thread. bionic uses a different guard for each one.
  if (state->guards.find(guard) != state->guards.end()) {
    // Duplicate guard. Our bug. Note this is potentially flaky; we _could_ get the
    // same guard for two threads, but it should be vanishingly unlikely.
    return reinterpret_cast<void*>(2);
  }
#endif
  if (guard == 0) {
    // Uninitialized guard. Our bug. Note this is potentially flaky; we _could_ get
    // four random zero bytes, but it should be vanishingly unlikely.
    return reinterpret_cast<void*>(3);
  }

  state->tids.insert(tid);
  state->guards.insert(guard);

  return NULL;
}

TEST(stack_protector, guard_per_thread) {
  state_t state;
  size_t thread_count = 10;
  for (size_t i = 0; i < thread_count; ++i) {
    pthread_t t;
    ASSERT_EQ(0, pthread_create(&t, NULL, ThreadGuardHelper, &state));
    void* result;
    ASSERT_EQ(0, pthread_join(t, &result));
    ASSERT_EQ(NULL, result);
  }
  ASSERT_EQ(thread_count, state.tids.size());

  // glibc uses the same guard for every thread. bionic uses a different guard for each one.
#ifdef __BIONIC__
  ASSERT_EQ(thread_count, state.guards.size());
#else
  ASSERT_EQ(1U, state.guards.size());
#endif
}

#endif

#ifdef __BIONIC__

// For ARM and MIPS, bionic has a global stack check guard value.

extern "C" void* __stack_chk_guard;

TEST(stack_protector, global_guard) {
  ASSERT_NE(0, gettid());
  ASSERT_NE(0U, reinterpret_cast<uintptr_t>(__stack_chk_guard));
}

#endif
