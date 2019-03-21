/*
 * Copyright (C) 2019 The Android Open Source Project
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
#include <sys/param.h>

TEST(param_test, is_zero_power_of_2) {
  ASSERT_FALSE(powerof2(0));
}

TEST(param_test, is_negative_power_of_2) {
  int32_t i;
  for (i = INT32_MIN; i < 0; i++) {
    ASSERT_FALSE(powerof2(i));
  }
}
