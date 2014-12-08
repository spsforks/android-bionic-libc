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

#ifndef _BIONIC_TESTS_BIONIC_GTEST_H
#define _BIONIC_TESTS_BIONIC_GTEST_H

#include <gtest/gtest.h>

// Type and function declared in namespace bionic_gtest are not intented to be used directly.
namespace bionic_gtest {

void BionicSetDeadlineInfo(const char* testcase_name, const char* test_name, int deadline_in_ms);

class BionicDeadLineInfo {
 public:
  BionicDeadLineInfo(const char* testcase_name, const char* test_name, int deadline_in_ms) {
    BionicSetDeadlineInfo(testcase_name, test_name, deadline_in_ms);
  }
};

} // namespace bionic_gtest

// Use TEST_DEADLINE() instead of TEST() to add deadline info to test.
#define TEST_DEADLINE(testcase_name, test_name, deadline_in_ms) \
  static bionic_gtest::BionicDeadLineInfo testcase_name##_##test_name##_deadline_info(#testcase_name, #test_name, deadline_in_ms); \
  TEST(testcase_name, test_name)

// Use TEST_F_DEADLINE() instead of TEST_F() to add deadline info to test.
#define TEST_F_DEADLINE(test_fixture, test_name, deadline_in_ms) \
  static bionic_gtest::BionicDeadLineInfo test_fixture##_##test_name##_deadline_info(#test_fixture, #test_name, deadline_in_ms); \
  TEST_F(test_fixture, test_name)

#endif // _BIONIC_TESTS_BIONIC_GTEST_H
