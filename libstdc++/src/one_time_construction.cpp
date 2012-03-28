/*
 * one_time_construction.cpp
 *
 * Copyright 2006 The Android Open Source Project
 *
 * This file contains C++ ABI support functions for one time
 * constructors as defined in the "Run-time ABI for the ARM Architecture"
 * section 4.4.2
 */

#include <stddef.h>
#include <sys/atomics.h>
#include <bionic_futex.h>
#include <bionic_atomic_inline.h>

// NOTE: XXX
// If endian is supported in bionic, this define need be updated.
#ifndef le32_to_cpu
#define le32_to_cpu(x)    (x)
#endif

#if __arm__
// For arm, check the LSB of 32 bit guard variable.
typedef uint32_t _guard_t;
#else
// For others, check the first byte of 64bit guard variable
typedef uint64_t _guard_t;
#endif

// if _guard_t is 64bit, it can't be used by futex directly.
// union instead of cast could make sure gv_v and gv_t share
// the same "first byte" for ARM vs others and LE vs BE.
union _guard_u {
    _guard_t    gv_m;   // _guard_t for gv memory
    int         gv_i;   // int for futex
};

const static int ready     = le32_to_cpu(0x1);
const static int pending   = le32_to_cpu(0x100);
const static int waiting   = le32_to_cpu(0x10000);

extern "C" int __cxa_guard_acquire(_guard_t volatile * gv)
{
    // 0 -> pending, return 1
    // pending -> waiting, wait and return 0
    // waiting: untouched, wait and return 0
    // ready: untouched, return 0

    union _guard_u *gv_u = (union _guard_u *)gv;

retry:
    if (__atomic_cmpxchg(0, pending, &gv_u->gv_i) == 0) {
        ANDROID_MEMBAR_FULL();
        return 1;
    }
    __atomic_cmpxchg(pending, waiting, &gv_u->gv_i); // Indicate there is a waiter
    __futex_wait(&gv_u->gv_i, waiting, NULL);

    if (gv_u->gv_i != ready) // __cxa_guard_abort was called, let every thread try since there is no return code for this condition
        goto retry;

    ANDROID_MEMBAR_FULL();
    return 0;
}

extern "C" void __cxa_guard_release(_guard_t volatile * gv)
{
    // pending -> ready
    // waiting -> ready, and wake
    union _guard_u *gv_u = (union _guard_u *)gv;

    ANDROID_MEMBAR_FULL();
    if (__atomic_cmpxchg(pending, ready, &gv_u->gv_i) == 0) {
        return;
    }

    gv_u->gv_i = ready;
    __futex_wake(&gv_u->gv_i, 0x7fffffff);
}

extern "C" void __cxa_guard_abort(int volatile * gv)
{
    union _guard_u *gv_u = (union _guard_u *)gv;

    ANDROID_MEMBAR_FULL();
    gv_u->gv_i = 0;
    __futex_wake(&gv_u->gv_i, 0x7fffffff);
}
