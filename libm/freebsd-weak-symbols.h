/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef _FREEBSD_WEAK_SYMBOLS_H_
#define _FREEBSD_WEAK_SYMBOLS_H_

/* This file contains all of the freebsd symbols that should be marked weak. */

/* Math functions. */
#pragma weak acos
#pragma weak asin
#pragma weak atan
#pragma weak atan2
#pragma weak cos
#pragma weak sin
#pragma weak tan

#pragma weak cosh
#pragma weak sinh
#pragma weak tanh

#pragma weak exp
#pragma weak frexp
#pragma weak ldexp
#pragma weak log
#pragma weak log10
#pragma weak modf

#pragma weak pow
#pragma weak sqrt

#pragma weak ceil
#pragma weak fabs
#pragma weak floor
#pragma weak fmod

#pragma weak acosh
#pragma weak asinh
#pragma weak atanh
#pragma weak cbrt
#pragma weak erf
#pragma weak erfc
#pragma weak exp2
#pragma weak expm1
#pragma weak fma
#pragma weak hypot
#pragma weak ilogb
#pragma weak isinf
#pragma weak isnan
#pragma weak lgamma
#pragma weak llrint
#pragma weak llround
#pragma weak log1p
#pragma weak log2
#pragma weak logb
#pragma weak lrint
#pragma weak lround
#pragma weak nan
#pragma weak nextafter
#pragma weak remainder
#pragma weak remquo
#pragma weak rint

#pragma weak j0
#pragma weak j1
#pragma weak jn
#pragma weak y0
#pragma weak y1
#pragma weak yn

#pragma weak gamma

#pragma weak scalb

#pragma weak copysign
#pragma weak fdim
#pragma weak fmax
#pragma weak fmin
#pragma weak nearbyint
#pragma weak round
#pragma weak scalbln
#pragma weak scalbn
#pragma weak tgamma
#pragma weak trunc

#pragma weak drem
#pragma weak finite
#pragma weak isnanf
#pragma weak significandl

#pragma weak gamma_r
#pragma weak lgamma_r

#pragma weak significand

#pragma weak acosf
#pragma weak asinf
#pragma weak atanf
#pragma weak atan2f
#pragma weak cosf
#pragma weak sinf
#pragma weak tanf

#pragma weak coshf
#pragma weak sinhf
#pragma weak tanhf

#pragma weak exp2f
#pragma weak expf
#pragma weak expm1f
#pragma weak frexpf
#pragma weak ilogbf
#pragma weak ldexpf
#pragma weak log10f
#pragma weak log1pf
#pragma weak log2f
#pragma weak logf
#pragma weak modff

#pragma weak powf
#pragma weak sqrtf

#pragma weak ceilf
#pragma weak fabsf
#pragma weak floorf
#pragma weak fmodf
#pragma weak roundf

#pragma weak erff
#pragma weak erfcf
#pragma weak hypotf
#pragma weak lgammaf
#pragma weak tgammaf

#pragma weak acoshf
#pragma weak asinhf
#pragma weak atanhf
#pragma weak cbrtf
#pragma weak logbf
#pragma weak copysignf
#pragma weak llrintf
#pragma weak llroundf
#pragma weak lrintf
#pragma weak lroundf
#pragma weak nanf
#pragma weak nearbyintf
#pragma weak nextafterf
#pragma weak remainderf
#pragma weak remquof
#pragma weak rintf
#pragma weak scalblnf
#pragma weak scalbnf
#pragma weak truncf

#pragma weak fdimf
#pragma weak fmaf
#pragma weak fmaxf
#pragma weak fminf

#pragma weak dremf
#pragma weak finitef
#pragma weak gammaf
#pragma weak j0f
#pragma weak j1f
#pragma weak jnf
#pragma weak scalbf
#pragma weak y0f
#pragma weak y1f
#pragma weak ynf

#pragma weak gammaf_r
#pragma weak lgammaf_r

#pragma weak significandf

#pragma weak acoshl
#pragma weak acosl
#pragma weak asinhl
#pragma weak asinl
#pragma weak atan2l
#pragma weak atanhl
#pragma weak atanl
#pragma weak cbrtl
#pragma weak ceill
#pragma weak copysignl
#pragma weak coshl
#pragma weak cosl
#pragma weak erfcl
#pragma weak erfl
#pragma weak exp2l
#pragma weak expl
#pragma weak expm1l
#pragma weak fabsl
#pragma weak fdiml
#pragma weak floorl
#pragma weak fmal
#pragma weak fmaxl
#pragma weak fminl
#pragma weak fmodl
#pragma weak frexpl
#pragma weak hypotl
#pragma weak ilogbl
#pragma weak ldexpl
#pragma weak lgammal
#pragma weak llrintl
#pragma weak llroundl
#pragma weak log10l
#pragma weak log1pl
#pragma weak log2l
#pragma weak logbl
#pragma weak logl
#pragma weak lrintl
#pragma weak lroundl
#pragma weak modfl
#pragma weak nanl
#pragma weak nearbyintl
#pragma weak nextafterl
#pragma weak nexttoward
#pragma weak nexttowardf
#pragma weak nexttowardl
#pragma weak powl
#pragma weak remainderl
#pragma weak remquol
#pragma weak rintl
#pragma weak roundl
#pragma weak scalblnl
#pragma weak scalbnl
#pragma weak sinhl
#pragma weak sinl
#pragma weak sqrtl
#pragma weak tanhl
#pragma weak tanl
#pragma weak tgammal
#pragma weak truncl

#pragma weak lgammal_r

#pragma weak sincos
#pragma weak sincosf
#pragma weak sincosl

/* Complex functions. */
#pragma weak cacos
#pragma weak cacosf
#pragma weak cacosh
#pragma weak cacoshf

#pragma weak casin
#pragma weak casinf
#pragma weak casinh
#pragma weak casinhf

#pragma weak catan
#pragma weak catanf
#pragma weak catanh
#pragma weak catanhf

#pragma weak ccos
#pragma weak ccosf
#pragma weak ccosh
#pragma weak ccoshf

#pragma weak csin
#pragma weak csinf
#pragma weak csinh
#pragma weak csinhf

#pragma weak ctan
#pragma weak ctanf
#pragma weak ctanh
#pragma weak ctanhf

#pragma weak carg
#pragma weak cargf
#pragma weak cargl

#pragma weak cexp
#pragma weak cexpf

#pragma weak cimag
#pragma weak cimagf
#pragma weak cimagl

#pragma weak conj
#pragma weak conjf
#pragma weak conjl

#pragma weak cproj
#pragma weak cprojf
#pragma weak cprojl

#pragma weak creal
#pragma weak crealf
#pragma weak creall

#pragma weak csqrt
#pragma weak csqrtf
#pragma weak csqrtl

#pragma weak cabs
#pragma weak cabsf
#pragma weak cabsl

#pragma weak imprecise_powl
#pragma weak imprecise_tgammal

#endif /* _FREEBSD_WEAK_SYMBOLS_H_ */
