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


static int __set_errno(int n)
{
    errno = n;
    return -1;
}

// Note: 'errno' is reserved name

TEST(libc_errno, __set_errno)
{
  { SCOPED_TRACE("reset_errno");
    if (errno != 0)
      errno = 0;
    ASSERT_EQ(0, errno);
  }

  { SCOPED_TRACE("__set_errno()");
    __set_errno(EACCES);
    EXPECT_EQ(EACCES, errno);
    __set_errno(0);
    ASSERT_EQ(0, errno);
  }
}

TEST(libc_errno, __errno)
{
  { SCOPED_TRACE("reset_errno");
    if (errno != 0)
      errno = 0;
    ASSERT_EQ(0, errno);
  }

  { SCOPED_TRACE("__set_errno()");
    errno = EACCES;
    EXPECT_EQ(EACCES, *__errno());
    errno = 0;
    ASSERT_EQ(0, *__errno());
  }
}
