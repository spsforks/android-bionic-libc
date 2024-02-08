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

#include <android/crash_detail.h>

#include <async_safe/log.h>
#include <bionic/crash_detail_internal.h>

#include <bits/stdatomic.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>

#include "private/ScopedPthreadMutexLocker.h"
#include "private/bionic_defs.h"
#include "private/bionic_globals.h"

static _Atomic(ACrashDetail*) free_head = nullptr;

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
ACrashDetail* ACrashDetail_register(const char* name, size_t name_size, const char* data,
                                    size_t data_size) {
  auto populate_crash_detail = [&](ACrashDetail* result) {
    result->name = name;
    result->name_size = name_size;
    result->data = data;
    result->data_size = data_size;
  };
  // This is a atomic fast-path for RAII use-cases where the app keeps creating and deleting
  // crash details for short periods of time to capture detailed scopes.
  if (ACrashDetail* head = atomic_load(&free_head)) {
    while (head != nullptr && !atomic_compare_exchange_strong(&free_head, &head, head->prev_free)) {
      // intentionally left blank.
    }
    if (head) {
      head->prev_free = nullptr;
      populate_crash_detail(head);
      return head;
    }
  }
  ScopedPthreadMutexLocker locker(&__libc_shared_globals()->crash_detail_page_lock);
  struct crash_detail_page_t* prev = nullptr;
  struct crash_detail_page_t* page = __libc_shared_globals()->crash_detail_page;
  if (page != nullptr && page->used == kNumCrashDetails) {
    prev = page;
    page = nullptr;
  }
  if (page == nullptr) {
    size_t size = sizeof(crash_detail_page_t);
    // TODO(fmayer): consider using LinkerBlockAllocator.
    void* map = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (map == MAP_FAILED) {
      async_safe_format_log(ANDROID_LOG_ERROR, "libc", "failed to allocate crash_detail_page: %m");
      return nullptr;
    }
    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, map, size, "crash details");
    page = reinterpret_cast<struct crash_detail_page_t*>(map);
    page->prev = prev;
    __libc_shared_globals()->crash_detail_page = page;
  }
  ACrashDetail* result = &page->crash_details[page->used];
  populate_crash_detail(result);
  page->used++;
  return result;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
const char* ACrashDetail_unregister(ACrashDetail* crash_detail) {
  if (crash_detail) {
    const char* data = crash_detail->data;
    if (crash_detail->prev_free) {
      // removing already removed would mess up the free-list by creating a circle.
      return nullptr;
    }
    crash_detail->data = nullptr;
    crash_detail->name = nullptr;
    ACrashDetail* prev = atomic_load(&free_head);
    do {
      crash_detail->prev_free = prev;
    } while (!atomic_compare_exchange_strong(&free_head, &prev, crash_detail));
    return data;
  }
  return nullptr;
}
