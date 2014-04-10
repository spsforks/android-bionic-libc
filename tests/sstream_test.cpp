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

#include <stdint.h>
#include <limits>
#include <sstream>

// TODO: move this test to libcxx.

template <typename T>
static void CheckOverflow(T expected, const char* value, bool should_overflow) {
  std::stringstream ss(value);
  T result = T(0);
  ss >> result;
  EXPECT_FALSE(ss.bad()) << value << ' ' << int64_t(result);
  EXPECT_EQ(should_overflow, ss.fail()) << value << ' ' << int64_t(result);
  if (!should_overflow) { // glibc writes garbage on overflow.
    ASSERT_EQ(expected, result) << value;
  }
}

TEST(sstream, __get_integer_overflow_8) {
  // Bytes seem to be special. Presumably the usual byte/char confusion.
  CheckOverflow<int8_t>(45, "-128", false);
  CheckOverflow<int8_t>(45, "-129", false);
  CheckOverflow<int8_t>(49, "127", false);
  CheckOverflow<int8_t>(49, "128", false);
  CheckOverflow<uint8_t>(50, "255", false);
  CheckOverflow<uint8_t>(50, "256", false);
}

TEST(sstream, __get_integer_overflow_16) {
  CheckOverflow<int16_t>(std::numeric_limits<int16_t>::min(), "-32768", false);
  CheckOverflow<int16_t>(0, "-32769", true);
  CheckOverflow<int16_t>(std::numeric_limits<int16_t>::max(), "32767", false);
  CheckOverflow<int16_t>(0, "32768", true);

  CheckOverflow<uint16_t>(std::numeric_limits<uint16_t>::max(), "65535", false);
  CheckOverflow<uint16_t>(0, "65536", true);
}

TEST(sstream, __get_integer_overflow_32) {
  CheckOverflow<int32_t>(std::numeric_limits<int32_t>::min(), "-2147483648", false);
  CheckOverflow<int32_t>(0, "-2147483649", true);
  CheckOverflow<int32_t>(std::numeric_limits<int32_t>::max(), "2147483647", false);
  CheckOverflow<int32_t>(0, "2147483648", true);

  CheckOverflow<uint32_t>(std::numeric_limits<uint32_t>::max(), "4294967295", false);
  CheckOverflow<uint32_t>(0, "4294967296", true);
}

TEST(sstream, __get_integer_overflow_64) {
  CheckOverflow<int64_t>(std::numeric_limits<int64_t>::min(), "-9223372036854775808", false);
  CheckOverflow<int64_t>(0, "-9223372036854775809", true);
  CheckOverflow<int64_t>(std::numeric_limits<int64_t>::max(), "9223372036854775807", false);
  CheckOverflow<int64_t>(0, "9223372036854775808", true);

  CheckOverflow<uint64_t>(std::numeric_limits<uint64_t>::max(), "18446744073709551615", false);
  CheckOverflow<uint64_t>(0, "18446744073709551616", true);
}
