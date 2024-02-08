/*
 * Copyright (C) 2014 The Android Open Source Project
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

#pragma once

/**
 * @file android/set_abort_message.h
 * @brief Attach extra information to android crashes.
 */

#include <stddef.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

typedef struct crash_detail_t crash_detail_t;


/**
 * android_set_abort_message() sets the abort message that will be shown
 * by [debuggerd](https://source.android.com/devices/tech/debug/native-crash).
 * This is meant for use by libraries that deliberately abort so that they can
 * provide an explanation. It is used within bionic to implement assert() and
 * all FORTIFY/fdsan aborts.
 */
void android_set_abort_message(const char* _Nullable __msg);

/**
 * Add a new string that gets logged into a tombstone.
 * The lifetime of the string has to be valid until the program crashes, or until
 * android_remove_crash_detail is called.
 */
crash_detail_t* _Nullable android_add_crash_detail(const char* _Nonnull name, const char*  _Nonnull data, size_t n);

/*
 * Remove crash detail from being logged into tombstones.
 * After this function returns, the lifetime of the objects the crash_detail was
 * constructed from no longer needs to be valid.
 */
void android_remove_crash_detail(crash_detail_t* _Nonnull crash_detail);

__END_DECLS
