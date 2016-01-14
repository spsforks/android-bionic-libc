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

#include <stdlib.h>

#include <gtest/gtest.h>

#include "Memleak.h"

volatile void* ptr;

TEST(MemleakTest, clean) {
  std::vector<Leak> leaks;

  ASSERT_TRUE(DetectLeaks(leaks));
  ASSERT_EQ(0U, leaks.size());
}

TEST(MemleakTest, clean_stack) {
  void* buf = malloc(256);
  write(0, buf, 0);

  std::vector<Leak> leaks;

  ASSERT_TRUE(DetectLeaks(leaks));
  ASSERT_EQ(0U, leaks.size());

  free(buf);
}

TEST(MemleakTest, clean_global) {
  ptr = malloc(256);
  write(0, const_cast<void*>(ptr), 0);

  std::vector<Leak> leaks;

  ASSERT_TRUE(DetectLeaks(leaks));
  ASSERT_EQ(0U, leaks.size());

  free(const_cast<void*>(ptr));
}

TEST(MemleakTest, clean_tls) {
  pthread_key_t key;
  pthread_key_create(&key, NULL);
  pthread_setspecific(key, malloc(256));

  std::vector<Leak> leaks;

  ASSERT_TRUE(DetectLeaks(leaks));
  ASSERT_EQ(0U, leaks.size());

  free(pthread_getspecific(key));
  pthread_setspecific(key, nullptr);
  pthread_key_delete(key);
}

TEST(MemleakTest, leak) {
  ptr = malloc(256);
  write(0, const_cast<void*>(ptr), 0);
  ptr = nullptr;

  std::vector<Leak> leaks;

  ASSERT_TRUE(DetectLeaks(leaks));
  ASSERT_EQ(1U, leaks.size());
}
