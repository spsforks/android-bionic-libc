/*
 * Copyright (C) 2018 The Android Open Source Project
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

extern char __aeabi_cdcmpeq;
extern char __aeabi_cdcmple;
extern char __aeabi_cdrcmple;
extern char __aeabi_cfcmpeq;
extern char __aeabi_cfcmple;
extern char __aeabi_cfrcmple;
extern char __aeabi_d2uiz;
extern char __aeabi_dadd;
extern char __aeabi_dcmpeq;
extern char __aeabi_dcmpge;
extern char __aeabi_dcmpgt;
extern char __aeabi_dcmple;
extern char __aeabi_dcmplt;
extern char __aeabi_ddiv;
extern char __aeabi_dmul;
extern char __aeabi_drsub;
extern char __aeabi_dsub;
extern char __aeabi_f2d;
extern char __aeabi_fcmpe;
extern char __aeabi_i2d;
extern char __aeabi_ui2d;
extern char __aeabi_ul2d;
extern char __floatsidf;
extern char __floatundidf;
extern char __floatunsidf;

void* __bionic_libcrt_compat_symbols[] = {
    &__aeabi_cdcmpeq,
    &__aeabi_cdcmple,
    &__aeabi_cdrcmple,
    &__aeabi_cfcmpeq,
    &__aeabi_cfcmple,
    &__aeabi_cfrcmple,
    &__aeabi_d2uiz,
    &__aeabi_dadd,
    &__aeabi_dcmpeq,
    &__aeabi_dcmpge,
    &__aeabi_dcmpgt,
    &__aeabi_dcmple,
    &__aeabi_dcmplt,
    &__aeabi_ddiv,
    &__aeabi_dmul,
    &__aeabi_drsub,
    &__aeabi_dsub,
    &__aeabi_f2d,
    &__aeabi_fcmpe,
    &__aeabi_i2d,
    &__aeabi_ui2d,
    &__aeabi_ul2d,
    &__floatsidf,
    &__floatundidf,
    &__floatunsidf,
};
