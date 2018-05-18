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

// XXX: What about the initial TLS block?

#include "elf_tls.h"

#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/param.h>

#include "pthread_internal.h"
#include "private/bionic_globals.h"
#include "private/ScopedPthreadMutexLocker.h"

extern "C" void je_bootstrap_free(void* ptr);
extern "C" void* je_bootstrap_calloc(size_t num, size_t size);
extern "C" void* je_bootstrap_malloc(size_t size);
extern "C" void* je_bootstrap_memalign(size_t alignment, size_t size);

static inline TlsModules& tls_modules() {
  return __libc_shared_globals->tls_modules;
}

static inline TlsDtv* get_thread_dtv(pthread_internal_t* thread) {
  return static_cast<TlsDtv*>(thread->tcb->slots[TLS_SLOT_DTV]);
}

static inline void set_thread_dtv(pthread_internal_t* thread, TlsDtv* val) {
  thread->tcb->slots[TLS_SLOT_DTV] = val;
}

static inline TlsDtv* get_dtv() {
  return static_cast<TlsDtv*>(__get_tls()[TLS_SLOT_DTV]);
}

bool __update_tls_dtv(pthread_internal_t* thread) {
  ScopedPthreadMutexLocker locker(&tls_modules().mutex);

  const size_t old_cnt = thread->dtv_module_count;
  const size_t loaded_cnt = tls_modules().module_count;

  // Grow the DTV to at least the size of tls_modules().module->count. Never shrink the DTV.
  if (loaded_cnt > old_cnt) {
    TlsDtv* const old_dtv = get_thread_dtv(thread);
    const size_t new_cnt = MAX(loaded_cnt, 1);
    TlsDtv* const new_dtv =
      static_cast<TlsDtv*>(je_bootstrap_calloc(1, sizeof(TlsDtv) + new_cnt * sizeof(void*)));
    if (!new_dtv) return false;
    memcpy(new_dtv, old_dtv, sizeof(TlsDtv) + old_cnt * sizeof(void*));
    set_thread_dtv(thread, new_dtv);
    thread->dtv_module_count = new_cnt;
    if (old_dtv && old_dtv->generation != 0) {
      // When the dynamic loader initializes the main thread, it can't use jemalloc to allocate
      // a DTV, so instead it points the DTV slot at a static DTV with generation 0. Don't try to
      // free that DTV.
      je_bootstrap_free(old_dtv);
    }
  }

  TlsDtv* const dtv = get_thread_dtv(thread);

  // Initialize initset modules and free unloaded modules.
  for (size_t i = 0; i < thread->dtv_module_count; ++i) {
    if (i < loaded_cnt) {
      const TlsModule& module = tls_modules().modules[i];
      if (module.initset_present) {
        dtv->modules[i] = reinterpret_cast<char*>(&thread->tcb->slots) + module.initset_delta;
        continue;
      }
      if (module.first_generation != kTlsGenerationNone &&
          module.first_generation <= dtv->generation) {
        continue;
      }
    }
    je_bootstrap_free(dtv->modules[i]);
    dtv->modules[i] = nullptr;
  }

  dtv->generation = tls_modules().generation; // XXX: does this need to use atomic_load?
  return true;
}

void __free_tls_dtv(pthread_internal_t* thread) {
  ScopedPthreadMutexLocker locker(&tls_modules().mutex);

  const size_t loaded_cnt = tls_modules().module_count;

  TlsDtv* dtv = get_thread_dtv(thread);
  for (size_t i = 0; i < thread->dtv_module_count; ++i) {
    if (i < loaded_cnt && tls_modules().modules[i].initset_present) {
      // This module is allocated in the TCB (Initial TLS Block), so don't free it here.
      continue;
    }
    je_bootstrap_free(dtv->modules[i]);
  }
  if (dtv && dtv->generation != 0) {
    // As when updating a DTV, don't free a DTV of generation 0.
    je_bootstrap_free(dtv);
  }
  thread->dtv_module_count = 0;
  set_thread_dtv(thread, nullptr);
}

static void* tls_allocate(unsigned long module) {
  ScopedPthreadMutexLocker locker(&tls_modules().mutex);

  const TlsModule& desc = tls_modules().modules[module];
  void* ptr = je_bootstrap_memalign(desc.alignment, desc.size);
  if (!ptr) abort();
  if (desc.init_size) memcpy(ptr, desc.init_ptr, desc.init_size);
  memset(static_cast<char*>(ptr) + desc.init_size, 0, desc.size - desc.init_size);
  return ptr;
}

// __tls_get_addr or ___tls_get_addr.
extern "C" void* TLS_GET_ADDR(TlsIndex* ti) TLS_GET_ADDR_CCONV {
  TlsDtv* dtv = get_dtv();
  const unsigned long module = ti->module - 1;

  // XXX: If/when the generation count loops, bad things happen?
  // 32-bit targets... (pthread sequences already have this issue; it's probably OK?)
  const auto generation = atomic_load(&tls_modules().generation);
  if (__predict_false(generation != dtv->generation)) {
    if (!__update_tls_dtv(__get_thread())) abort();
    dtv = get_dtv();
  }

  auto& dtv_slot = dtv->modules[module];
  if (__predict_false(dtv_slot == nullptr)) {
    dtv_slot = tls_allocate(module);
  }

  return static_cast<char*>(dtv_slot) + ti->offset;
}
