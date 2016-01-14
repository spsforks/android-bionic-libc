/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <gtest/gtest.h>

#include "Memleak.h"

void* ptr;

class PointerHider {
 public:
  PointerHider() {
    EXPECT_EQ(0, pipe2(pipefd_, O_CLOEXEC|O_NONBLOCK));
  }
  ~PointerHider() {
    close(pipefd_[0]);
    close(pipefd_[1]);
  }
  void Hide(void* ptr) {
    EXPECT_EQ(static_cast<ssize_t>(sizeof(ptr)), write(pipefd_[1], &ptr, sizeof(ptr)));
  }
  void* Restore() {
    void* ptr = NULL;
    EXPECT_EQ(static_cast<ssize_t>(sizeof(ptr)), read(pipefd_[0], &ptr, sizeof(ptr)));
    return ptr;
  }
 private:
  int pipefd_[2];
};

TEST(MemleakTest, clean) {
  std::vector<Leak> leaks;

  ASSERT_TRUE(DetectLeaks(leaks));
  ASSERT_EQ(0U, leaks.size());
}

TEST(MemleakTest, stack) {
  PointerHider hider;

  {
    void* ptr = malloc(256);
    write(0, ptr, 0);

    std::vector<Leak> leaks;

    ASSERT_TRUE(DetectLeaks(leaks));
    ASSERT_EQ(0U, leaks.size());

    hider.Hide(ptr);
  }

  {
    std::vector<Leak> leaks;

    ASSERT_TRUE(DetectLeaks(leaks));
    ASSERT_EQ(1U, leaks.size());
  }

  {
    void* ptr = hider.Restore();
    free(ptr);

    std::vector<Leak> leaks;

    ASSERT_TRUE(DetectLeaks(leaks));
    ASSERT_EQ(0U, leaks.size());
  }
}

TEST(MemleakTest, global) {
  PointerHider hider;

  {
    ptr = malloc(256);
    write(0, ptr, 0);

    std::vector<Leak> leaks;

    ASSERT_TRUE(DetectLeaks(leaks));
    ASSERT_EQ(0U, leaks.size());

    hider.Hide(ptr);
    ptr = NULL;
  }

  {
    std::vector<Leak> leaks;

    ASSERT_TRUE(DetectLeaks(leaks));
    ASSERT_EQ(1U, leaks.size());
  }

  {
    void* ptr = hider.Restore();
    free(ptr);

    std::vector<Leak> leaks;

    ASSERT_TRUE(DetectLeaks(leaks));
    ASSERT_EQ(0U, leaks.size());
  }
}

TEST(MemleakTest, tls) {
  PointerHider hider;
  pthread_key_t key;
  pthread_key_create(&key, NULL);

  {
    pthread_setspecific(key, malloc(256));

    std::vector<Leak> leaks;

    ASSERT_TRUE(DetectLeaks(leaks));
    ASSERT_EQ(0U, leaks.size());

    hider.Hide(pthread_getspecific(key));
    pthread_setspecific(key, nullptr);
  }

  {
    std::vector<Leak> leaks;

    ASSERT_TRUE(DetectLeaks(leaks));
    ASSERT_EQ(1U, leaks.size());
  }

  {
    void* ptr = hider.Restore();
    free(ptr);

    std::vector<Leak> leaks;

    ASSERT_TRUE(DetectLeaks(leaks));
    ASSERT_EQ(0U, leaks.size());
  }

  std::vector<Leak> leaks;

  ASSERT_TRUE(DetectLeaks(leaks));
  ASSERT_EQ(0U, leaks.size());

  pthread_key_delete(key);
}

TEST(MemleakTest, twice) {
  PointerHider hider;

  {
    void* ptr = malloc(256);
    hider.Hide(ptr);
  }

  {
    std::vector<Leak> leaks;

    ASSERT_TRUE(DetectLeaks(leaks));
    ASSERT_EQ(1U, leaks.size());
  }

  {
    std::vector<Leak> leaks;

    ASSERT_TRUE(DetectLeaks(leaks));
    ASSERT_EQ(1U, leaks.size());
  }

  {
    void* ptr = hider.Restore();
    free(ptr);
  }

  {
    std::vector<Leak> leaks;

    ASSERT_TRUE(DetectLeaks(leaks));
    ASSERT_EQ(0U, leaks.size());
  }
}

TEST(MemleakTest, log) {
  PointerHider hider;

  {
    void* ptr = malloc(256);
    hider.Hide(ptr);
  }

  ASSERT_TRUE(DetectAndLogLeaks(true));

  {
    void* ptr = hider.Restore();
    free(ptr);
  }

  {
    std::vector<Leak> leaks;

    ASSERT_TRUE(DetectLeaks(leaks));
    ASSERT_EQ(0U, leaks.size());
  }
}
