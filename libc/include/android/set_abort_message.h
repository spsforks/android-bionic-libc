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
#include <stdint.h>
#include <string.h>
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
 * Register a new buffer to get logged into tombstones for crashes.
 *
 * It will be added to both the tombstone proto in the crash_detail field, and
 * in the tombstone text format.
 *
 * Tombstone proto definition:
 *   https://cs.android.com/android/platform/superproject/main/+/main:system/core/debuggerd/proto/tombstone.proto
 *
 * The lifetime of name and data has to be valid until the program crashes, or until
 * android_unregister_crash_detail is called.
 *
 * Example usage:
 *   const char* stageName = "garbage_collection";
 *   crash_detail_t* cd = android_register_crash_detail("stage", stageName, strlen(stageName));
 *   do_garbage_collection();
 *   android_unregister_crash_detail(cd);
 *
 * If this example crashes in do_garbage_collection, a line will show up in the textual representation of the tombstone:
 *   Extra crash detail: stage: 'garbage_collection'
 *
 * Introduced in API 35.
 *
 * \param name identifying name for this extra data.
 * \param name_size number of bytes of the buffer pointed to by name
 * \param data a buffer containing the extra detail bytes, if null the crash detail
 *             is disabled until android_replace_crash_detail_data replaces it with
 *             a non-null pointer.
 * \param data_size number of bytes of the buffer pointed to by data
 *
 * \return a handle to the extra crash detail.
 */
crash_detail_t* _Nullable android_register_crash_detail(
    const char* _Nonnull name, size_t name_size, const void* _Nullable data, size_t data_size) __INTRODUCED_IN(35);

/**
 * Unregister crash detail from being logged into tombstones.
 *
 * After this function returns, the lifetime of the objects crash_detail was
 * constructed from no longer needs to be valid.
 *
 * Introduced in API 35.
 *
 * \param crash_detail the crash_detail that should be removed.
 */
void android_unregister_crash_detail(crash_detail_t* _Nonnull crash_detail) __INTRODUCED_IN(35);

/**
 * Replace data of crash detail.
 *
 * This is more efficient than using android_unregister_crash_detail followed by
 * android_register_crash_detail. If you very frequently need to swap out the data,
 * you can hold onto the crash_detail.
 *
 * Introduced in API 35.
 *
 * \param data the new buffer containing the extra detail bytes, or null to disable until
 *             android_replace_crash_detail_data is called again with non-null data.
 * \param data_size the number of bytes of the buffer pointed to by data.
 */
void android_replace_crash_detail_data(crash_detail_t* _Nonnull crash_detail, const void* _Nullable data, size_t data_size);

/**
 * Replace name of crash detail.
 *
 * This is more efficient than using android_unregister_crash_detail followed by
 * android_register_crash_detail. If you very frequently need to swap out the name,
 * you can hold onto the crash_detail.
 *
 * Introduced in API 35.
 *
 * \param name identifying name for this extra data.
 * \param name_size number of bytes of the buffer pointed to by name
 */
void android_replace_crash_detail_name(crash_detail_t* _Nonnull crash_detail, const char* _Nonnull name, size_t name_size);

__END_DECLS
