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

#include <private/bionic_asm.h>

    .text
    .syntax unified
    .fpu neon
    .arch armv7-a

#define kStep0  1
#define kStep1  (0xffffffff & (kStep0 * 33))
#define kStep2  (0xffffffff & (kStep1 * 33))
#define kStep3  (0xffffffff & (kStep2 * 33))
#define kStep4  (0xffffffff & (kStep3 * 33))
#define kStep5  (0xffffffff & (kStep4 * 33))
#define kStep6  (0xffffffff & (kStep5 * 33))
#define kStep7  (0xffffffff & (kStep6 * 33))
#define kStep8  (0xffffffff & (kStep7 * 33))
#define kStep9  (0xffffffff & (kStep8 * 33))
#define kStep10 (0xffffffff & (kStep9 * 33))
#define kStep11 (0xffffffff & (kStep10 * 33))

#define qExpand16   q2
#define qAccLo      q8
#define qAccHi      q9
#define qStep8      q15

#define dChunk          d0
#define dInitialMask    d1
#define dInclineVec     d2
#define dExpand16_0     d4
#define dExpand16_1     d5
#define dTmp1           d6
#define dAccLo_0        d16
#define dAccLo_1        d17
#define dAccHi_0        d18
#define dAccHi_1        d19

#define rInputStr       r0
#define rPtr            r1
#define rNumPadBytes    r2
#define rInitialAccum   r2
#define rIsNulLo        r2
#define rIsNulHi        r3

    .code 16
    .thumb_func

    .p2align 4
ENTRY(calculate_gnu_hash_asm)
    // Round the pointer down to an aligned 8-byte address. Load the chunk's value. Accommodate
    // misaligned pointers:
    //  - Set the initial padding bytes to 0xff in the vector.
    //  - Load a u32 initial accumulator that (a) adds 5381*33**K and (b) cancels out the
    //    0xff*33**K contributions.
    // If we could set bytes to NUL instead, then the accumulator wouldn't need to cancel out the
    // 0xff*33**K values, but this would break the NUL check.

    and rNumPadBytes, rInputStr, #7
    bic rPtr, rInputStr, #7
    adr ip, .L_InitialTable
    add ip, rNumPadBytes, lsl #4

    vld1.8 {dChunk}, [rPtr :64]!
    vld1.8 {dInitialMask}, [ip :64]
    vorr.8 dChunk, dInitialMask

    ldr rInitialAccum, [ip, #8]
    vmov.u32 qAccHi, #0
    vmov.u32 qAccLo, #0

    adr ip, .L_kInlineVec
    vld1.u16 dInclineVec, [ip :64]
    vmov.32 dAccHi_0[0], rInitialAccum
    movw ip, #(kStep8 & 0xffff)
    movt ip, #(kStep8 >> 16)
    vdup.32 qStep8, ip

    // Check for a NUL byte in the first 8-byte chunk.
    vceq.i8 dTmp1, dChunk, #0
    vmovl.u8 qExpand16, dChunk
    vmov rIsNulLo, rIsNulHi, dTmp1
    orrs rIsNulLo, rIsNulHi
    bne .L_loop_exit

.L_loop_top:
    // First multiply the two accumulators by 33**8. Multiply each half of the chunk by the
    // "incline" vector {33**3, 33**2, 33**1, 33**0}, and accumulate it. The lower accumulator
    // will be multiplied by an extra 33**4 at the end. Start loading the next chunk.
    vmul.u32 qAccLo, qAccLo, qStep8
    vmul.u32 qAccHi, qAccHi, qStep8
    vld1.8 {dChunk}, [rPtr :64]!
    vmlal.u16 qAccLo, dExpand16_0, dInclineVec
    vmlal.u16 qAccHi, dExpand16_1, dInclineVec

    // Check for NUL bytes in the next chunk, and expand it.
    vceq.i8 dTmp1, dChunk, #0
    vmovl.u8 qExpand16, dChunk
    vmov rIsNulLo, rIsNulHi, dTmp1
    orrs rIsNulLo, rIsNulHi
    beq .L_loop_top

.L_loop_exit:
#define rStringLength       r0
#define rFinalValidBits     r1
#define rStepLo             r2
#define rStepHi             r3
#define qExpandLo           q10
#define qExpandHi           q11
#define qInclineLo          q12
#define qInclineHi          q13
#define qStepLo             q14
#define qStepHi             q15

    vrev64.8 dTmp1, dTmp1
    vmov rIsNulHi, rIsNulLo, dTmp1
    sub rStringLength, rPtr, rInputStr
    sub rStringLength, #8

    vrev64.16 qExpand16, qExpand16

    clz rFinalValidBits, rIsNulHi
    add rFinalValidBits, #32
    clz ip, rIsNulLo
    cmp rIsNulLo, #0
    it ne
    movne rFinalValidBits, ip

    adr ip, .L_FinalStepTable
    add ip, ip, rFinalValidBits
    vmovl.u16 qExpandLo, dExpand16_0
    ldrd rStepLo, rStepHi, [ip]
    vmovl.u16 qExpandHi, dExpand16_1
    vdup.32 qStepLo, rStepLo
    vdup.32 qStepHi, rStepHi

    adr ip, .L_FinalInclineTable
    add ip, ip, rFinalValidBits, lsr #1
    vld1.u32 {qInclineLo, qInclineHi}, [ip]

    vmul.u32 qAccLo, qAccLo, qStepLo
    vmla.u32 qAccLo, qAccHi, qStepHi
    vmla.u32 qAccLo, qExpandLo, qInclineHi
    vmla.u32 qAccLo, qExpandHi, qInclineLo

    // Sum all qAccLo/qAccHi pieces.
    vadd.u32 dAccLo_0, dAccLo_1
    add r1, rStringLength, rFinalValidBits, lsr #3
    vmov r0, ip, dAccLo_0
    add r0, ip

    bx lr

    .p2align 3
.L_InitialTable:
    // (addr&7) == 0
    .long 0, 0
    .long 0xffffffff & (0x00001505)
    .zero 4
    // (addr&7) == 1
    .long 0xff, 0
    .long 0xffffffff & (0x7c1f0865 + 0x2e8ba2e1)
    .zero 4
    // (addr&7) == 2
    .long 0xffff, 0
    .long 0xffffffff & (0xec7d0fc5 + 0x2e8ba2e1 + 0x66424ac1)
    .zero 4
    // (addr&7) == 3
    .long 0xffffff, 0
    .long 0xffffffff & (0x7b87ab25 + 0x2e8ba2e1 + 0x66424ac1 + 0xebd376a1)
    .zero 4
    // (addr&7) == 4
    .long 0xffffffff, 0
    .long 0xffffffff & (0xfbfc5a85 + 0x2e8ba2e1 + 0x66424ac1 + 0xebd376a1 + 0x6bfea681)
    .zero 4
    // (addr&7) == 5
    .long 0xffffffff, 0xff
    .long 0xffffffff & (0x1ee89de5 + 0x2e8ba2e1 + 0x66424ac1 + 0xebd376a1 + 0x6bfea681 + 0x39935a61)
    .zero 4
    // (addr&7) == 6
    .long 0xffffffff, 0xffff
    .long 0xffffffff & (0xe9a9f545 + 0x2e8ba2e1 + 0x66424ac1 + 0xebd376a1 + 0x6bfea681 + 0x39935a61 + 0xcb711241)
    .zero 4
    // (addr&7) == 7
    .long 0xffffffff, 0xffffff
    .long 0xffffffff & (0x6bede0a5 + 0x2e8ba2e1 + 0x66424ac1 + 0xebd376a1 + 0x6bfea681 + 0x39935a61 + 0xcb711241 + 0x7a874e21)
    .zero 4

    .p2align 3
.L_kInlineVec:
    .short 33 * 33 * 33
    .short 33 * 33
    .short 33
    .short 1

    .p2align 3
.L_FinalInclineTable:
    .long 0,      0,      0,      0,      0,      0,      0,     0
    .long kStep0, kStep1, kStep2, kStep3, kStep4, kStep5, kStep6

    .p2align 3
.L_FinalStepTable:
    .long kStep4, kStep0
    .long kStep5, kStep1
    .long kStep6, kStep2
    .long kStep7, kStep3
    .long kStep8, kStep4
    .long kStep9, kStep5
    .long kStep10, kStep6
    .long kStep11, kStep7

END(calculate_gnu_hash_asm)
