/*
 * Copyright (C) 2019 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

// A Neon vectorized implementation of the GNU symbol hash function.

// This function generally accesses beyond the bounds of the name string. Specifically, it reads
// each aligned 8-byte chunk containing a byte of the string, including the final NUL byte. This
// should be acceptable for use with MTE, which uses 16-byte granules. Typically, the function is
// used to hash strings in an ELF file's string table, where MTE is presumably unaware of the
// bounds of each symbol, but the linker also hashes the symbol name passed to dlsym.

#include <arm_neon.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct __attribute__((aligned(8))) GnuHashInitEntry {
  uint64_t ignore_mask;
  uint32_t accum;
};

constexpr uint32_t kStep0 = 1;
constexpr uint32_t kStep1 = kStep0 * 33;
constexpr uint32_t kStep2 = kStep1 * 33;
constexpr uint32_t kStep3 = kStep2 * 33;
constexpr uint32_t kStep4 = kStep3 * 33;
constexpr uint32_t kStep5 = kStep4 * 33;
constexpr uint32_t kStep6 = kStep5 * 33;
constexpr uint32_t kStep7 = kStep6 * 33;
constexpr uint32_t kStep8 = kStep7 * 33;
constexpr uint32_t kStep9 = kStep8 * 33;
constexpr uint32_t kStep10 = kStep9 * 33;
constexpr uint32_t kStep11 = kStep10 * 33;

// Calculate the GNU hash and string length of the symbol name.
__attribute__((noinline))
static uint64_t calculate_gnu_hash_neon(const char* name) {
  // Generated using (TODO: copy in the program that generates these constants)
  static const struct GnuHashInitEntry kInitTable[] = {
    { // (addr&7) == 0
      0ull,
      0x00001505u
    }, { // (addr&7) == 1
      0xffull,
      0x7c1f0865u + 0x2e8ba2e1,
    }, { // (addr&7) == 2
      0xffffull,
      0xec7d0fc5u + 0x2e8ba2e1 + 0x66424ac1,
    }, { // (addr&7) == 3
      0xffffffull,
      0x7b87ab25u + 0x2e8ba2e1 + 0x66424ac1 + 0xebd376a1,
    }, { // (addr&7) == 4
      0xffffffffull,
      0xfbfc5a85u + 0x2e8ba2e1 + 0x66424ac1 + 0xebd376a1 + 0x6bfea681,
    }, { // (addr&7) == 5
      0xffffffffffull,
      0x1ee89de5u + 0x2e8ba2e1 + 0x66424ac1 + 0xebd376a1 + 0x6bfea681 + 0x39935a61,
    }, { // (addr&7) == 6
      0xffffffffffffull,
      0xe9a9f545u + 0x2e8ba2e1 + 0x66424ac1 + 0xebd376a1 + 0x6bfea681 + 0x39935a61 + 0xcb711241,
    }, { // (addr&7) == 7
      0xffffffffffffffull,
      0x6bede0a5u + 0x2e8ba2e1 + 0x66424ac1 + 0xebd376a1 + 0x6bfea681 + 0x39935a61 + 0xcb711241 + 0x7a874e21,
    },
  };

  uint8_t offset = reinterpret_cast<uintptr_t>(name) & 7;
  const uint64_t* chunk_ptr = reinterpret_cast<const uint64_t*>(reinterpret_cast<uintptr_t>(name) & ~7);
  const struct GnuHashInitEntry* entry = &kInitTable[offset];

  uint8x8_t chunk = vld1_u8(reinterpret_cast<const uint8_t*>(chunk_ptr));
  chunk |= vld1_u8(reinterpret_cast<const uint8_t*>(&entry->ignore_mask));

  uint32x4_t accum_lo = { 0 };
  uint32x4_t accum_hi = { entry->accum, 0, 0, 0 };
  const uint16x4_t kInclineVec = { kStep3, kStep2, kStep1, kStep0 };
  const uint32_t kStep8 = (33u * 33 * 33 * 33) * (33u * 33 * 33 * 33);
  const uint32x4_t kStep8Vec = vdupq_n_u32(kStep8);
  uint8x8_t is_nul;
  uint16x8_t expand;

  while (1) {
    is_nul = vceq_u8(chunk, (uint8x8_t){ 0 });
    expand = vmovl_u8(chunk);
    uint64x1_t is_nul_64 = vreinterpret_u64_u8(is_nul);
    if (vget_lane_u64(is_nul_64, 0)) break;

    accum_lo = vmulq_u32(accum_lo, kStep8Vec);
    accum_hi = vmulq_u32(accum_hi, kStep8Vec);
    accum_lo = vmlal_u16(accum_lo, vget_low_u16(expand), kInclineVec);
    accum_hi = vmlal_u16(accum_hi, vget_high_u16(expand), kInclineVec);

    chunk = vld1_u8(reinterpret_cast<const uint8_t*>(++chunk_ptr));
  }

  is_nul = vrev64_u8(is_nul);
  expand = vrev64q_u16(expand);

#if defined(__arm__)
  // This inline assembly is faster with both GCC and Clang.
  uint32_t is_nul_hi = vget_lane_u32(vreinterpret_u32_u8(is_nul), 0);
  uint32_t is_nul_lo = vget_lane_u32(vreinterpret_u32_u8(is_nul), 1);
  asm("clz %1, %1; "
      "add %1, #32; "
      "cmp %0, #0; "
      "clz %0, %0; "
      "it eq; "
      "moveq %0, %1; "
       : "+r"(is_nul_lo), "+r"(is_nul_hi) :: "cc");
  const uint32_t num_valid_bits = is_nul_lo;
#else
  const uint64_t is_nul_u64 = vget_lane_u64(vreinterpret_u64_u8(is_nul), 0);
  const uint32_t num_valid_bits = is_nul_u64 == 0 ? 64 : __builtin_clzll(is_nul_u64);
#endif

  uint32_t name_len = reinterpret_cast<const char*>(chunk_ptr) - name + (num_valid_bits >> 3);

  static const uint32_t kFinalStepTable[] = {
    kStep4, kStep0,
    kStep5, kStep1,
    kStep6, kStep2,
    kStep7, kStep3,
    kStep8, kStep4,
    kStep9, kStep5,
    kStep10, kStep6,
    kStep11, kStep7,
  };

  const uint32_t* final_step = &kFinalStepTable[num_valid_bits >> 2];
  accum_lo = vmulq_u32(accum_lo, vdupq_n_u32(final_step[0]));
  accum_lo = vmlaq_u32(accum_lo, accum_hi, vdupq_n_u32(final_step[1]));

  static const uint32_t kFinalInclineTable[] = {
    0,      0,      0,      0,      0,      0,      0,      0,
    kStep0, kStep1, kStep2, kStep3, kStep4, kStep5, kStep6,
  };

  const uint32_t* const incline_rev = &kFinalInclineTable[num_valid_bits >> 3];
  const uint32x4_t incline_rev_hi = vld1q_u32(incline_rev);
  const uint32x4_t incline_rev_lo = vld1q_u32(incline_rev + 4);

  accum_lo = vmlaq_u32(accum_lo, vmovl_u16(vget_low_u16(expand)), incline_rev_lo);
  accum_lo = vmlaq_u32(accum_lo, vmovl_u16(vget_high_u16(expand)), incline_rev_hi);

  uint32x2_t sum = vadd_u32(vget_low_u32(accum_lo), vget_high_u32(accum_lo));
  const uint32_t hash = sum[0] + sum[1];

  union {
    struct { uint32_t lo; uint32_t hi; } s;
    uint64_t u64;
  } u;
  u.s.lo = hash;
  u.s.hi = name_len;
  return u.u64;
}
