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

#include <fnmatch.h>


TEST(fnmatch, fnmatch)
{
  const char *pattern0 = "The*";
  const char *pattern1 = "*";
  const char *pattern2 = "the*";
  const char *filename = "TheFooBar.bin";

  { SCOPED_TRACE("fnmatch()");
    EXPECT_TRUE( fnmatch(pattern0, filename, FNM_CASEFOLD) == 0 );
    EXPECT_TRUE( fnmatch(pattern1, filename, FNM_CASEFOLD) == 0 );
    EXPECT_TRUE( fnmatch(pattern2, filename, FNM_CASEFOLD) == 0 );
    EXPECT_FALSE( fnmatch(pattern2, filename, 0) == 0 );
  }
}
