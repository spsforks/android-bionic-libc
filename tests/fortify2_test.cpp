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

#undef _FORTIFY_SOURCE
#define _FORTIFY_SOURCE 2
#define FORTIFY_TEST(test_name) TEST(Fortify2_DeathTest, test_name)

// All fortify source level 1 tests should pass.
#include "fortify1_test.cpp"

// Start fortify level 2 tests

struct foo {
  char a[10];
  char b[10];
};

FORTIFY_TEST(strncpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  struct foo myfoo;
  int copy_amt = atoi("11");
  ASSERT_EXIT(strncpy(myfoo.a, "01234567890", copy_amt),
              testing::KilledBySignal(SIGSEGV), "");
}
