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

#undef MD_SWAP       // Don't use implicit machine optimizations
#include <endian.h>

/*
 * This test checks that
 *   __swapXX() is not equal to
 *   __swapXXmd().
 */
TEST(endian, define_md_swap)
{
    #ifdef MD_SWAP
    FAIL() << "MD_SWAP is defined";
    #endif
}

TEST(endian, swap16_swap16md)
{
    uint16_t word16 = 0x0001;
    uint16_t word16_machine = word16;
    uint16_t backup = word16;

    { SCOPED_TRACE("__swap16() swap");
        // Compiler version
        word16 = __swap16(word16);
        EXPECT_FALSE((word16) & 0xFF);
        EXPECT_TRUE((word16 >> 8) & 0xFF);
    }
    { SCOPED_TRACE("__swap16()md swap");
        // Machine version
        word16_machine = __swap16md(word16_machine);
        EXPECT_FALSE((word16_machine) & 0xFF);
        EXPECT_TRUE((word16_machine >> 8) & 0xFF);
    }

    { SCOPED_TRACE("__swap16() re-swap");
        // Compiler version
        word16 = __swap16(word16);
        EXPECT_EQ(backup, word16);
    }
    { SCOPED_TRACE("__swap16md() re-swap");
        // Machine version
        word16_machine = __swap16(word16_machine);
        EXPECT_EQ(backup, word16_machine);
    }
}

TEST(endian, swap32_swap32md)
{
    uint32_t word32 = 0x0B00B501;
    uint32_t word32_machine = word32;
    uint32_t word32_inv = 0x01B5000B;

    { SCOPED_TRACE("__swap32() swap");
        // Compiler version
        word32 = __swap32(word32);
        EXPECT_EQ(word32_inv, word32);
    }
    { SCOPED_TRACE("__swap32md() swap");
        // Machine version
        word32_machine = __swap32md(word32_machine);
        EXPECT_EQ(word32_inv, word32_machine);
    }
}

TEST(endian, swap64_swap64md)
{
    uint64_t word64 = 0x0B00BEEFB501DEAD;
    uint64_t word64_machine = word64;
    uint64_t word64_inv = 0xADDE01B5EFBE000B;

    { SCOPED_TRACE("__swap64() swap");
        // Compiler version
        word64 = __swap64(word64);
        EXPECT_EQ(word64_inv, word64);
    }
    { SCOPED_TRACE("__swap64md() swap");
        // Machine version
        word64_machine = __swap64md(word64_machine);
        EXPECT_EQ(word64_inv, word64_machine);
    }
}

