/*
 * Copyright (C) 2024 The Android Open Source Project
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

#include <platform/bionic/tls_defines.h>
#include <private/bionic_asm.h>

.globl __tls_get_addr

// spill a register onto the stack
.macro spill reg, idx, max, f=
  \f\()sd \reg, \idx*8(sp)
  .cfi_offset \reg, (\idx - \max)*8
.endm

// reload a value from the stack
// keep max for consistency w/ spill
.macro reload reg, idx, max, f=
  \f\()ld \reg, \idx*8(sp)
  .cfi_same_value \reg
.endm

.macro spill_vector_regs
  csrr a1, vlenb
  slli a1, a1, 3
  sub sp, sp, a1
  vl8r.v v0, (sp)
  sub sp, sp, a1
  vl8r.v v8, (sp)
  sub sp, sp, a1
  vl8r.v v16, (sp)
  sub sp, sp, a1
  vl8r.v v24, (sp)
.endm

.macro reload_vector_regs
  csrr a1, vlenb
  slli a1, a1, 3
  vl8r.v v24, (sp)
  add sp, sp, a1
  vl8r.v v16, (sp)
  add sp, sp, a1
  vl8r.v v8, (sp)
  add sp, sp, a1
  vl8r.v v0, (sp)
  add sp, sp, a1
.endm

// We save a total of 33 registers
.macro on_saved_regs op max
  \op ra, 0, \max
  \op a1, 1, \max
  \op a2, 2, \max
  \op a3, 3, \max
  \op a4, 4, \max
  \op a5, 5, \max
  \op a6, 6, \max
  \op a7, 7, \max
  \op t2, 8, \max
  \op t3, 9, \max
  \op t4, 10, \max
  \op t5, 11, \max
  \op t6, 12, \max
  // save floating point regs
  \op ft0, 13, \max, f
  \op ft1, 14, \max, f
  \op ft2, 15, \max, f
  \op ft3, 16, \max, f
  \op ft4, 17, \max, f
  \op ft5, 18, \max, f
  \op ft6, 19, \max, f
  \op ft7, 20, \max, f
  \op ft8, 21, \max, f
  \op ft9, 22, \max, f
  \op ft10, 23, \max, f
  \op ft11, 24, \max, f
  \op fa0, 25, \max, f
  \op fa1, 26, \max, f
  \op fa2, 27, \max, f
  \op fa3, 28, \max, f
  \op fa4, 29, \max, f
  \op fa5, 30, \max, f
  \op fa6, 31, \max, f
  \op fa7, 32, \max, f
.endm

// These resolver functions must preserve every register except a0. They set a0
// to the offset of the TLS symbol relative to the thread pointer.

ENTRY_PRIVATE(tlsdesc_resolver_static)
  ld a0, 8(a0)
  jr t0
END(tlsdesc_resolver_static)

ENTRY_PRIVATE(tlsdesc_resolver_dynamic)
  // We only need 3 stack slots, but still require a 4th slot for alignment
  addi sp, sp, -4*8
  .cfi_def_cfa_offset 4*8
  spill a1, 1, 4
  spill a2, 2, 4
  spill a3, 3, 4

  ld a2, -8(tp)   // a2= &DTV
  ld a1, (a2)                 // a1 = TlsDtv::generation (DTV[0])

  ld a0, 8(a0)                // a0 = TlsDynamicResolverArg*
  ld a3, (a0)                 // a3 = TlsDynamicResolverArg::generation

  blt a1, a3, .fallback

  ld a1, 8(a0)                // a1 = TlsIndex::module_id
  slli a1, a1, 3              // a1 = module_id*8 -- scale the idx
  add a1, a2, a1              // a1 = &TlsDtv::modules[module_id]
  ld a1, (a1)                 // a1 = TlsDtv::modules[module_id]
  beqz a1, .fallback
  ld a0, 16(a0)               // a0 = TlsIndex::offset
  add a0, a1, a0              // a0 = TlsDtv::modules[module_id] + offset
  sub a0, a0, tp              // a0 = TlsDtv::modules[module_id] + offset - tp

  .cfi_remember_state
  reload a3, 3
  reload a2, 2
  reload a1, 1
  addi sp, sp, 4*8
  .cfi_adjust_cfa_offset -4*8
  jr t0

.fallback:
  reload a3, 3
  reload a2, 2
  reload a1, 1
  addi sp, sp, 4*8
  .cfi_adjust_cfa_offset -4*8
  j tlsdesc_resolver_dynamic_slow_path
END(tlsdesc_resolver_dynamic)

// On entry, a0 is the address of a TlsDynamicResolverArg object rather than
// the TlsDescriptor address passed to the original resolver function.
ENTRY_PRIVATE(tlsdesc_resolver_dynamic_slow_path)
  // We save a total of 33 registers, but vector spills require an alignment of 16
  addi sp, sp, (8*34)
  .cfi_def_cfa_offset (8 * 34)
  on_saved_regs spill, 34
  spill_vector_regs

  add a0, a0, 8
  call __tls_get_addr
  sub a0, a0, tp

  reload_vector_regs
  on_saved_regs reload, 34
  addi sp, sp, 8*34
  .cfi_def_cfa_offset 0
  jr t0
END(tlsdesc_resolver_dynamic_slow_path)

// The address of an unresolved weak TLS symbol evaluates to NULL with TLSDESC.
// The value returned by this function is added to the thread pointer, so return
// a negated thread pointer to cancel it out.
ENTRY_PRIVATE(tlsdesc_resolver_unresolved_weak)
  sub a0, zero, tp
  jr t0
END(tlsdesc_resolver_unresolved_weak)
