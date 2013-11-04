//===-- aeabi_memcmp.S - EABI memcmp implementation -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <machine/cpu-features.h>
#include <machine/asm.h>

//  void __aeabi_memcmp(void *dest, void *src, size_t n) { memcmp(dest, src, n); }

#ifdef __clang__
        .align 2
ENTRY(__aeabi_memcmp)
        b       memcmp

WEAK_ALIAS(__aeabi_memcmp4, __aeabi_memcmp)
WEAK_ALIAS(__aeabi_memcmp8, __aeabi_memcmp)

END(__aeabi_memcmp)
#endif
