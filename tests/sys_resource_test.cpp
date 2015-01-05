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
#include <errno.h>
#include <string.h>
#include <sys/resource.h>

TEST(sys_resource, smoke) {
#if defined(__LP64__) || defined(__GLIBC__)
  ASSERT_EQ(sizeof(rlimit), sizeof(rlimit64));
  ASSERT_EQ(8U, sizeof(rlim_t));
#else
  ASSERT_NE(sizeof(rlimit), sizeof(rlimit64));
  ASSERT_EQ(4U, sizeof(rlim_t));
#endif

  // Hard limit is not changed in the test, as it can only go down for non-root users.
  // Soft limit should be set between 0 and hard limit.
  // Read with getrlimit, getrlimit64, and prlimit64.
  // (prlimit is prlimit64 on LP64 and unimplemented on 32-bit.)
  rlimit l32;
  rlimit64 l64;
  rlimit64 pr_l64;
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, NULL, &pr_l64));
  ASSERT_EQ(l64.rlim_cur, pr_l64.rlim_cur);
  if (l64.rlim_cur == RLIM64_INFINITY) {
    ASSERT_EQ(RLIM_INFINITY, l32.rlim_cur);
  } else {
    ASSERT_EQ(l64.rlim_cur, l32.rlim_cur);
  }
  ASSERT_EQ(l64.rlim_max, pr_l64.rlim_max);
  if (l64.rlim_max == RLIM64_INFINITY) {
    ASSERT_EQ(RLIM_INFINITY, l32.rlim_max);
  } else {
    ASSERT_EQ(l64.rlim_max, l32.rlim_max);
  }

  // Write with setrlimit and read back with everything.
  l32.rlim_cur = l32.rlim_max;
  ASSERT_EQ(0, setrlimit(RLIMIT_CORE, &l32)) << strerror(errno);
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, NULL, &pr_l64));
  ASSERT_EQ(l32.rlim_max, l32.rlim_cur);
  if (l32.rlim_cur == RLIM_INFINITY) {
    ASSERT_EQ(RLIM64_INFINITY, l64.rlim_cur);
  } else {
    ASSERT_EQ(l32.rlim_cur, l64.rlim_cur);
  }
  ASSERT_EQ(l64.rlim_cur, pr_l64.rlim_cur);

  // Write with setrlimit64 and read back with everything.
  l64.rlim_cur = 0;
  ASSERT_EQ(0, setrlimit64(RLIMIT_CORE, &l64)) << strerror(errno);
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, NULL, &pr_l64));
  ASSERT_EQ(0U, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, pr_l64.rlim_cur);

  // Write with prlimit64 and read back with everything.
  pr_l64.rlim_cur = pr_l64.rlim_max;
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, &pr_l64, NULL)) << strerror(errno);
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, NULL, &pr_l64));
  ASSERT_EQ(pr_l64.rlim_max, pr_l64.rlim_cur);
  ASSERT_EQ(pr_l64.rlim_cur, l64.rlim_cur);
  if (pr_l64.rlim_cur == RLIM64_INFINITY) {
    ASSERT_EQ(RLIM_INFINITY, l32.rlim_cur);
  } else {
    ASSERT_EQ(pr_l64.rlim_cur, l32.rlim_cur);
  }
}
