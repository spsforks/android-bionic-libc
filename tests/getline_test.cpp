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

#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>

#if __BIONIC__
#define TMP_FILENAME "/data/getline-test"
#else
#define TMP_FILENAME "/tmp/getline-test"
#endif

TEST(getline, basic) {
  // Basic test that writes some lines into a file
  // and reads them back using getline
  FILE *fp;
  char *line_read = NULL;
  const char *line_written = "This is a test for getline\n";
  size_t len = 0;
  ssize_t read;
  int i, ret, nr_lines = 5;

  errno = 0;

  fp = fopen(TMP_FILENAME, "w+");
  ASSERT_TRUE(fp != NULL);
  ASSERT_EQ(errno, 0);

  for (i = 0; i < nr_lines; i++) {
    ret = fprintf(fp, "%s", line_written);
    ASSERT_TRUE(ret == (int) strlen(line_written));
    ASSERT_EQ(errno, 0);
  }

  rewind(fp);

  i = 0;
  while ((read = getline(&line_read, &len, fp)) != -1) {
    ASSERT_TRUE(read == (int) strlen(line_written));
    ASSERT_GE(len, strlen(line_written));
    EXPECT_STREQ(line_read, line_written);
    ASSERT_EQ(errno, 0);
    memset(line_read, 0, sizeof(line_read));
    i++;
  }
  ASSERT_EQ(i, nr_lines);

  free(line_read);
  fclose(fp);
  remove(TMP_FILENAME);
}
