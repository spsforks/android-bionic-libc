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

#include <byteswap.h>

#include <gtest/gtest.h>

TEST(byteswap, bswap_16)
{
    uint16_t word16 = 0x0001;
    uint16_t backup = word16;

    { SCOPED_TRACE("bswap_16() swap");
        // Endian swap expected
        word16 = bswap_16(word16);
        EXPECT_FALSE((word16) & 0xFF);
        EXPECT_TRUE((word16 >> 8) & 0xFF);
    }

    { SCOPED_TRACE("bswap_16() re-swap");
        // Endian restore expected
        word16 = bswap_16(word16);
        EXPECT_EQ(backup, word16);
    }
}

TEST(byteswap, bswap_32)
{
    uint32_t word32     = 0x0B00B501;
    uint32_t word32_inv = 0x01B5000B;

    { SCOPED_TRACE("bswap_32() swap");
        // Endian swap expected
        word32 = bswap_32(word32);
        EXPECT_EQ(word32_inv, word32);
    }
}

TEST(byteswap, bswap_64)
{
    uint64_t word64     = 0x0B00BEEFB501DEAD;
    uint64_t word64_inv = 0xADDE01B5EFBE000B;

    { SCOPED_TRACE("bswap_64() swap");
        // Endian swap expected
        word64 = bswap_64(word64);
        EXPECT_EQ(word64_inv, word64);
    }
}

