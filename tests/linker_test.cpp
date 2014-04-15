/*
 * Copyright (C) 2012 The Android Open Source Project
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

#if defined(__LP64__)
#define SUFFIX_64 "64"
#else
#define SUFFIX_64 ""
#endif

#define PRINT(x...) printf("PRINT: " x)
#define INFO(x...) printf("INFO: " x)
#define LDPATH_SIZE 128

#include "../linker/linker_util.cpp"


TEST(linker_util, parse_so_path_basic) {
    const char* origin_path = "ORIGIN_PATH";
    const char* input = "$ORIGIN/dir1:${ORIGIN}/dir2:${ORIGIN/dir3:/dir4:$LIB/dir5$";
    const char* expected[] = {
        "ORIGIN_PATH/dir1",
        "ORIGIN_PATH/dir2",
        "${ORIGIN/dir3",
        "/dir4",
        "/system/lib" SUFFIX_64 "/dir5$",
        NULL,
    };
    char output[1280];
    const char* array[11]; // 10 + terminator

    memset(output, 43, sizeof(output));
    memset(array, 0, sizeof(array));

    ASSERT_TRUE(parse_so_paths(input, array, output, 512, 10, origin_path));
    ASSERT_STREQ(expected[0], array[0]);
    ASSERT_STREQ(expected[1], array[1]);
    ASSERT_STREQ(expected[2], array[2]);
    ASSERT_STREQ(expected[3], array[3]);
    ASSERT_STREQ(expected[4], array[4]);
}

TEST(linker_util, parse_so_path_replace_token_long) {
    const char* origin_path = "somewhat_long_origin";
    const char* input = "$ORIGIN/very/long/path/a12345678901234567890";
    char output[512];
    memset(output, 42, sizeof(output));
    ASSERT_FALSE(parse_so_path_replace_token(output, 46, input, origin_path));
    ASSERT_EQ(output[46], 42);
    ASSERT_EQ(output[113], 42);
}

