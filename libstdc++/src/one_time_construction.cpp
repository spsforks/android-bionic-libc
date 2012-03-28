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

/* Even gcc defines the GUARD_BIT, it's actually mapped to byte */
#define GUARD_BIT       0x00001
#define GUARD_PENDING   0x00100
#define GUARD_WAITING   0x10000

extern "C" int __cxa_guard_acquire(int volatile * gv)
{
    // 0 -> pending, return 1
    // pending -> waiting, wait and return 0
    // waiting: untouched, wait and return 0
    // ready: untouched, return 0
    unsigned int ready = GUARD_BIT;
    unsigned int pending = GUARD_PENDING;
    unsigned int waiting = GUARD_WAITING;

retry:
    if (__atomic_cmpxchg(0, pending, gv) == 0) {
        ANDROID_MEMBAR_FULL();
        return 1;
    }
    __atomic_cmpxchg(pending, waiting, gv); // Indicate there is a waiter
    __futex_wait(gv, waiting, NULL);

    if(*gv != ready) // __cxa_guard_abort was called, let every thread try since there is no return code for this condition
        goto retry;

    ANDROID_MEMBAR_FULL();
    return 0;
}

extern "C" void __cxa_guard_release(int volatile * gv)
{
    // pending -> ready
    // waiting -> ready, and wake
    unsigned int ready = GUARD_BIT;
    unsigned int pending = GUARD_PENDING;

    ANDROID_MEMBAR_FULL();
    if (__atomic_cmpxchg(pending, ready, gv) == 0) {
        return;
    }

    *gv = ready;
    __futex_wake(gv, 0x7fffffff);
}

extern "C" void __cxa_guard_abort(int volatile * gv)
{
    ANDROID_MEMBAR_FULL();
    *gv = 0;
    __futex_wake(gv, 0x7fffffff);
}
