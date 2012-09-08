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

#include <gtest/gtest.h>

#include <errno.h>
#include <string.h>

TEST(string, strerror) {
  // Valid.
  ASSERT_STREQ("Success", strerror(0));
  ASSERT_STREQ("Operation not permitted", strerror(1));

  // Invalid.
  ASSERT_STREQ("Unknown error 4294967295", strerror(-1));
  ASSERT_STREQ("Unknown error 1234", strerror(1234));
}

#if __BIONIC__ // glibc's strerror_r doesn't even have the same signature as the POSIX one.
TEST(string, strerror_r) {
  char buf[256];

  // Valid.
  ASSERT_EQ(0, strerror_r(0, buf, sizeof(buf)));
  ASSERT_STREQ("Success", buf);
  ASSERT_EQ(0, strerror_r(1, buf, sizeof(buf)));
  ASSERT_STREQ("Operation not permitted", buf);

  // Invalid.
  ASSERT_EQ(0, strerror_r(-1, buf, sizeof(buf)));
  ASSERT_STREQ("Unknown error 4294967295", buf);
  ASSERT_EQ(0, strerror_r(1234, buf, sizeof(buf)));
  ASSERT_STREQ("Unknown error 1234", buf);

  // Buffer too small.
  ASSERT_EQ(-1, strerror_r(0, buf, 2));
  ASSERT_EQ(ERANGE, errno);
}
#endif

TEST(string, strsignal) {
  // A regular signal.
  ASSERT_STREQ("Hangup", strsignal(1));

  // A real-time signal.
  // TODO: why is our SIGRTMIN
#ifdef __GLIBC__
  ASSERT_STREQ("Real-time signal 14", strsignal(48));
#else
  ASSERT_STREQ("Real-time signal 16", strsignal(48));
#endif

  // Errors.
  ASSERT_STREQ("Unknown signal -1", strsignal(-1)); // Too small.
  ASSERT_STREQ("Unknown signal 0", strsignal(0)); // Still too small.
  ASSERT_STREQ("Unknown signal 1234", strsignal(1234)); // Too large.
}
