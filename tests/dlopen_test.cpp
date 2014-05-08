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

#include <dlfcn.h>

TEST(dlopen, symlink) {
  void* handle1 = dlopen("libdlext_test.so", RTLD_NOW);
  void* handle2 = dlopen("libdlext_test_v2.so", RTLD_NOW);
  ASSERT_TRUE(handle1 != NULL);
  ASSERT_TRUE(handle2 != NULL);
  ASSERT_EQ(handle1, handle2);
}
