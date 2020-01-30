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

#include <bionic/mte.h>

__attribute__((no_sanitize("address", "hwaddress")))
static void test_oob() {
  ScopedDisableMTE x;
  std::unique_ptr<int[]> p = std::make_unique<int[]>(4);
  volatile int oob = p[-1];
  (void)oob;
}

TEST(mte_test, ScopedDisableMTE) {
  test_oob();
}
