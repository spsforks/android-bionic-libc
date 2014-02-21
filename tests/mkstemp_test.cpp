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

#include <gtest/gtest.h>

#include "TemporaryFile.h"
#include <stdlib.h>
#include <sys/stat.h>

TEST(mkstemp, mkstemp) {
  TemporaryFile tf;
  struct stat sb;
  ASSERT_EQ(0, fstat(tf.fd, &sb));
}

TEST(mkstemp, mkstemp64) {
  TemporaryFile tf(true);
  struct stat64 sb;
  ASSERT_EQ(0, fstat64(tf.fd, &sb));
}
