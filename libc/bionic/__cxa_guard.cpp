/*
 * Copyright (C) 2006 The Android Open Source Project
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

#include <stddef.h>
#include <endian.h>

#include "private/bionic_atomic_inline.h"
#include "private/bionic_futex.h"

// This file contains C++ ABI support functions for one time
// constructors as defined in the "Run-time ABI for the ARM Architecture"
// section 4.4.2
//
// ARM C++ ABI and Itanium/x86 C++ ABI has different definition for
// one time construction:
//
//    ARM C++ ABI defines the LSB of guard variable should be tested
//    by compiler-generated code before calling __cxa_guard_acquire et al.
//
//    The Itanium/x86 C++ ABI defines the low-order _byte_ should be
//    tested instead.
//
//    Meanwhile, guard variable are 32bit aligned for ARM, and 64bit
//    aligned for x86.
//
// Reference documentation:
//
//    section 3.2.3 of ARM IHI 0041C (for ARM)
//    section 3.3.2 of the Itanium C++ ABI specification v1.83 (for x86).
//
// There is no C++ ABI available for other ARCH. But the gcc source
// shows all other ARCH follow the definition of Itanium/x86 C++ ABI.

#if defined(__arm__)
// The ARM C++ ABI mandates that guard variables are 32-bit aligned, 32-bit
// values. The LSB is tested by the compiler-generated code before calling
// __cxa_guard_acquire.
union _guard_t {
  int volatile state;
  int32_t aligner;
};

const static int ready = 0x1;
const static int pending = 0x2;
const static int waiting = 0x6;

#else
// The Itanium/x86 C++ ABI (used by all other architectures) mandates that
// guard variables are 64-bit aligned, 64-bit values. The LSB is tested by
// the compiler-generated code before calling __cxa_guard_acquire.
union _guard_t {
  int volatile state;
  int64_t aligner;
};

const static int ready = letoh32(0x1);
const static int pending = letoh32(0x100);
const static int waiting = letoh32(0x10000);
#endif

extern "C" int __cxa_guard_acquire(_guard_t* gv) {
// 0 -> pending, return 1
// pending -> waiting, wait and return 0
// waiting: untouched, wait and return 0
// ready: untouched, return 0

retry:
  if (__bionic_cmpxchg(0, pending, &gv->state) == 0) {
    ANDROID_MEMBAR_FULL();
    return 1;
  }
  __bionic_cmpxchg(pending, waiting, &gv->state);  // Indicate there is a waiter
  __futex_wait(&gv->state, waiting, NULL);

  if (gv->state != ready) {
    // __cxa_guard_abort was called, let every thread try since there is no
    // return code for this condition
    goto retry;
  }

  ANDROID_MEMBAR_FULL();
  return 0;
}

extern "C" void __cxa_guard_release(_guard_t* gv) {
  // pending -> ready
  // waiting -> ready, and wake

  ANDROID_MEMBAR_FULL();
  if (__bionic_cmpxchg(pending, ready, &gv->state) == 0) {
    return;
  }

  gv->state = ready;
  __futex_wake(&gv->state, 0x7fffffff);
}

extern "C" void __cxa_guard_abort(_guard_t* gv) {
  ANDROID_MEMBAR_FULL();
  gv->state = 0;
  __futex_wake(&gv->state, 0x7fffffff);
}
