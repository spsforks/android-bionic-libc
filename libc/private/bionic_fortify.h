/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef _BIONIC_FORTIFY_H
#define _BIONIC_FORTIFY_H

#include <sys/cdefs.h>
#include <stdint.h>

#define BIONIC_EVENT_MEMCPY_BUFFER_OVERFLOW 80100
#define BIONIC_EVENT_STRCAT_BUFFER_OVERFLOW 80105
#define BIONIC_EVENT_MEMMOVE_BUFFER_OVERFLOW 80110
#define BIONIC_EVENT_STRNCAT_BUFFER_OVERFLOW 80115
#define BIONIC_EVENT_STRNCPY_BUFFER_OVERFLOW 80120
#define BIONIC_EVENT_MEMSET_BUFFER_OVERFLOW 80125
#define BIONIC_EVENT_STRCPY_BUFFER_OVERFLOW 80130

#define BIONIC_EVENT_STRCAT_INTEGER_OVERFLOW 80200
#define BIONIC_EVENT_STRNCAT_INTEGER_OVERFLOW 80205


__BEGIN_DECLS

__noreturn extern void __bionic_fortify_die(const char *, uint32_t);

__END_DECLS


#endif
