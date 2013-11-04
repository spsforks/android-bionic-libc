//===-- aeabi_memset.S - EABI memset implementation -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <machine/cpu-features.h>
#include <machine/asm.h>


//  void __aeabi_memset(void *dest, size_t n, int c) { memset(dest, c, n); }

        .align 2

#ifdef __clang__
ENTRY(__aeabi_memset)
        mov     r3, r1
        mov     r1, r2
        mov     r2, r3
        b       memset

WEAK_ALIAS(__aeabi_memset4, __aeabi_memset)
WEAK_ALIAS(__aeabi_memset8, __aeabi_memset)
END(__aeabi_memset)
#endif
