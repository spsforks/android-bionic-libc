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

#include <stddef.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

// Create a temporary file and fill it with stuff.
// This function aborts on error, except on ENOSPC (see lockf64 test).
//
static FILE* get_temp_file(off64_t size) {
  const size_t kBufferSize = 1024*1024;
  static const char kBuffer[kBufferSize] = { 0 };

  FILE* fp = tmpfile();
  ASSERT_NE(fp, NULL);

  while (size > 0) {
      size_t avail = kBufferSize;
      if (avail > size)
          avail = static_cast<size_t>(size);

      size_t ret = fwrite(kBuffer, 1, kBufferSize, fp);

      // Special case to be able to report ENOSPC
      if (ret == 0 && ferror(fp) && errno == ENOSPC) {
          // This should delete the file too.
          fclose(fp);
          // Just in case.
          errno = ENOSPC;
          return NULL;
      }

      ASSERT_NE(0, ret);
      size -= ret;
  }

  return fp;
}

TEST(unistd, lockf) {
  static const off64_t kFileSize = 1024*1024;  // 1 MB
  static const off_t kFileLen = (off_t) kFileSize;
  FILE* fp = get_temp_file(kFileSize);
  ASSERT_TRUE(fp != NULL);

  int fd = fileno(fp);
  ASSERT_NE(-1, fd);

  // Try-lock everything, this should succeed immediately.
  ASSERT_EQ(0, lockf(fd, F_TLOCK, kFileLen));

  // Fork, then check that the child can't lock the file.
  // (Locks are not inherited by child processes).
  pid_t child = fork();
  if (child == 0) {
      ASSERT_EQ(-1, lockf(fd, F_TEST, kFileLen));
      exit(0);
  }
  int status;
  wait(&status);
  ASSERT_TRUE(WIFEXITED(status) && WEXITSTATUS(status) == 0);

  // Unlock the first half of the file, then fork a child
  ASSERT_EQ(0, lockf(fd, F_ULOCK, kFileLen/2));
  child = fork();
  if (child == 0) {
      // Check that the child can immediately lock the other half
      ASSERT_EQ(kFileLen/2, lseek(fd, kFileLen/2, SEEK_SET));
      ASSERT_EQ(0, lockf(fd, F_TLOCK, kFileLen - kFileLen/2));

      // Check that the child cannot unlock the first half
      ASSERT_EQ( 0, lseek(fd, 0, SEEK_SET));
      ASSERT_EQ(-1, lockf(fd, F_ULOCK, kFileLen/2));

      exit(0);
  }
  // Wait for the child to complete.
  wait(&status);
  ASSERT_TRUE(WIFEXITED(status) && WEXITSTATUS(status) == 0);

  // Try to lock the first half now. It was locked by the child, but
  // the lock should have disappeared when it exited.
  ASSERT_EQ(0, lockf(fd, F_TLOCK, kFileLen/2));

  // Close the file, it is already unlinked, so no need to delete it.
  fclose(fp);
}

TEST(unistd, lockf64) {
  // NOTE: This test requires 5 GiB of free space on the test device by
  //       default.
  off64_t kFileLen = 5LL*1024*1024*1024;
  FILE* fp = get_temp_file(kFileLen);

  if (fp == NULL && errno == ENOSPC) {
      fprintf(stderr,
              "Warning: Not enough free space to run 5 GiB lockf test\n");
      kFileLen = 1024*1024;
      fp = get_temp_file(kFileLen);
  }
  ASSERT_NE(NULL, fp);

  int fd = fileno(fp);
  ASSERT_NE(-1, fd);

  // Try-lock everything, this should succeed immediately.
  ASSERT_EQ(0, lockf64(fd, F_TLOCK, kFileLen));

  // Fork, then check that the child can't lock the file.
  // (Locks are not inherited by child processes).
  pid_t child = fork();
  if (child == 0) {
      ASSERT_EQ(-1, lockf64(fd, F_TEST, kFileLen));
      exit(0);
  }
  int status;
  wait(&status);
  ASSERT_TRUE(WIFEXITED(status) && WEXITSTATUS(status) == 0);

  // Unlock the first half of the file, then fork a child
  ASSERT_EQ(0, lockf64(fd, F_ULOCK, kFileLen/2));
  child = fork();
  if (child == 0) {
      // Check that the child can immediately lock the other half
      ASSERT_EQ(kFileLen/2, lseek64(fd, kFileLen/2, SEEK_SET));
      ASSERT_EQ(0, lockf64(fd, F_TLOCK, kFileLen - kFileLen/2));

      // Check that the child cannot unlock the first half
      ASSERT_EQ( 0, lseek64(fd, 0, SEEK_SET));
      ASSERT_EQ(-1, lockf64(fd, F_ULOCK, kFileLen/2));

      exit(0);
  }
  // Wait for the child to complete.
  wait(&status);
  ASSERT_TRUE(WIFEXITED(status) && WEXITSTATUS(status) == 0);

  // Try to lock the first half now. It was locked by the child, but
  // the lock should have disappeared when it exited.
  ASSERT_EQ(0, lockf64(fd, F_TLOCK, kFileLen/2));

  // Close the file, it is already unlinked, so no need to delete it.
  fclose(fp);
}
