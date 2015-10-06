/*
 * Copyright (C) 2013 The Android Open Source Project
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

#ifndef _PRIVATE_BIONIC_ASM_H_
#define _PRIVATE_BIONIC_ASM_H_

#include <asm/unistd.h> /* For system call numbers. */
#define MAX_ERRNO 4095  /* For recognizing system call error returns. */

#define __bionic_asm_custom_entry(f)
#define __bionic_asm_custom_end(f)
#define __bionic_asm_function_type @function

#include <machine/asm.h>

#undef INTERNAL
#define INTERNAL(FNAME) __bionic_##FNAME

#define WEAK_ALIAS(alias, original) \
  .weak alias; \
  .equ alias, original

/* This is the new "basic!!!" asm entry macro */
#define ENTRY_BASIC(f) \
    .text; \
    .globl f; \
    .align __bionic_asm_align; \
    .type f, __bionic_asm_function_type; \
    f: \
    __bionic_asm_custom_entry(f) \

#define ENTRY_NO_DWARF(f) \
  ENTRY_BASIC(f)

#define ENTRY(f) \
    ENTRY_BASIC(INTERNAL(f)); \
    .cfi_startproc \

#define END_BASIC(f) \
    .size f, .-f; \
    __bionic_asm_custom_end(f) \


#define END_NO_DWARF(f) \
    END_BASIC(f); \

#define END(f) \
    .cfi_endproc; \
    END_BASIC(INTERNAL(f)); \
    WEAK_ALIAS(f, INTERNAL(f))

/* Like ENTRY, but with hidden visibility. */
/* Unlike ENTRY, we do not need to declare __bionic_foo() */
#define ENTRY_PRIVATE(f) \
    ENTRY_BASIC(f); \
    .cfi_startproc; \
    .hidden f

#define END_PRIVATE(f) \
    .cfi_endproc; \
    END_BASIC(f)


/* Like ENTRY_NO_DWARF, but with hidden visibility. */
/* Unlike ENTRY, we do not need to declare __bionic_foo() */
#define ENTRY_PRIVATE_NO_DWARF(f) \
    ENTRY_BASIC(f); \
    .hidden f \

#define END_PRIVATE_NO_DWARF(f) \
    END_BASIC(F)

#define ENTRY_NOINTERNAL(f) \
    ENTRY_BASIC(f); \
    .cfi_startproc

#define END_NOINTERNAL(f) \
    END_PRIVATE(f)

/* This is somewhat more convoluted, but the C API
   defines __bionic_foo() and generates a weak alias foo that points back to
   __bionic_foo(). We mirror that here*/

#define ALIAS_SYMBOL(alias, original) \
    .globl INTERNAL(alias); \
    .equ INTERNAL(alias), original; \
    WEAK_ALIAS(alias, INTERNAL(alias))

#endif /* _PRIVATE_BIONIC_ASM_H_ */
