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

#define _GNU_SOURCE 1

#include <math.h>

#include <fenv.h>

#include <gtest/gtest.h>

template <typename T>
struct data_t {
  T expected;
  T call_data;
};

union float_u {
  float value;
  struct {
    unsigned frac:23;
    unsigned exp:8;
    unsigned sign:1;
  } bits;
  uint32_t sign_magnitude;
};

union double_u {
  double value;
  struct {
    unsigned fracl;
    unsigned frach:20;
    unsigned exp:11;
    unsigned sign:1;
  } bits;
  uint64_t sign_magnitude;
};

// TODO: factor out duplication.
static inline uint32_t SignAndMagnitudeToBiased(const float& value) {
  float_u u;
  u.value = value;
  if (u.bits.sign) {
    return ~u.sign_magnitude + 1;
  } else {
    u.bits.sign = 1;
    return u.sign_magnitude;
  }
}

static inline uint64_t SignAndMagnitudeToBiased(const double& value) {
  double_u u;
  u.value = value;
  if (u.bits.sign) {
    return ~u.sign_magnitude + 1;
  } else {
    u.bits.sign = 1;
    return u.sign_magnitude;
  }
}

// Based on the existing googletest implementation.
template <typename T>
size_t UlpDistance(T lhs, T rhs) {
  const auto biased1 = SignAndMagnitudeToBiased(lhs);
  const auto biased2 = SignAndMagnitudeToBiased(rhs);
  return (biased1 >= biased2) ? (biased1 - biased2) : (biased2 - biased1);
}

template <size_t ULP, typename T>
struct FpEq {
  ::testing::AssertionResult operator()(const char* /*expected_expression*/,
                                        const char* /*actual_expression*/,
                                        T expected,
                                        T actual) {
    if (!isnan(expected) && !isnan(actual) && UlpDistance(expected, actual) <= ULP) {
      return ::testing::AssertionSuccess();
    }

    char expected_str[64];
    char actual_str[64];
    snprintf(expected_str, sizeof(expected_str), "%a", expected);
    snprintf(actual_str, sizeof(actual_str), "%a", actual);

    return ::testing::AssertionFailure() << "expected (" << expected_str << ") != actual (" << actual_str << ")";
  }
};

template <size_t ULP, typename T, size_t N>
void DoMathDataTest(data_t<T> (&data)[N], T f(T)) {
  fesetenv(FE_DFL_ENV);
  FpEq<ULP, T> predicate;
  for (size_t i = 0; i < N; ++i) {
    EXPECT_PRED_FORMAT2(predicate,
                        data[i].expected, f(data[i].call_data)) << "Failed on entry " << i;
  }
}
