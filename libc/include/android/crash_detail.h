/*
 * Copyright (C) 2024 The Android Open Source Project
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
 * @file android/crash_detail.h
 * @brief Attach extra information to android crashes.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

typedef struct ACrashDetail ACrashDetail;

/**
 * Register a new buffer to get logged into tombstones for crashes.
 *
 * The lifetime of name and data has to be valid until the program crashes, or until
 * ACrashDetail_unregister is called.
 *
 * Example usage:
 *   const char* stageName = "garbage_collection";
 *   ACrashDetail* cd = ACrashDetail_register("stage", stageName, strlen(stageName));
 *   do_garbage_collection();
 *   ACrashDetail_unregister(cd);
 *
 * Introduced in API 35.
 *
 * \param name identifying name for this extra data.
 *             needs to be a valid ASCII-string.
 * \param name_size number of bytes of the buffer pointed to by name
 * \param data a buffer containing the extra detail bytes
 * \param data_size number of bytes of the buffer pointed to by data
 *
 * \return a handle to the extra crash detail for use with ACrashDetail_unregister.
 */
ACrashDetail* _Nullable ACrashDetail_register(
    const char* _Nonnull name, size_t name_size, const char* _Nonnull data, size_t data_size) __INTRODUCED_IN(35);

/**
 * Unregister crash detail from being logged into tombstones.
 *
 * After this function returns, the lifetime of the objects crash_detail was
 * constructed from no longer needs to be valid.
 *
 * Introduced in API 35.
 *
 * \param crash_detail the crash_detail that should be removed.
 *
 * \return the data pointer that was used to construct the crash_detail.
 *         you can generally ignore the return value, but it can be useful
 *         if you need to free its allocation.
 */
const char* _Nullable ACrashDetail_unregister(ACrashDetail* _Nonnull crash_detail) __INTRODUCED_IN(35);

__END_DECLS
