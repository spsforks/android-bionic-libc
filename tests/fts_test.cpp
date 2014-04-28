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

#include <fts.h>

#include <gtest/gtest.h>

TEST(fts, fts_smoke) {
  char* const paths[] = { "/", nullptr };

  FTS* fts = fts_open(paths, FTS_PHYSICAL | FTS_XDEV | FTS_SEEDOT, NULL);
  ASSERT_TRUE(fts != nullptr);

  FTSENT* entry = fts_children(fts, 0);
  ASSERT_TRUE(entry != nullptr);

  EXPECT_FALSE(entry->fts_cycle != nullptr);
  EXPECT_FALSE(entry->fts_parent == nullptr);
  EXPECT_FALSE(entry->fts_link != nullptr);

  ASSERT_EQ(0, fts_close(fts));
}
