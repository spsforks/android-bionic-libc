/* Copyright (C) 2013 The Android Open Source Project
 * Copyright (c) 2013, NVIDIA CORPORATION.  All rights reserved.
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

#include <bionic_asm.h>

ENTRY(floor)    /* x in r0, r1 */

        and             r3, r1, #0x80000000     /* sign(x) */
        bic             r1, r1, #0x80000000     /* x = abs(x) */

        /* extract exp of x */
        lsr             r2, r1, #20
        sub             r2, r2, #0x3fc
        subs            r2, r2, #0x3            /* r2 <- exp */

        /* |x| < 1.0? */
        blt             x_lt_one

        /* x < 0? */
        cmp             r3, #0
        bne             clr_frac_neg

        /* |x| <= 2^20? */
        cmp             r2, #20
        ble             clr_frac_r1

        /* |x| < 2^52? */
        cmp             r2, #52
        blt             clr_frac_r0

        /* return x */
        bx              lr

clr_frac_r1:
        rsb             r2, r2, #20
        lsr             r1, r1, r2
        lsl             r1, r1, r2
        mov             r0, #0
        bx              lr

clr_frac_r0:
        rsb             r2, r2, #52
        lsr             r0, r0, r2
        lsl             r0, r0, r2
        bx              lr

clr_frac_neg:
        /* |x| <= 2^20? */
        cmp             r2, #20
        ble             clr_frac_r1_neg

        /* |x| < 2^52? */
        cmp             r2, #52
        blt             clr_frac_r0_neg

        /* return x */
        orr             r1, r1, #0x80000000
        bx              lr

clr_frac_r1_neg:
        rsb             r2, r2, #20
        mov             r3, #1
        lsl             r3, r3, r2
        sub             r3, r3, #1
        and             r3, r1, r3
        orr             r3, r3, r0
        lsr             r1, r1, r2
        lsl             r1, r1, r2
        mov             r0, #0
        b               return_x_neg

clr_frac_r0_neg:
        rsb             r2, r2, #52
        mov             r3, #1
        lsl             r3, r3, r2
        sub             r3, r3, #1
        and             r3, r0, r3
        lsr             r0, r0, r2
        lsl             r0, r0, r2
        b               return_x_neg

x_lt_one:
        /* x == +-0? */
        cmp             r0, #0
        cmpeq           r1, #0
        orreq           r1, r1, r3
        bxeq            lr

        /* (x > 0) ? 0 : -1 */
        mov             r1, #0x00100000
        mov             r0, #0
        cmp             r3, #0
        movne           r1, #0xc0000000
        sub             r1, r1, #0x00100000
        bx              lr

return_x_neg:
        cmp             r3, #0
        orr             r1, r1, #0x80000000
        bxeq            lr

        vmov            d16, r0, r1
        vmov.f64        d17, #1.0
        vsub.f64        d16, d16, d17
        vmov            r0, r1, d16
        bx              lr

END(floor)

        .weak           floorl
        .equ            floorl,floor
