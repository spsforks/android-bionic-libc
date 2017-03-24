// The following is copied from bionic/libm/sincos.c with modification

/*-
 * Copyright (C) 2010 The Android Open Source Project
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
 */

#define _GNU_SOURCE 1
#include <math.h>

// Disable sincos optimization for all functions in this file,
// otherwise gcc would generate infinite calls.
// Refer to gcc PR46926.
// -fno-builtin-sin or -fno-builtin-cos can disable sincos optimization,
// but these two options do not work inside optimize pragma in-file.
// Thus we just enforce -O0 when compiling this file.
#pragma GCC optimize ("O0")
/*
void sincos(double x, double* p_sin, double* p_cos) {
  *p_sin = sin(x);
  *p_cos = cos(x);
}

void sincosf(float x, float* p_sinf, float* p_cosf) {
  *p_sinf = sinf(x);
  *p_cosf = cosf(x);
}
*/
void sincosl(long double x, long double* p_sinl, long double* p_cosl) {
  *p_sinl = sinl(x);
  *p_cosl = cosl(x);
}

// The following is copied from bionic/libm/upstream-freebsd/lib/msun/src/s_nearbyint.c
// with modification

/*-
 * Copyright (c) 2004 David Schultz <das@FreeBSD.ORG>
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
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <fenv.h>
#include <math.h>

/*
 * We save and restore the floating-point environment to avoid raising
 * an inexact exception.  We can get away with using fesetenv()
 * instead of feclearexcept()/feupdateenv() to restore the environment
 * because the only exception defined for rint() is overflow, and
 * rounding can't overflow as long as emax >= p.
 *
 * The volatile keyword is needed below because clang incorrectly assumes
 * that rint won't raise any floating-point exceptions. Declaring ret volatile
 * is sufficient to trick the compiler into doing the right thing.
 */
#define	DECL(type, fn, rint)	\
type				\
fn(type x)			\
{				\
	volatile type ret;	\
	fenv_t env;		\
				\
	fegetenv(&env);		\
	ret = rint(x);		\
	fesetenv(&env);		\
	return (ret);		\
}

//DECL(double, nearbyint, rint)
//DECL(float, nearbyintf, rintf)
DECL(long double, nearbyintl, rintl)
