//===-- aeabi_memmove.S - EABI memmove implementation --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//

#include <machine/cpu-features.h>
#include <machine/asm.h>

//  void __aeabi_memmove(void *dest, void *src, size_t n) { memmove(dest, src, n); }

#ifdef __clang__
        .align 2
ENTRY(__aeabi_memmove)
        b       memmove

WEAK_ALIAS(__aeabi_memmove4, __aeabi_memmove)
WEAK_ALIAS(__aeabi_memmove8, __aeabi_memmove)
END(__aeabi_memmove)
#endif
