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

#include <alloca.h>
#include <string.h>

#include <gtest/gtest.h>
#include "CrashCatch.h"

const static int SIZE    = 1024;
const static int PADDING = 64;

/*
 * This test checks that
 *   alloca() works,
 *   that allocated memory is available for read and write.
 */
TEST(alloca, alloca)
{
    CRASH_CATCH();

    unsigned char *a;

    { SCOPED_TRACE("alloca()");
      // The test subject
      a = (unsigned char *) alloca(SIZE);
    }

    // Check allocation
    EXPECT_TRUE(a != NULL);

    { SCOPED_TRACE("memory test");
      // Check availability of allocated memory
      unsigned char *setA, *getA;
      setA = getA = a;

       for (int i = 0; i < SIZE; ++i)
       {
	 *setA = 0;
	 EXPECT_EQ(0, *getA);
	 *setA = 0xFF;
	 EXPECT_EQ(0xFF, *getA);
	 ++setA;
	 ++getA;
       }
    }
}

/*
 * Allocates and reuses memory on stack
 * 1. copy test_string if *base_addr is NULL
 * 2. check reused memory with test_string if *base_addr is not NULL
 */
static void allocate_and_reuse(size_t *base_addr)
{
  const static char test_string[SIZE] =
    "99 bottles of beer on the wall, 99 bottles of beer.\n"
    "Take one down and pass it around, 98 bottles of beer on the wall.\n"
    "98 bottles of beer on the wall, 98 bottles of beer.\n"
    "Take one down and pass it around, 97 bottles of beer on the wall.\n"
    "97 bottles of beer on the wall, 97 bottles of beer.\n"
    "Take one down and pass it around, 96 bottles of beer on the wall.\n"
    "96 bottles of beer on the wall, 96 bottles of beer.\n"
    "Take one down and pass it around, 95 bottles of beer on the wall.\n";

  char *a;
  // The test subject
  a = (char *) alloca(SIZE + PADDING);
  if (*base_addr == 0)

    {
      strncpy(a + PADDING, test_string, SIZE);
      *base_addr = (size_t) a;
    }
  else
    {
      ptrdiff_t offset = (size_t) a - *base_addr;
      if ((offset >= 0) && (offset <= PADDING))
	EXPECT_EQ(0, strncmp(a + PADDING, test_string + offset, SIZE));
      else
	FAIL() << "Illegal stack offset: " << offset;
    }
}

/*
 * This test checks alloca() space reuse.
 * Allocation on top of stack frame is required to pass.
 */
TEST(alloca, alloca_reuse)
{
  CRASH_CATCH();

  size_t addr_on_stack = 0;

  { SCOPED_TRACE("allocate_and_reuse");
    // We should be careful about stack offsets
    allocate_and_reuse(&addr_on_stack);
    allocate_and_reuse(&addr_on_stack);
  }
}
