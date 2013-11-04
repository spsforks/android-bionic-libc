//===-- aeabi_memcpy.S - EABI memcpy implementation -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <machine/cpu-features.h>
#include <machine/asm.h>

//  void __aeabi_memcpy(void *dest, void *src, size_t n) { memcpy(dest, src, n); }

#ifdef __clang__
        .align 2
ENTRY(__aeabi_memcpy)
        b       memcpy

WEAK_ALIAS(__aeabi_memcpy4, __aeabi_memcpy)
WEAK_ALIAS(__aeabi_memcpy8, __aeabi_memcpy)
END(__aeabi_memcpy)
#endif
