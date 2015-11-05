/*
 * Copyright (C) 2013 The Android Open Source Project
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

#ifndef _BIONIC_TIME_CONVERSIONS_H
#define _BIONIC_TIME_CONVERSIONS_H

#include <time.h>
#include <sys/cdefs.h>

#include "private/bionic_constants.h"

__BEGIN_DECLS

__LIBC_HIDDEN__ bool timespec_from_timeval(timespec& ts, const timeval& tv);
__LIBC_HIDDEN__ void timespec_from_ms(timespec& ts, const int ms);

__LIBC_HIDDEN__ void timeval_from_timespec(timeval& tv, const timespec& ts);

__LIBC_HIDDEN__ bool timespec_from_absolute_timespec(timespec& ts, const timespec& abs_ts, clockid_t clock);

__LIBC_HIDDEN__ void timespec_to_absolute_timespec(const timespec& ts, timespec& abs_ts, clockid_t clock);
__END_DECLS

#endif
