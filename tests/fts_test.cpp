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

#include <sys/types.h>
#include <fts.h>

#include <gtest/gtest.h>
#include "CrashCatch.h"


/*
 * This test checks that
 *   fts_open() and
 *   fts_children() works.
 */
TEST(fts, fts)
{
    CRASH_CATCH();

    FTS *ftsResult;
    char * const path0 = (char *) "/";
    char * paths[] = {path0, 0};

    { SCOPED_TRACE("fts_open()");
       ftsResult = fts_open(paths, FTS_PHYSICAL | FTS_XDEV | FTS_SEEDOT, NULL);
       ASSERT_TRUE(ftsResult != NULL);
    }

    FTSENT* childEntry;

    { SCOPED_TRACE("fts_children()");
       childEntry = fts_children(ftsResult, 0);
       ASSERT_TRUE(childEntry != NULL);

       EXPECT_FALSE(childEntry->fts_cycle != NULL);
       EXPECT_FALSE(childEntry->fts_parent == NULL);
       EXPECT_FALSE(childEntry->fts_link != NULL);
    }
}
