/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 * Copyright (c) 2013 NVIDIA Corporation.  All rights reserved.
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

#define CACHE_LINE_SIZE         64
#define MEMCPY_BLOCK_SIZE_SMALL (32768)
#define MEMCPY_BLOCK_SIZE_MID   (1048576)
#define PREFETCH_DISTANCE_NEAR  (CACHE_LINE_SIZE*4)
#define PREFETCH_DISTANCE_MID   (CACHE_LINE_SIZE*4)
#define PREFETCH_DISTANCE_FAR   (CACHE_LINE_SIZE*16)

ENTRY(MEMCPY_BASE)
        .save   {r0, lr}
        .cfi_def_cfa_offset 8
        .cfi_rel_offset r0, 0
        .cfi_rel_offset lr, 4

        cmp         r2, #0
        it          ne
        cmpne       r0, r1
        it          eq
        popeq       {r0, pc}

        /* preload next cache line */
        pld         [r1, #0]
        pld         [r1, #CACHE_LINE_SIZE*1]

memcpy_align_dest:
        /* Deal with very small blocks (< 32bytes) asap */
        cmp         r2, #32
        blo         memcpy_lt_32bytes
        /* no need to align if len < 128 bytes */
        cmp         r2, #128
        blo         memcpy_lt_128bytes
        /* align destination to 64 bytes */
        rsb         r3, r0, #0
        ands        r3, r3, #0x3f
        beq         memcpy_dispatch
        sub         r2, r2, r3
0:      /* copy 1 byte */
        movs        ip, r3, lsl #31
        itt         mi
        ldrbmi      ip, [r1], #1
        strbmi      ip, [r0], #1
1:      /* copy 2 bytes */
        itttt       cs
        ldrbcs      ip, [r1], #1
        strbcs      ip, [r0], #1
        ldrbcs      ip, [r1], #1
        strbcs      ip, [r0], #1
2:      /* copy 4 bytes */
        movs        ip, r3, lsl #29
        bpl         3f
        vld4.8      {d0[0], d1[0], d2[0], d3[0]}, [r1]!
        vst4.8      {d0[0], d1[0], d2[0], d3[0]}, [r0, :32]!
3:      /* copy 8 bytes */
        bcc         4f
        vld1.8      {d0}, [r1]!
        vst1.8      {d0}, [r0, :64]!
4:      /* copy 16 bytes */
        movs        ip, r3, lsl #27
        bpl         5f
        vld1.8      {q0}, [r1]!
        vst1.8      {q0}, [r0, :128]!
5:      /* copy 32 bytes */
        bcc         memcpy_dispatch
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!

memcpy_dispatch:
        /* preload more cache lines */
        pld         [r1, #CACHE_LINE_SIZE*2]
        pld         [r1, #CACHE_LINE_SIZE*3]

        cmp         r2, #MEMCPY_BLOCK_SIZE_SMALL
        blo         memcpy_neon_pld_near
        cmp         r2, #MEMCPY_BLOCK_SIZE_MID
        blo         memcpy_neon_pld_mid
        /* Is src and dst word-aligned? */
        ands        r3, r1, #0x3
        beq         memcpy_ldrd_strd_pld_far
        b           memcpy_neon_pld_far

memcpy_neon_pld_near:
        /* less than 128 bytes? */
        subs        r2, r2, #128
        blo         1f
        .align      4
0:
        /* copy 128 bytes in each loop */
        subs        r2, r2, #128

        /* preload a cache line to cache */
        pld         [r1, #PREFETCH_DISTANCE_NEAR]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!

        /* preload a cache line to cache */
        pld         [r1, #PREFETCH_DISTANCE_NEAR]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!

        bhs         0b
1:
        adds        r2, r2, #128
        bne         memcpy_lt_128bytes
        pop         {r0, pc}

memcpy_neon_pld_mid:
        sub         r2, r2, #128
        .align      4
0:
        /* copy 128 bytes in each loop */
        subs        r2, r2, #128

        /* preload a cache line to cache */
        pld         [r1, #PREFETCH_DISTANCE_MID]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!

        /* preload a cache line to cache */
        pld         [r1, #PREFETCH_DISTANCE_MID]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!

        bhs         0b
1:
        adds        r2, r2, #128
        bne         memcpy_lt_128bytes
        pop         {r0, pc}

memcpy_neon_pld_far:
        sub         r2, r2, #128
        .align      4
0:
        /* copy 128 bytes in each loop */
        subs        r2, r2, #128

        /* preload a cache line to cache */
        pld         [r1, #PREFETCH_DISTANCE_FAR]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!

        /* preload a cache line to cache */
        pld         [r1, #PREFETCH_DISTANCE_FAR]
        /* copy a cache line */
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0, :256]!

        bhs         0b
1:
        adds        r2, r2, #128
        bne         memcpy_lt_128bytes
        pop         {r0, pc}

memcpy_ldrd_strd_pld_far:
        sub         r2, r2, #128
        strd        r4, r5, [sp, #-8]!
        .save       {r4, r5}
        .cfi_adjust_cfa_offset 8
        .cfi_rel_offset r4, 0
        .cfi_rel_offset r5, 4
        .align      4
0:
        /* copy 128 bytes in each loop */
        subs        r2, r2, #128

        /* preload a cache line to cache */
        pld         [r1, #PREFETCH_DISTANCE_FAR]
        /* copy a cache line */
        .irp        offset, #0, #8, #16, #24, #32, #40, #48, #56
        ldrd        r4, r5, [r1, \offset]
        strd        r4, r5, [r0, \offset]
        .endr

        /* preload a cache line to cache */
        pld         [r1, #PREFETCH_DISTANCE_FAR+CACHE_LINE_SIZE]
        /* copy a cache line */
        .irp        offset, #64, #72, #80, #88, #96, #104, #112, #120
        ldrd        r4, r5, [r1, \offset]
        strd        r4, r5, [r0, \offset]
        .endr

        add         r0, r0, #128
        add         r1, r1, #128
        bhs         0b
1:
        adds        r2, r2, #128
        ldrd        r4, r5, [sp], #8
        bne         memcpy_lt_128bytes
        pop         {r0, pc}

memcpy_lt_128bytes:
6:      /* copy 64 bytes */
        movs        ip, r2, lsl #26
        bcc         5f
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0]!
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0]!
5:      /* copy 32 bytes */
        bpl         4f
        vld1.8      {q0, q1}, [r1]!
        vst1.8      {q0, q1}, [r0]!
memcpy_lt_32bytes:
4:      /* copy 16 bytes */
        movs        ip, r2, lsl #28
        bcc         3f
        vld1.8      {q0}, [r1]!
        vst1.8      {q0}, [r0]!
3:      /* copy 8 bytes */
        bpl         2f
        vld1.8      {d0}, [r1]!
        vst1.8      {d0}, [r0]!
2:      /* copy 4 bytes */
        ands        ip, r2, #0x4
        beq         1f
        vld4.8      {d0[0], d1[0], d2[0], d3[0]}, [r1]!
        vst4.8      {d0[0], d1[0], d2[0], d3[0]}, [r0]!
1:      /* copy 2 bytes */
        movs        ip, r2, lsl #31
        itttt       cs
        ldrbcs      ip, [r1], #1
        strbcs      ip, [r0], #1
        ldrbcs      ip, [r1], #1
        strbcs      ip, [r0], #1
0:      /* copy 1 byte */
        itt         mi
        ldrbmi      ip, [r1, #0]
        strbmi      ip, [r0, #0]

        pop         {r0, pc}
END(MEMCPY_BASE)
