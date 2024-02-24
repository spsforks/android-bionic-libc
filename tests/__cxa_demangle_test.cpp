/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <cxxabi.h>
#include <gtest/gtest.h>
#include <string.h>

TEST(__cxa_demangle, cxa_demangle_fuzz_152588929) {
#if defined(__aarch64__)
  // Test the C++ demangler on a slightly invalid mangled string:
  //    (1 "\006") (I (L e "eeEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" E) E)
  // There are a few issues with this mangled string:
  //  - The IA64 C++ ABI specifies that an FP literal's hex chars are supposed
  //    to be lowercase, but the libc++abi demangler also accepts uppercase
  //    digits, which is confusing because 'E' also the terminating character
  //    for <template-args> and <expr-primary>.
  //  - libc++abi uses snprintf("%a") which puts an unspecified number of bits
  //    in the digit before the decimal point.
  //  - The identifier name is "\006", and the IA64 C++ ABI spec is explicit
  //    about not specifying the encoding for characters outside of
  //    [_A-Za-z0-9].
  //  - The 'e' type is documented as "long double, __float80", and in practice
  //    the length of the literal depends on the arch. For arm64, it is a
  //    128-bit FP type encoded using 32 hex chars. The situation with x86-64
  //    Android OTOH is messy because Clang uses 'g' for its 128-bit
  //    long double.
  char* p = abi::__cxa_demangle("1\006ILeeeEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", 0, 0, 0);
  if (!strcmp(p, "\x6<-0x1.cecececececececececececececep+11983")) {
    // TODO(b/175635923). After upgrading libc++, stop accepting this bad
    // demangling. See https://reviews.llvm.org/D77924.
  } else {
    ASSERT_STREQ("\x6<-0x1.cecececececececececececececep+11983L>", p);
  }
  free(p);
#endif
}

TEST(__cxa_demangle, DISABLED_cxa_demangle_fuzz_167977068) {
#if defined(__aarch64__)
  char* p = abi::__cxa_demangle("DTLeeeeeeeeeeeeeeeeeeeeeeeeeEEEEeeEEEE", 0, 0, 0);
  ASSERT_EQ(nullptr, p) << p;
  free(p);
#endif
}
