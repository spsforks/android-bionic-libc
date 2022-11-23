/*
 * Copyright (C) 2020 The Android Open Source Project
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

#pragma once

#include <stddef.h>
#include <sys/auxv.h>
#include <sys/prctl.h>

// Note: Most PR_MTE_* constants come from the upstream kernel. This tag mask
// allows for the hardware to provision any nonzero tag. Zero tags are reserved
// for scudo to use for the chunk headers in order to prevent linear heap
// overflow/underflow.
#define PR_MTE_TAG_SET_NONZERO (0xfffeUL << PR_MTE_TAG_SHIFT)

constexpr size_t kTagGranuleSize = 16;

inline bool mte_supported() {
#if defined(__aarch64__)
  static bool supported = getauxval(AT_HWCAP2) & HWCAP2_MTE;
#else
  static bool supported = false;
#endif
  return supported;
}

inline void* get_tagged_address(const void* ptr) {
#if defined(__aarch64__)
  if (mte_supported()) {
    __asm__ __volatile__(".arch_extension mte; ldg %0, [%0]" : "+r"(ptr));
  }
#endif  // aarch64
  return const_cast<void*>(ptr);
}

// Inserts a random tag tag to `ptr`, using any of the set lower 16 bits in
// `mask` to exclude the corresponding tag from being generated. Note: This does
// not tag memory.
inline void* insert_random_tag(const void* ptr, __attribute__((unused)) uint64_t mask = 0) {
#if defined(__aarch64__)
  if (mte_supported() && ptr) {
    __asm__ __volatile__(".arch_extension mte; irg %0, %0, %1" : "+r"(ptr) : "r"(mask));
  }
#endif  // aarch64
  return const_cast<void*>(ptr);
}

// Stores the address tag in `ptr` to memory, at `ptr`.
inline void set_memory_tag(__attribute__((unused)) void* ptr) {
#if defined(__aarch64__)
  if (mte_supported()) {
    __asm__ __volatile__(".arch_extension mte; stg %0, [%0]" : "+r"(ptr));
  }
#endif  // aarch64
}

#ifdef __aarch64__
class ScopedDisableMTE {
  size_t prev_tco_;

 public:
  ScopedDisableMTE() {
    if (mte_supported()) {
      __asm__ __volatile__(".arch_extension mte; mrs %0, tco; msr tco, #1" : "=r"(prev_tco_));
    }
  }

  ~ScopedDisableMTE() {
    if (mte_supported()) {
      __asm__ __volatile__(".arch_extension mte; msr tco, %0" : : "r"(prev_tco_));
    }
  }
};
#else
struct ScopedDisableMTE {
  // Silence unused variable warnings in non-aarch64 builds.
  ScopedDisableMTE() {}
};
#endif
