/*
 * Copyright (C) 2020 The Android Open Source Project
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
#include "platform/bionic/android_fast_backtrace.h"

// Prevent tail calls inside recurse.
__attribute__((weak, noinline)) size_t nop(size_t val) {
  return val;
}

// Call android_fast_backtrace inside count recurse stack frames.
__attribute__((weak, noinline)) int recurse(int count, uintptr_t* buf, size_t buf_size) {
  if (count != 0) return nop(recurse(count - 1, buf, buf_size));
  return nop(android_fast_backtrace(buf, buf_size));
}

enum { NumFrames = 32 };

static bool CheckFrames(uintptr_t* frames, size_t num_frames) {
  // We expect one recurse frame calling android_fast_backtrace, followed by NumFrames identical recurse
  // frames calling themselves, followed by at least one frame (the first caller of recurse).
  if (num_frames < NumFrames + 2 || frames[0] == frames[1]) {
    return false;
  }

  for (size_t i = 2; i <= NumFrames; ++i) {
    if (frames[i] != frames[1]) {
      return false;
    }
  }

  if (frames[NumFrames] == frames[NumFrames + 1]) {
    return false;
  }

  return true;
}

TEST(android_fast_backtrace, main_thread) {
  size_t size = recurse(NumFrames, 0, 0);

  uintptr_t frames[NumFrames + 2];
  size_t size2 = recurse(NumFrames, frames, NumFrames + 2);
  EXPECT_EQ(size2, size);

  EXPECT_TRUE(CheckFrames(frames, size));
}

static void *BacktraceThread(void *) {
  size_t size = recurse(NumFrames, 0, 0);

  uintptr_t frames[NumFrames + 2];
  size_t size2 = recurse(NumFrames, frames, NumFrames + 2);
  if (size2 != size) {
    return (void*)"size2 != size";
  }

  if (!CheckFrames(frames, size)) {
    return (void*)"CheckFrames failed";
  }
  return nullptr;
}

TEST(android_fast_backtrace, pthread) {
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, BacktraceThread, nullptr));
  void* retval;
  ASSERT_EQ(0, pthread_join(t, &retval));
  EXPECT_EQ(nullptr, reinterpret_cast<char*>(retval));
}
