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
#include <memory>
#include <string>
#include <vector>
#include "private/bionic_string_utils.h"
#include "../libc/bionic/bionic_string_utils.cpp"
using std::vector;
using std::string;
using std::unique_ptr;

namespace {

unique_ptr<char[]> init_buf(const vector<const string> &list, int *buflen) {
  size_t size = 0;
  for (const auto& elem : list) {
    size += elem.size() + 1;
  }
  *buflen = (list.size() + 1) * sizeof(char**) + size;
  return std::move(unique_ptr<char[]>(new char[*buflen]));
}

vector<char*> create_list(const vector<const string> &list) {
  vector<char*> result;
  result.reserve(list.size() + 1);
  for (const auto& elem : list) {
    result.push_back(const_cast<char*>(elem.c_str()));
  }
  result.push_back(nullptr);
  return std::move(result);
}

void assert_lists_eq(const vector<char*> &expected, char** actual) {
  size_t size = 0;
  while (*(actual + size) != nullptr) {
    ++size;
  }
  ASSERT_EQ(expected.size(), size + 1);
  for (size_t i = 0; i < size; ++i) {
    ASSERT_EQ(0, strcmp(expected[i], *(actual + i)));
  }
  ASSERT_EQ(nullptr, *(actual + size));
}

void test(const vector<const string> &list) {
  int buflen = 0;
  unique_ptr<char[]> buf = init_buf(list, &buflen);
  vector<char*> input = create_list(list);
  ASSERT_EQ(buflen, copy_list(&input[0], buf.get(), buflen));
  assert_lists_eq(input, reinterpret_cast<char**>(buf.get()));
}

} // namespace

TEST(bionic_string_utils, empty_list) {
  test({});
}

TEST(bionic_string_utils, single_string_list) {
  test({"test_string"});
}

TEST(bionic_string_utils, single_string_list_too_small_buf) {
  vector<const string> list{"test_string"};
  int buflen = 0;
  unique_ptr<char[]> buf = init_buf(list, &buflen);
  vector<char*> input = create_list(list);
  ASSERT_EQ(-1, copy_list(&input[0], buf.get(), buflen - 1));
}

TEST(bionic_string_utils, two_strings_list_too_small_buf) {
  vector<const string> list{"test_string", "example_second_string"};
  int buflen = 0;
  unique_ptr<char[]> buf = init_buf(list, &buflen);
  vector<char*> input = create_list(list);
  ASSERT_EQ(-1, copy_list(&input[0], buf.get(), buflen - 1));
}

TEST(bionic_string_utils, two_strings_list) {
  test({"test_string", "example_second_string"});
}

TEST(bionic_string_utils, three_strings_list) {
  test({"a", "bc", "def"});
}

TEST(bionic_string_utils, empty_strings_list) {
  test({""});
  test({"", ""});
  test({"", "", ""});
  test({"", "a", ""});
  test({"a", "", "b"});
}
