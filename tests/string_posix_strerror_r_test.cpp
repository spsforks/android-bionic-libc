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

#include <features.h>

#include <string.h>

#include <errno.h>
#include <gtest/gtest.h>

#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
// This test is for the posix version of strerror_r, which returns int.

TEST(string, posix_strerror_r) {
  char buf[256];

  // Valid.
  ASSERT_EQ(0, strerror_r(0, buf, sizeof(buf)));
  ASSERT_STREQ("Success", buf);
  ASSERT_EQ(0, strerror_r(1, buf, sizeof(buf)));
  ASSERT_STREQ("Operation not permitted", buf);

  // Invalid.
  ASSERT_EQ(0, strerror_r(-1, buf, sizeof(buf)));
  ASSERT_STREQ("Unknown error -1", buf);
  ASSERT_EQ(0, strerror_r(1234, buf, sizeof(buf)));
  ASSERT_STREQ("Unknown error 1234", buf);

  // Buffer too small.
  errno = 0;
  memset(buf, 0, sizeof(buf));
  ASSERT_EQ(-1, strerror_r(4567, buf, 2));
  ASSERT_STREQ("U", buf);
  // The POSIX strerror_r sets errno to ERANGE (the GNU one doesn't).
  ASSERT_EQ(ERANGE, errno);
}

#endif
