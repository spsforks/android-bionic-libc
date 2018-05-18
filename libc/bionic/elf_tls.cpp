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

#include "elf_tls.h"

#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/param.h>

#include "pthread_internal.h"
#include "private/bionic_globals.h"
#include "private/ScopedSignalBlocker.h"
#include "private/ScopedPthreadMutexLocker.h"

// Returns info on a TLS segment inside a phdr table.
TlsSegment __bionic_get_tls_segment(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                    ElfW(Addr) load_bias) {
  TlsSegment result = {
    .tpoffset = INTPTR_MIN,
  };
  for (size_t i = 0; i < phdr_count; ++i) {
    const ElfW(Phdr)& phdr = phdr_table[i];
    if (phdr.p_type == PT_TLS) {
      result.size = phdr.p_memsz;
      result.alignment = phdr.p_align;
      result.init_ptr = reinterpret_cast<void*>(load_bias + phdr.p_vaddr);
      result.init_size = phdr.p_filesz;
    } else if (phdr.p_type == PT_ANDROID_TLS_TPOFFSET) {
      result.tpoffset = phdr.p_memsz;
    }
  }
  return result;
}

void __bionic_validate_exe_tls_segment(const TlsSegment& seg) {
  if (seg.tpoffset == INTPTR_MIN) {
    async_safe_fatal("error: executable with PT_TLS segment lacks PT_ANDROID_TLS_TPOFFSET segment");
  }
  if (seg.alignment == 0 || !powerof2(seg.alignment) ||
      seg.tpoffset < BIONIC_TLS_RESERVATION_FULL || seg.tpoffset % seg.alignment != 0) {
    // With variant 1 TLS, both the thread pointer and the TLS segment must be aligned to the
    // alignment of the TLS segment, so the tpoffset must be a multiple of the alignment.
    async_safe_fatal("error: invalid executable tpoffset (%ld) or TLS alignment (%ld)",
      static_cast<long>(seg.tpoffset), static_cast<long>(seg.alignment));
  }
}

static inline TlsModules& tls_modules() {
  return __libc_shared_globals->tls_modules;
}

static inline TlsDtv* get_thread_dtv(pthread_internal_t* thread) {
  return static_cast<TlsDtv*>(thread->tls[TLS_SLOT_DTV]);
}

static inline void set_thread_dtv(pthread_internal_t* thread, TlsDtv* val) {
  thread->tls[TLS_SLOT_DTV] = val;
}

static inline TlsDtv* get_dtv() {
  return static_cast<TlsDtv*>(__get_tls()[TLS_SLOT_DTV]);
}

static void __update_tls_dtv(pthread_internal_t* thread) {
  ScopedSignalBlocker ssb;
  ScopedPthreadMutexLocker locker(&tls_modules().mutex);

  const size_t old_cnt = thread->dtv_module_count;
  const size_t loaded_cnt = tls_modules().module_count;

  // Grow the DTV to at least the size of tls_modules().module->count. Never shrink the DTV.
  if (loaded_cnt > old_cnt) {
    TlsDtv* const old_dtv = get_thread_dtv(thread);
    TlsDtv* const new_dtv =
      static_cast<TlsDtv*>(tls_modules().allocator.alloc(sizeof(TlsDtv) + loaded_cnt * sizeof(void*)));
    memcpy(new_dtv, old_dtv, sizeof(TlsDtv) + old_cnt * sizeof(void*));
    set_thread_dtv(thread, new_dtv);
    thread->dtv_module_count = loaded_cnt;
    // When the dynamic loader initializes the main thread, it points the DTV slot at a static DTV
    // with generation 0. Don't try to free that DTV.
    if (old_dtv && old_dtv->generation != 0) {
      tls_modules().allocator.free(old_dtv);
    }
  }

  TlsDtv* const dtv = get_thread_dtv(thread);

  // Initialize initset modules and free unloaded modules.
  for (size_t i = 0; i < thread->dtv_module_count; ++i) {
    if (i < loaded_cnt) {
      const TlsModule& module = tls_modules().modules[i];
      if (module.initset_present) {
        dtv->modules[i] = reinterpret_cast<char*>(&thread->tls) + module.initset_delta;
        continue;
      }
      if (module.first_generation != kTlsGenerationNone &&
          module.first_generation <= dtv->generation) {
        continue;
      }
    }
    tls_modules().allocator.free(dtv->modules[i]);
    dtv->modules[i] = nullptr;
  }

  dtv->generation = atomic_load(&tls_modules().generation);
}

// The caller must have already blocked signals.
void __free_tls_dtv(pthread_internal_t* thread) {
  ScopedPthreadMutexLocker locker(&tls_modules().mutex);

  const size_t loaded_cnt = tls_modules().module_count;

  TlsDtv* dtv = get_thread_dtv(thread);
  for (size_t i = 0; i < thread->dtv_module_count; ++i) {
    if (i < loaded_cnt && tls_modules().modules[i].initset_present) {
      // This module's TLS memory is allocated statically, so don't free it here.
      continue;
    }
    tls_modules().allocator.free(dtv->modules[i]);
  }
  if (dtv && dtv->generation != 0) {
    // As when updating a DTV, don't free a DTV of generation 0.
    tls_modules().allocator.free(dtv);
  }
  thread->dtv_module_count = 0;
  set_thread_dtv(thread, nullptr);
}

static void* tls_allocate(unsigned long module) {
  ScopedSignalBlocker ssb;
  ScopedPthreadMutexLocker locker(&tls_modules().mutex);

  const TlsModule& desc = tls_modules().modules[module];
  void* ptr = tls_modules().allocator.memalign(desc.alignment, desc.size);
  if (desc.init_size > 0) {
    memcpy(ptr, desc.init_ptr, desc.init_size);
  }
  return ptr;
}

// __tls_get_addr or ___tls_get_addr.
extern "C" void* TLS_GET_ADDR(TlsIndex* ti) TLS_GET_ADDR_CCONV {
  TlsDtv* dtv = get_dtv();
  const unsigned long module = ti->module - 1;

  // XXX: Can/should we use a relaxed order to read the generation counter? For this particular
  // variable, I would expect the thread that loaded it to have synchronized with the current
  // thread. Some thread we haven't synchronized with, though, could also have bumped the
  // generation counter.
  const auto generation = atomic_load(&tls_modules().generation);
  if (__predict_false(generation != dtv->generation)) {
    __update_tls_dtv(__get_thread());
    dtv = get_dtv();
  }

  auto& dtv_slot = dtv->modules[module];
  if (__predict_false(dtv_slot == nullptr)) {
    dtv_slot = tls_allocate(module);
  }

  return static_cast<char*>(dtv_slot) + ti->offset;
}
