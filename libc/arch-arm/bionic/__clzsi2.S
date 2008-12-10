/*
 * Copyright (C) 2008 The Android Open Source Project
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
 *
 * Binary search implementation of clz (count leading zeroes)
 */

#include <machine/cpu-features.h>

        .text
        .type __clzsi, #function
        .global __clzsi
        .align 4        

#ifndef __ARM_HAVE_CLZ
__clzsi:
        mov     r3, r0, lsr #16
        cmp     r3, #0
        movne   r0, r3
        mov     r2, r0, lsr #8
        moveq   r3, #16
        movne   r3, #0
        tst     r2, #255
        movne   r0, r2
        mov     r2, r0, lsr #4
        addeq   r3, r3, #8
        tst     r2, #15
        movne   r0, r2
        mov     r2, r0, lsr #2
        addeq   r3, r3, #4
        tst     r2, #3
        movne   r0, r2
        mov     r2, r0, lsr #1
        addeq   r3, r3, #2
        tst     r2, #1
        movne   r0, r2
        addeq   r3, r3, #1
        cmp     r0, #0
        addeq   r3, r3, #1
        mov     r0, r3
        bx      lr
#else
        clz     r0, r0
        bx      lr
#endif
