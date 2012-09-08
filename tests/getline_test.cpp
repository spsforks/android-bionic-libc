/*
 * Copyright (c) 2010, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of Intel Corporation nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
