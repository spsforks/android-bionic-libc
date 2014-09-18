/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <dlfcn.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <unwind.h>

#define noinline __attribute__((noinline))

static _Unwind_Reason_Code FrameCounter(_Unwind_Context* ctx __unused, void* arg) {
  int* count_ptr = reinterpret_cast<int*>(arg);

#if 0
  void* ip = reinterpret_cast<void*>(_Unwind_GetIP(ctx));

  const char* symbol = "<unknown>";
  int offset = 0;

  Dl_info info;
  memset(&info, 0, sizeof(info));
  if (dladdr(ip, &info) != 0) {
    symbol = info.dli_sname;
    if (info.dli_saddr != nullptr) {
      offset = static_cast<int>(reinterpret_cast<char*>(ip) - reinterpret_cast<char*>(info.dli_saddr));
    }
  }

  fprintf(stderr, " #%02d %p %s%+d (%s)\n", *count_ptr, ip, symbol, offset, info.dli_fname ? info.dli_fname : "??");
  fflush(stderr);
#endif

  ++*count_ptr;
  return _URC_NO_REASON;
}

static int noinline unwind_one_frame_deeper() {
  int count = 0;
  _Unwind_Backtrace(FrameCounter, &count);
  return count;
}

TEST(stack_unwinding, easy) {
  int count = 0;
  _Unwind_Backtrace(FrameCounter, &count);
  int deeper_count = unwind_one_frame_deeper();
  ASSERT_EQ(count + 1, deeper_count);
}

static int killer_count = 0;
static int handler_count = 0;
static int handler_one_deeper_count = 0;

static void noinline Handler(int signo __unused) {
  _Unwind_Backtrace(FrameCounter, &handler_count);
  ASSERT_GT(handler_count, killer_count);

  handler_one_deeper_count = unwind_one_frame_deeper();
  ASSERT_EQ(handler_count + 1, handler_one_deeper_count);
}

TEST(stack_unwinding, unwind_through_signal_frame) {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = Handler;
  ASSERT_EQ(0, sigaction(SIGUSR1, &sa, NULL));

  _Unwind_Backtrace(FrameCounter, &killer_count);

  kill(getpid(), SIGUSR1);
}

extern "C" void unwind_through_frame_with_cleanup_function();

// We have to say "DeathTest" here so gtest knows to run this test (which exits)
// in its own process.
TEST(stack_unwinding_DeathTest, unwind_through_frame_with_cleanup_function) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(unwind_through_frame_with_cleanup_function(), ::testing::ExitedWithCode(42), "");
}
