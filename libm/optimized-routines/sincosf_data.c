/*
 * Copyright (c) 2018, Arm Limited.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !WANT_SINGLEPREC

#include <stdint.h>
#include <math.h>
#include "math_config.h"
#include "sincosf.h"

/* The constants and polynomials for sine and cosine.  The 2nd entry
   computes -cos (x) rather than cos (x) to get negation for free.  */
sincos_t sincosf_table[2] =
{
  {
    { 1.0, -1.0, -1.0, 1.0 },
#if HAVE_FAST_ROUND
    0x1.45F306DC9C883p-1,
#else
    0x1.45F306DC9C883p+23,
#endif
    0x1.921FB54442D18p0,
    0x1p0,
    -0x1.ffffffd0c621cp-2,
    0x1.55553e1068f19p-5,
    -0x1.6c087e89a359dp-10,
    0x1.99343027bf8c3p-16,
    -0x1.555545995a603p-3,
    0x1.1107605230bc4p-7,
    -0x1.994eb3774cf24p-13
  },
  {
    { 1.0, -1.0, -1.0, 1.0 },
#if HAVE_FAST_ROUND
    0x1.45F306DC9C883p-1,
#else
    0x1.45F306DC9C883p+23,
#endif
    0x1.921FB54442D18p0,
    -0x1p0,
    0x1.ffffffd0c621cp-2,
    -0x1.55553e1068f19p-5,
    0x1.6c087e89a359dp-10,
    -0x1.99343027bf8c3p-16,
    -0x1.555545995a603p-3,
    0x1.1107605230bc4p-7,
    -0x1.994eb3774cf24p-13
  }
};

/* Table with 4/PI to 192 bit precision.  To avoid unaligned accesses
   only 8 new bits are added per entry, making the table 4 times larger.  */
const uint32_t inv_pio4[24] =
{
  0xa2,       0xa2f9,	  0xa2f983,   0xa2f9836e,
  0xf9836e4e, 0x836e4e44, 0x6e4e4415, 0x4e441529,
  0x441529fc, 0x1529fc27, 0x29fc2757, 0xfc2757d1,
  0x2757d1f5, 0x57d1f534, 0xd1f534dd, 0xf534ddc0,
  0x34ddc0db, 0xddc0db62, 0xc0db6295, 0xdb629599,
  0x6295993c, 0x95993c43, 0x993c4390, 0x3c439041
};

#endif
