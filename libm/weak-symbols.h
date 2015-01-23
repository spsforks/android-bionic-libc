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

#pragma weak acos
#pragma weak acosf
#pragma weak acosh
#pragma weak acoshf
#pragma weak acoshl
#pragma weak acosl
#pragma weak asin
#pragma weak asinf
#pragma weak asinh
#pragma weak asinhf
#pragma weak asinhl
#pragma weak asinl
#pragma weak atan
#pragma weak atan2
#pragma weak atan2f
#pragma weak atan2l
#pragma weak atanf
#pragma weak atanh
#pragma weak atanhf
#pragma weak atanhl
#pragma weak atanl
#pragma weak cabs
#pragma weak cabsf
#pragma weak cabsl
#pragma weak cacos
#pragma weak cacosf
#pragma weak cacosh
#pragma weak cacoshf
#pragma weak carg
#pragma weak cargf
#pragma weak cargl
#pragma weak casin
#pragma weak casinf
#pragma weak casinh
#pragma weak casinhf
#pragma weak catan
#pragma weak catanf
#pragma weak catanh
#pragma weak catanhf
#pragma weak cbrt
#pragma weak cbrtf
#pragma weak cbrtl
#pragma weak ccos
#pragma weak ccosf
#pragma weak ccosh
#pragma weak ccoshf
#pragma weak ceil
#pragma weak ceilf
#pragma weak ceill
#pragma weak cexp
#pragma weak cexpf
#pragma weak cimag
#pragma weak cimagf
#pragma weak cimagl
#pragma weak conj
#pragma weak conjf
#pragma weak conjl
#pragma weak copysign
#pragma weak copysignf
#pragma weak copysignl
#pragma weak cos
#pragma weak cosf
#pragma weak cosh
#pragma weak coshf
#pragma weak coshl
#pragma weak cosl
#pragma weak cproj
#pragma weak cprojf
#pragma weak cprojl
#pragma weak creal
#pragma weak crealf
#pragma weak creall
#pragma weak csin
#pragma weak csinf
#pragma weak csinh
#pragma weak csinhf
#pragma weak csqrt
#pragma weak csqrtf
#pragma weak csqrtl
#pragma weak ctan
#pragma weak ctanf
#pragma weak ctanh
#pragma weak ctanhf
#pragma weak drem
#pragma weak dremf
#pragma weak erf
#pragma weak erfc
#pragma weak erfcf
#pragma weak erfcl
#pragma weak erff
#pragma weak erfl
#pragma weak exp
#pragma weak exp2
#pragma weak exp2f
#pragma weak exp2l
#pragma weak expf
#pragma weak expl
#pragma weak expm1
#pragma weak expm1f
#pragma weak expm1l
#pragma weak fabs
#pragma weak fabsf
#pragma weak fabsl
#pragma weak fdim
#pragma weak fdimf
#pragma weak fdiml
#pragma weak finite
#pragma weak finitef
#pragma weak floor
#pragma weak floorf
#pragma weak floorl
#pragma weak fma
#pragma weak fmaf
#pragma weak fmal
#pragma weak fmax
#pragma weak fmaxf
#pragma weak fmaxl
#pragma weak fmin
#pragma weak fminf
#pragma weak fminl
#pragma weak fmod
#pragma weak fmodf
#pragma weak fmodl
#pragma weak frexp
#pragma weak frexpf
#pragma weak frexpl
#pragma weak gamma
#pragma weak gammaf
#pragma weak gammaf_r
#pragma weak gamma_r
#pragma weak hypot
#pragma weak hypotf
#pragma weak hypotl
#pragma weak ilogb
#pragma weak ilogbf
#pragma weak ilogbl
#pragma weak imprecise_powl
#pragma weak imprecise_tgammal
#pragma weak isinf
#pragma weak isnan
#pragma weak isnanf
#pragma weak j0
#pragma weak j0f
#pragma weak j1
#pragma weak j1f
#pragma weak jn
#pragma weak jnf
#pragma weak ldexp
#pragma weak ldexpf
#pragma weak ldexpl
#pragma weak lgamma
#pragma weak lgammaf
#pragma weak lgammaf_r
#pragma weak lgammal
#pragma weak lgammal_r
#pragma weak lgamma_r
#pragma weak llrint
#pragma weak llrintf
#pragma weak llrintl
#pragma weak llround
#pragma weak llroundf
#pragma weak llroundl
#pragma weak log
#pragma weak log10
#pragma weak log10f
#pragma weak log10l
#pragma weak log1p
#pragma weak log1pf
#pragma weak log1pl
#pragma weak log2
#pragma weak log2f
#pragma weak log2l
#pragma weak logb
#pragma weak logbf
#pragma weak logbl
#pragma weak logf
#pragma weak logl
#pragma weak lrint
#pragma weak lrintf
#pragma weak lrintl
#pragma weak lround
#pragma weak lroundf
#pragma weak lroundl
#pragma weak modf
#pragma weak modff
#pragma weak modfl
#pragma weak nan
#pragma weak nanf
#pragma weak nanl
#pragma weak nearbyint
#pragma weak nearbyintf
#pragma weak nearbyintl
#pragma weak nextafter
#pragma weak nextafterf
#pragma weak nextafterl
#pragma weak nexttoward
#pragma weak nexttowardf
#pragma weak nexttowardl
#pragma weak pow
#pragma weak powf
#pragma weak powl
#pragma weak remainder
#pragma weak remainderf
#pragma weak remainderl
#pragma weak remquo
#pragma weak remquof
#pragma weak remquol
#pragma weak rint
#pragma weak rintf
#pragma weak rintl
#pragma weak round
#pragma weak roundf
#pragma weak roundl
#pragma weak scalb
#pragma weak scalbf
#pragma weak scalbln
#pragma weak scalblnf
#pragma weak scalblnl
#pragma weak scalbn
#pragma weak scalbnf
#pragma weak scalbnl
#pragma weak significand
#pragma weak significandf
#pragma weak significandl
#pragma weak sin
#pragma weak sincos
#pragma weak sincosf
#pragma weak sincosl
#pragma weak sinf
#pragma weak sinh
#pragma weak sinhf
#pragma weak sinhl
#pragma weak sinl
#pragma weak sqrt
#pragma weak sqrtf
#pragma weak sqrtl
#pragma weak tan
#pragma weak tanf
#pragma weak tanh
#pragma weak tanhf
#pragma weak tanhl
#pragma weak tanl
#pragma weak tgamma
#pragma weak tgammaf
#pragma weak tgammal
#pragma weak trunc
#pragma weak truncf
#pragma weak truncl
#pragma weak y0
#pragma weak y0f
#pragma weak y1
#pragma weak y1f
#pragma weak yn
#pragma weak ynf

#endif /* _FREEBSD_WEAK_SYMBOLS_H_ */
