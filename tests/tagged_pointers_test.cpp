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
#include <sys/utsname.h>

#include "utils.h"

#include <bionic/malloc_tagged_pointers.h>

static bool PlatformSupportsTaggedPointers() {
#ifndef __aarch64__
  return false;
#else
  struct utsname buf;
  struct utsname* tagged_buf = static_cast<struct utsname*>(TagPointer(&buf));
  return uname(tagged_buf) == 0;
#endif  // !defined(__aarch64__)
}

TEST(tagged_pointers, check_tagged_pointer_dies) {
  if (!PlatformSupportsTaggedPointers()) {
    GTEST_SKIP() << "Platform doesn't support tagged pointers.";
  }

#ifdef __aarch64__
  void *x = malloc(1);

  // Ensure that `x` has a pointer tag.
  EXPECT_NE(reinterpret_cast<uintptr_t>(x) >> 56, 0);

  x = untag_address(x);
  EXPECT_DEATH(free(x), "Pointer tag for 0x[a-zA-Z0-9]* was truncated");
#endif // defined(__aarch64__)
}
