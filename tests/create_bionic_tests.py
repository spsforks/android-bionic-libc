#!/usr/bin/env python
import sys
import os

#cpp file template strings
header = '''/*
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

 /*
  * This file is auto-generated.
  * Create with: python bionic/tests/create_bionic_tests.py libcore/luni/src/test/resources/math_tests.csv bionic/tests/math_csv_test.cpp
  */

#include <math.h>
#include <gtest/gtest.h>

#if defined(__BIONIC__)
'''

struct = '''struct TestCase {
  double expected;
  double call_data;
};

'''

array_start = '''static TestCase g_%s_data[] = {
'''

array_entry ='''  { // Entry %u
    %s,
    %s
  },
'''

array_end = '''};

'''

close_bionic = '''#endif // __BIONIC__

'''

test_func_header = '''TEST(math_csv, csv_values) {
#if defined(__BIONIC__)
'''

test_func_run = '''  for (size_t i = 0; i < sizeof(g_%s_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_%s_data[i].expected, %s(g_%s_data[i].call_data)) << "Failed on element " << i << " of function %s";
  }
'''

test_func_footer = '''#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.";
#endif // __BIONIC__
}'''

class TestCase(object):
    """keeps test case together as object"""
    def __init__(self, func, expected_output, input_value, extra = ""):
        self.func = func
        self.input_value = input_value
        self.expected_output = expected_output
        self.extra = extra


if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.stderr.write("Usage: this.py input_file output_file\n")
        sys.exit(1)

    input_file = open(sys.argv[1], 'r')

    all_functions = {}
    for line in input_file:
        if line[0] != '#':
            test_case_arr = line.split(',')
            test_case = TestCase(test_case_arr[0], test_case_arr[1], test_case_arr[2])
            if test_case.func not in all_functions:
                all_functions[test_case.func] = []
            all_functions[test_case.func].append(test_case)

    input_file.close()
    output_file = open(sys.argv[2], 'w')

    output_file.write(header)
    output_file.write(struct)

    for func in all_functions:
        output_file.write(array_start % func)
        test_cases = all_functions[func]
        for i in range(len(test_cases)):
            test_case = test_cases[i]
            output_file.write(array_entry % (i, test_case.expected_output, test_case.input_value))
        output_file.write(array_end)

    output_file.write(close_bionic)
    output_file.write(test_func_header)

    for func in all_functions:
        output_file.write(test_func_run % (func, func, func, func, func))

    output_file.write(test_func_footer)


