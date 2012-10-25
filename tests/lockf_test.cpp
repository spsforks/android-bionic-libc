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

class LockfTest : public ::testing::Test
{
public:
  virtual void SetUp() {
    fp_ = NULL;
    fd_ = -1;
  }

  virtual void TearDown() {
    if (fp_ != NULL) {
      // The file is already unlinked, no need to delete it.
      fclose(fp_);
      fp_ = NULL;
      fd_ = -1;
    }
  }

  void SeekTo(off64_t pos) {
    ASSERT_EQ(pos, lseek64(fd_, pos, SEEK_SET));
  };

  // Override this for 64-bit test.
  virtual void TestLockf(int expected, int cmd, off64_t start, off64_t len) {
    ASSERT_TRUE(static_cast<off_t>(start) == start);
    ASSERT_TRUE(static_cast<off_t>(len) == len);
    SeekTo(start);
    ASSERT_EQ(expected, lockf(fd_, cmd, static_cast<off_t>(len)));
  }

  void CreateTempFile(off64_t file_size);
  void CheckLockAndTest();
  void CheckLockWithChild();
  void CheckLockHalfWithChild();

  FILE* fp_;
  int fd_;
};

class Lockf64Test : public LockfTest
{
  virtual void TestLockf(int expected, int cmd, off64_t start, off64_t len) {
    SeekTo(start);
    ASSERT_EQ(expected, lockf64(fd_, cmd, len));
  }
};


static void WaitForChildExit(void) {
  int status;
  wait(&status);
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(0, WEXITSTATUS(status));
}

// Create a temporary file, fill it with zeroes.
// This method aborts in case of error.
void LockfTest::CreateTempFile(off64_t size) {
    // Create temporary file, store its file descriptor in fd_;
    const size_t kBufferSize = 65536;
    static const char kBuffer[kBufferSize] = { 0 };

    fp_ = tmpfile();
    ASSERT_TRUE(fp_ != NULL);

    while (size > 0) {
        size_t avail = kBufferSize;
        if (avail > size)
            avail = static_cast<size_t>(size);

        size_t ret = fwrite(kBuffer, 1, kBufferSize, fp_);
        ASSERT_NE(0U, ret);
        size -= ret;
    }

    fd_ = fileno(fp_);
    ASSERT_NE(-1, fd_);
};

// Run our test. This uses the TestLockf method which will be different
// depending on whether lockf or lockf64 is being tested.
void LockfTest::CheckLockAndTest() {
  const off64_t file_size = 1024*1024LL;
  CreateTempFile(file_size);

  // Lock everything.
  TestLockf(0, F_LOCK, 0LL, file_size);
  // Try-lock everything, this should succeed too.
  TestLockf(0, F_TLOCK, 0LL, file_size);
  // Check status.
  TestLockf(0, F_TEST, 0LL, file_size);
  // Unlock file.
  TestLockf(0, F_ULOCK, 0LL, file_size);
}

void LockfTest::CheckLockWithChild() {
  const off64_t file_size = 1024*1024LL;
  CreateTempFile(file_size);

  // Lock everything
  TestLockf(0, F_TLOCK, 0LL, file_size);
  // Fork a child process
  pid_t child = fork();
  if (child == 0) {
    // Check that the child cannot lock the file.
    TestLockf(-1, F_TLOCK, 0LL, file_size);
    TestLockf(-1, F_TEST,  0LL, file_size);
    ASSERT_EQ(EACCES, errno);
    exit(0);
  }
  WaitForChildExit();

  TestLockf(0, F_ULOCK, 0LL, file_size);
}

void LockfTest::CheckLockHalfWithChild() {
  const off64_t file_size = 1024*1024LL;
  CreateTempFile(file_size);

  // Lock the first half of the file.
  TestLockf(0, F_TLOCK, 0LL, file_size/2);

  // Fork a child process
  pid_t child = fork();
  if (child == 0) {
    // Check that the child can lock the other half
    TestLockf(0, F_TLOCK, file_size/2, file_size - file_size/2);

    // Check that the child cannot lock the first half
    TestLockf(-1, F_TEST, 0LL, file_size/2);
    ASSERT_EQ(EACCES, errno);

    TestLockf(-1, F_TLOCK, 0LL, file_size/2);
    exit(0);
  }
  WaitForChildExit();

  // The second half was locked by the child, but the lock disappeared
  // when the process exited, so check it can be locked now.
  TestLockf(0, F_TLOCK, file_size/2, file_size - file_size/2);
}

TEST_F(LockfTest, LockAndTest) {
  CheckLockAndTest();
}

TEST_F(LockfTest, LockWithChild) {
  CheckLockWithChild();
}

TEST_F(LockfTest, LockHalfWithChild) {
  CheckLockHalfWithChild();
}

TEST_F(Lockf64Test, LockAndTest) {
  CheckLockAndTest();
}

TEST_F(Lockf64Test, LockWithChild) {
  CheckLockWithChild();
}

TEST_F(Lockf64Test, LockHalfWithChild) {
  CheckLockHalfWithChild();
}
