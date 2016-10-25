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

#include <inttypes.h>

#include <errno.h>
#include <gtest/gtest.h>
#include <stdio.h>

#define PRINTF_TYPED(FMT_SUFFIX, TYPE_SUFFIX) \
  snprintf(buf, sizeof(buf), "%08" PRId##FMT_SUFFIX, int##TYPE_SUFFIX(0)); \
  snprintf(buf, sizeof(buf), "%08" PRIi##FMT_SUFFIX, int##TYPE_SUFFIX(0)); \
  snprintf(buf, sizeof(buf), "%08" PRIo##FMT_SUFFIX, int##TYPE_SUFFIX(0)); \
  snprintf(buf, sizeof(buf), "%08" PRIu##FMT_SUFFIX, uint##TYPE_SUFFIX(0)); \
  snprintf(buf, sizeof(buf), "%08" PRIx##FMT_SUFFIX, uint##TYPE_SUFFIX(0)); \
  snprintf(buf, sizeof(buf), "%08" PRIX##FMT_SUFFIX, uint##TYPE_SUFFIX(0)); \

#define PRINTF_SIZED(WIDTH) \
  PRINTF_TYPED(WIDTH, WIDTH##_t) \
  PRINTF_TYPED(FAST##WIDTH, _fast##WIDTH##_t) \
  PRINTF_TYPED(LEAST##WIDTH, _least##WIDTH##_t) \


#define SCANF_TYPED(FMT_SUFFIX, TYPE_SUFFIX) \
  sscanf(buf, "%08" SCNd##FMT_SUFFIX, reinterpret_cast<int##TYPE_SUFFIX*>(dst)); \
  sscanf(buf, "%08" SCNi##FMT_SUFFIX, reinterpret_cast<int##TYPE_SUFFIX*>(dst)); \
  sscanf(buf, "%08" SCNo##FMT_SUFFIX, reinterpret_cast<int##TYPE_SUFFIX*>(dst)); \
  sscanf(buf, "%08" SCNu##FMT_SUFFIX, reinterpret_cast<uint##TYPE_SUFFIX*>(dst)); \
  sscanf(buf, "%08" SCNx##FMT_SUFFIX, reinterpret_cast<uint##TYPE_SUFFIX*>(dst)); \

#define SCANF_SIZED(SIZE) \
  SCANF_TYPED(SIZE, SIZE##_t) \
  SCANF_TYPED(FAST##SIZE, _fast##SIZE##_t) \
  SCANF_TYPED(LEAST##SIZE, _least##SIZE##_t) \


TEST(inttypes, misc) {
  char buf[512];
  char dst[512];

  PRINTF_SIZED(8);
  PRINTF_SIZED(16);
  PRINTF_SIZED(32);
  PRINTF_SIZED(64);

  PRINTF_TYPED(MAX, max_t);
  PRINTF_TYPED(PTR, ptr_t);

  SCANF_SIZED(8)
  SCANF_SIZED(16)
  SCANF_SIZED(32)
  SCANF_SIZED(64)

  SCANF_TYPED(MAX, max_t)
  SCANF_TYPED(PTR, ptr_t)
}

TEST(inttypes, wcstoimax) {
  ASSERT_EQ(123, wcstoimax(L"123", NULL, 10));
}

TEST(inttypes, wcstoumax) {
  ASSERT_EQ(123U, wcstoumax(L"123", NULL, 10));
}

TEST(inttypes, strtoimax_EINVAL) {
  errno = 0;
  strtoimax("123", NULL, -1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  strtoimax("123", NULL, 1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  strtoimax("123", NULL, 37);
  ASSERT_EQ(EINVAL, errno);
}

TEST(inttypes, strtoumax_EINVAL) {
  errno = 0;
  strtoumax("123", NULL, -1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  strtoumax("123", NULL, 1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  strtoumax("123", NULL, 37);
  ASSERT_EQ(EINVAL, errno);
}

TEST(inttypes, wcstoimax_EINVAL) {
  errno = 0;
  wcstoimax(L"123", NULL, -1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  wcstoimax(L"123", NULL, 1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  wcstoimax(L"123", NULL, 37);
  ASSERT_EQ(EINVAL, errno);
}

TEST(inttypes, wcstoumax_EINVAL) {
  errno = 0;
  wcstoumax(L"123", NULL, -1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  wcstoumax(L"123", NULL, 1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  wcstoumax(L"123", NULL, 37);
  ASSERT_EQ(EINVAL, errno);
}
