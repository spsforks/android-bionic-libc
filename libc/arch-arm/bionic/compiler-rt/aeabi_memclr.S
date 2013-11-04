//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <machine/cpu-features.h>
#include <machine/asm.h>


//  void __aeabi_memclr(void *dest, size_t n) { __aeabi_memset(dest, n, 0); }

        .align 2

#ifdef __clang__
ENTRY(__aeabi_memclr)

        mov     r2, r1
        mov     r1, #0
        b       memset

WEAK_ALIAS(__aeabi_memclr4, __aeabi_memclr)
WEAK_ALIAS(__aeabi_memclr8, __aeabi_memclr)
END(__aeabi_memclr)
#endif
