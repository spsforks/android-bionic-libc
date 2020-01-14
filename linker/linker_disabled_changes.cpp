/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <atomic>

#include <android/api-level.h>
#include <android/fdsan.h>
#include <async_safe/log.h>

#include "linker.h"

static unsigned long int *g_disabled_compat_changes = nullptr;
static int g_disabled_compat_changes_len = 0;

static int compare(const void* a, const void* b) {
    const unsigned long ai = *(reinterpret_cast<const unsigned long int*>(a));
    const unsigned long bi = *(reinterpret_cast<const unsigned long int*>(b));

    if (ai < bi) {
        return -1;
    } else if (ai > bi) {
        return 1;
    } else {
        return 0;
    }
}

void set_application_disabled_changes(unsigned long int* disabled_compat_changes, int len) {
    g_disabled_compat_changes = reinterpret_cast<unsigned long int*>
                                    (malloc(len * sizeof(unsigned long int)));
    for( int i=0; i < len; i++) {
        g_disabled_compat_changes[i] = disabled_compat_changes[i];
    }
    qsort(g_disabled_compat_changes, len, sizeof(unsigned long int), compare);
    g_disabled_compat_changes_len = len;
}


// Query if a given compatibility change is enabled for the current process. This method is
// intended to be called by code running inside a process of the affected app only.
// In a non app process all changes would be enabled.
//
// If this method returns 1, the calling code should implement the compatibility
// change, resulting in differing behaviour compared to earlier releases. If this method returns
// 0, the calling code should behave as it did in earlier releases.
//
// @param change_id The ID of the compatibility change in question.
// @return 1 if the change is enabled for the current app, 0 otherwise.
int is_change_enabled(unsigned long int change_id) {
    if (g_disabled_compat_changes != nullptr) {
        if (bsearch(&change_id, g_disabled_compat_changes, g_disabled_compat_changes_len,
                    sizeof(unsigned long int), compare) != NULL) {
            async_safe_format_log(ANDROID_LOG_DEBUG, "linker",
                                  "Compat change id reported: %lu; state: DISABLED", change_id);
            return 0;
        }
    }
    async_safe_format_log(ANDROID_LOG_DEBUG, "linker",
                          "Compat change id reported: %lu; state: ENABLED", change_id);
    return 1;
}

