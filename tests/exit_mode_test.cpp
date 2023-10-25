/*
 * Copyright (C) 2014 The Android Open Source Project
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

#if defined(__BIONIC__)
#include <android/exit_mode.h>
#endif

#include <signal.h>
#include <stdlib.h>

struct Bomb {
  explicit Bomb(int signal) : signal(signal) {}
  ~Bomb() {
    if (signal != 0) raise(signal);
  }

  void defuse() { signal = 0; }

  int signal;
};

static std::optional<Bomb> bomb;
thread_local std::optional<Bomb> thread_local_bomb;

TEST(exit_mode, normal_exit) {
  ASSERT_EXIT(exit(123), testing::ExitedWithCode(123), "");
}

TEST(exit_mode, explosive_exit) {
  bomb.emplace(SIGSEGV);
  ASSERT_EXIT(exit(123), testing::KilledBySignal(SIGSEGV), "");
  bomb->defuse();
}

TEST(exit_mode, explosive_thread_local_exit) {
  thread_local_bomb.emplace(SIGBUS);
  bomb.emplace(SIGSEGV);
  ASSERT_EXIT(exit(123), testing::KilledBySignal(SIGBUS), "");
  bomb->defuse();
  thread_local_bomb->defuse();
}

TEST(exit_mode, skip_thread_local_destructors) {
#if defined(__BIONIC__)
  android_set_exit_mode(ANDROID_EXIT_MODE_SKIP_THREAD_DESTRUCTORS);
  thread_local_bomb.emplace(SIGBUS);
  bomb.emplace(SIGSEGV);
  ASSERT_EXIT(exit(123), testing::KilledBySignal(SIGSEGV), "");
  bomb->defuse();
  thread_local_bomb->defuse();
#endif
}

TEST(exit_mode, skip_global_destructors) {
#if defined(__BIONIC__)
  android_set_exit_mode(ANDROID_EXIT_MODE_SKIP_GLOBAL_DESTRUCTORS);
  bomb.emplace(SIGSEGV);
  ASSERT_EXIT(exit(123), testing::ExitedWithCode(123), "");
  bomb->defuse();
#endif
}

TEST(exit_mode, skip_all_destructors) {
#if defined(__BIONIC__)
  static_assert(ANDROID_EXIT_MODE_SKIP_DESTRUCTORS == (ANDROID_EXIT_MODE_SKIP_THREAD_DESTRUCTORS |
                                                       ANDROID_EXIT_MODE_SKIP_GLOBAL_DESTRUCTORS));
  android_set_exit_mode(ANDROID_EXIT_MODE_SKIP_DESTRUCTORS);
  thread_local_bomb.emplace(SIGBUS);
  bomb.emplace(SIGSEGV);
  ASSERT_EXIT(exit(123), testing::ExitedWithCode(123), "");
  bomb->defuse();
  thread_local_bomb->defuse();
#endif
}

TEST(exit_mode, abort) {
#if defined(__BIONIC__)
  android_set_exit_mode(ANDROID_EXIT_MODE_ABORT);
  thread_local_bomb.emplace(SIGBUS);
  bomb.emplace(SIGSEGV);
  ASSERT_EXIT(exit(123), testing::KilledBySignal(SIGABRT),
              R"(exit\(\) called in multithreaded process, aborting instead.)");

  android_set_exit_mode(ANDROID_EXIT_MODE_NORMAL);
  bomb->defuse();
  thread_local_bomb->defuse();
#endif
}

TEST(exit_mode, warn) {
#if defined(__BIONIC__)
  // Tombstone generation not tested because it's difficult, just test that we don't abort.
  android_set_exit_mode(ANDROID_EXIT_MODE_WARN);
  ASSERT_EXIT(exit(123), testing::ExitedWithCode(123), "");
  android_set_exit_mode(ANDROID_EXIT_MODE_NORMAL);
#endif
}

TEST(exit_mode, warn_abort) {
#if defined(__BIONIC__)
  // Abort should win if you specify everything.
  android_set_exit_mode(static_cast<android_exit_mode>(~0));
  ASSERT_EXIT(exit(123), testing::KilledBySignal(SIGABRT),
              R"(exit\(\) called in multithreaded process, aborting instead.)");
  android_set_exit_mode(ANDROID_EXIT_MODE_NORMAL);
#endif
}
