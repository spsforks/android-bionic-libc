/*-
 * Copyright (c) 2003 Mike Barcroft <mike@FreeBSD.org>
 * Copyright (c) 2002 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

// ANDROID changed:
// - keep only little endian variants as they're the only one supported.
// - add long double structures here instead of _fpmath.h.
// - android uses 128 bits long doubles for LP64, so the structure and macros
//   were reworked for the quad precision ieee representation.

#ifndef _FPMATH_
#define _FPMATH_

#include <endian.h>

union IEEEf2bits {
  float f;
  struct {
    unsigned int  man   :23;
    unsigned int  exp   :8;
    unsigned int  sign  :1;
  } bits;
};

#define DBL_MANH_SIZE  20
#define DBL_MANL_SIZE  32

union IEEEd2bits {
  double  d;
  struct {
    unsigned int  manl  :32;
    unsigned int  manh  :20;
    unsigned int  exp   :11;
    unsigned int  sign  :1;
  } bits;
};

#ifdef __LP64__

union IEEEl2bits {
  long double e;
  struct {
    unsigned long manl  :64;
    unsigned long manh  :48;
    unsigned int  exp   :15;
    unsigned int  sign  :1;
  } bits;
  struct {
    unsigned long manl     :64;
    unsigned long manh     :48;
    unsigned int  expsign  :16;
  } xbits;
};

#define LDBL_NBIT 0
#define LDBL_IMPLICIT_NBIT
#define mask_nbit_l(u)  ((void)0)

#define LDBL_MANH_SIZE  48
#define LDBL_MANL_SIZE  64

#define LDBL_TO_ARRAY32(u, a) do {           \
  (a)[0] = (uint32_t)(u).bits.manl;          \
  (a)[1] = (uint32_t)((u).bits.manl >> 32);  \
  (a)[2] = (uint32_t)(u).bits.manh;          \
  (a)[3] = (uint32_t)((u).bits.manh >> 32);  \
} while(0)

#else // __LP32__

#ifdef __i386__

union IEEEl2bits {
  long double e;
  struct {
    unsigned int  manl  :32;
    unsigned int  manh  :32;
    unsigned int  exp   :15;
    unsigned int  sign  :1;
    unsigned int  junk  :16;
  } bits;
  struct {
    unsigned long long man  :64;
    unsigned int  expsign   :16;
    unsigned int  junk      :16;
  } xbits;
};

#define LDBL_NBIT 0x80000000
#define mask_nbit_l(u)  ((u).bits.manh &= ~LDBL_NBIT)

#define LDBL_MANH_SIZE  32
#define LDBL_MANL_SIZE  32

#else // arm, mips

union IEEEl2bits {
  long double e;
  struct {
    unsigned int  manl  :32;
    unsigned int  manh  :20;
    unsigned int  exp   :11;
    unsigned int  sign  :1;
  } bits;
};

#define LDBL_NBIT 0
#define LDBL_IMPLICIT_NBIT
#define mask_nbit_l(u)  ((void)0)

#define LDBL_MANH_SIZE  20
#define LDBL_MANL_SIZE  32

#endif // __i386__

#define LDBL_TO_ARRAY32(u, a) do {    \
  (a)[0] = (uint32_t)(u).bits.manl;   \
  (a)[1] = (uint32_t)(u).bits.manh;   \
} while(0)

#endif // __LP64__

/*
 * The BSD "long double" functions are broken when sizeof(long double) == sizeof(double).
 * Android works around those cases by replacing the broken functions with our own trivial stubs
 * that call the regular "double" function.
 */
#ifndef __LP64__

#define __fpclassifyl __broken__fpclassify
#define __isfinitel __broken__isfinitel
#define __isinfl __broken__isinfl
#define __isnanl __broken__isnanl
#define __isnormall __broken__isnormall
#define __signbitl __broken_signbitl

#endif // __LP64__

#endif // _FPMATH_
