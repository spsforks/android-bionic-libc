/*
 * Copyright (C) 2018 The Android Open Source Project
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
#include <private/bionic_asm_tls.h>

// XXX: CFI annotations?
// XXX: Optimize the assembly?
// XXX: https://godbolt.org/g/GsSuxF

.globl __ld_tls_get_addr

ENTRY(tlsdesc_resolver_static)
  ldr x0, [x0, #8]
  ret
END(tlsdesc_resolver_static)

  // This function must preserve every register except X0.
  // XXX: It assumes the stack pointer is aligned.
  // XXX: What about condition flags?
ENTRY(tlsdesc_resolver_dynamic)
  stp x19, x20, [sp, #-32]!
  stp x21, x22, [sp, #16]

  mrs x19, tpidr_el0            // __get_tls()
  ldr x20, [x19, #(TLS_SLOT_DTV_MACRO * 8)] // __get_tls()[TLS_SLOT_DTV]
  ldr x21, [x20]                // TlsDtv::generation

  ldr x0, [x0, #8]              // TlsDescResolverDynamicArg*
  ldr x22, [x0]                 // TlsDescResolverDynamicArg::generation

  cmp x21, x22
  b.lo .fallback

  ldr x21, [x0, #8]             // TlsIndex::module
  ldr x22, [x0, #16]            // TlsIndex::offset
  ldr x21, [x20, x21, lsl #3]   // TlsDtv::modules[module]
  cbz x21, .fallback
  add x0, x21, x22
  sub x0, x0, x19

  ldp x21, x22, [sp, #16]
  ldp x19, x20, [sp], #32
  ret

.fallback:
  // Restore the small stack frame. x0 still refers to a TlsDescResolverDynamicArg* rather than
  // its initial TlsDescriptor* value.
  ldp x21, x22, [sp, #16]
  ldp x19, x20, [sp], #32

  // XXX: This will use about a kilobyte of stack, maybe? Probably OK.
  sub sp, sp, #(16 * 47)

  // XXX: for i in range(1, 30, 2): print('stp x{}, x{}, [sp, #(16 * {})]'.format(i, i+1, i//2))
  stp x1, x2, [sp, #(16 * 0)]
  stp x3, x4, [sp, #(16 * 1)]
  stp x5, x6, [sp, #(16 * 2)]
  stp x7, x8, [sp, #(16 * 3)]
  stp x9, x10, [sp, #(16 * 4)]
  stp x11, x12, [sp, #(16 * 5)]
  stp x13, x14, [sp, #(16 * 6)]
  stp x15, x16, [sp, #(16 * 7)]
  stp x17, x18, [sp, #(16 * 8)]
  stp x19, x20, [sp, #(16 * 9)]
  stp x21, x22, [sp, #(16 * 10)]
  stp x23, x24, [sp, #(16 * 11)]
  stp x25, x26, [sp, #(16 * 12)]
  stp x27, x28, [sp, #(16 * 13)]
  stp x29, x30, [sp, #(16 * 14)]

  // XXX: I was confused by the qNN vs vNN register names. I am not sure this correctly saves vector regs.
  // for i in range(0, 32, 2): print('stp q{}, q{}, [sp, #(16 * {})]'.format(i, i+1, 15 + i))
  stp q0, q1, [sp, #(16 * 15)]
  stp q2, q3, [sp, #(16 * 17)]
  stp q4, q5, [sp, #(16 * 19)]
  stp q6, q7, [sp, #(16 * 21)]
  stp q8, q9, [sp, #(16 * 23)]
  stp q10, q11, [sp, #(16 * 25)]
  stp q12, q13, [sp, #(16 * 27)]
  stp q14, q15, [sp, #(16 * 29)]
  stp q16, q17, [sp, #(16 * 31)]
  stp q18, q19, [sp, #(16 * 33)]
  stp q20, q21, [sp, #(16 * 35)]
  stp q22, q23, [sp, #(16 * 37)]
  stp q24, q25, [sp, #(16 * 39)]
  stp q26, q27, [sp, #(16 * 41)]
  stp q28, q29, [sp, #(16 * 43)]
  stp q30, q31, [sp, #(16 * 45)]

  add x0, x0, #8
  adrp x1, __ld_tls_get_addr
  ldr x1, [x1, :lo12:__ld_tls_get_addr]
  blr x1
  mrs x1, tpidr_el0 // __get_tls()
  sub x0, x0, x1

  ldp q30, q31, [sp, #(16 * 45)]
  ldp q28, q29, [sp, #(16 * 43)]
  ldp q26, q27, [sp, #(16 * 41)]
  ldp q24, q25, [sp, #(16 * 39)]
  ldp q22, q23, [sp, #(16 * 37)]
  ldp q20, q21, [sp, #(16 * 35)]
  ldp q18, q19, [sp, #(16 * 33)]
  ldp q16, q17, [sp, #(16 * 31)]
  ldp q14, q15, [sp, #(16 * 29)]
  ldp q12, q13, [sp, #(16 * 27)]
  ldp q10, q11, [sp, #(16 * 25)]
  ldp q8, q9, [sp, #(16 * 23)]
  ldp q6, q7, [sp, #(16 * 21)]
  ldp q4, q5, [sp, #(16 * 19)]
  ldp q2, q3, [sp, #(16 * 17)]
  ldp q0, q1, [sp, #(16 * 15)]

  ldp x29, x30, [sp, #(16 * 14)]
  ldp x27, x28, [sp, #(16 * 13)]
  ldp x25, x26, [sp, #(16 * 12)]
  ldp x23, x24, [sp, #(16 * 11)]
  ldp x21, x22, [sp, #(16 * 10)]
  ldp x19, x20, [sp, #(16 * 9)]
  ldp x17, x18, [sp, #(16 * 8)]
  ldp x15, x16, [sp, #(16 * 7)]
  ldp x13, x14, [sp, #(16 * 6)]
  ldp x11, x12, [sp, #(16 * 5)]
  ldp x9, x10, [sp, #(16 * 4)]
  ldp x7, x8, [sp, #(16 * 3)]
  ldp x5, x6, [sp, #(16 * 2)]
  ldp x3, x4, [sp, #(16 * 1)]
  ldp x1, x2, [sp, #(16 * 0)]

  add sp, sp, #(16 * 47)
  ret
END(tlsdesc_resolver_dynamic)
