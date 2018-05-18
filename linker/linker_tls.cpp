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

#include "linker_tls.h"

#include <algorithm>
#include <vector>

#include <async_safe/log.h>
#include <string.h>

#include "bionic/elf_tls.h"
#include "private/ScopedPthreadMutexLocker.h"
#include "private/bionic_globals.h"
#include "private/bionic_tls.h"
#include "linker_soinfo.h"

static std::vector<TlsModule> tls_module_array;

static inline TlsModules& tls_modules() {
  return __libc_shared_globals->tls_modules;
}

void init_linker_tls_modules() {
  TlsModules initial = {
    .initset_alignment = 1,
    .initset_data = "",
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .generation = { 1 },
  };
  // XXX: std::atomic<uintptr_t> has no copy ctor...
  memcpy(&tls_modules(), &initial, sizeof(initial));
}

TLS_GET_ADDR_CCONV static void* stub_tls_get_addr(TlsIndex* ti __attribute__((unused))) {
  async_safe_fatal("thread_local variable from dlopen'ed library accessed prior to libc.so "
                   "initialization");
  return nullptr;
}

__LIBC_HIDDEN__ void* (*__ld_tls_get_addr)(TlsIndex* ti) TLS_GET_ADDR_CCONV = stub_tls_get_addr;

static void create_tls_initset(const soinfo_list_t& modules, bool has_exe) {
  intptr_t off = 0;
  size_t max_align = 1;

  // Lay out each module of the Initial TLS Block. Calculate its offset from
  // the thread-pointer, and calculate the total size and alignment of the
  // Initial TLS block.
  modules.for_each([&off, &max_align, &has_exe](soinfo* si) {
    TlsModule& mod = tls_module_array[si->get_tls_module_id() - 1];
    max_align = std::max(max_align, mod.alignment);
    // XXX: Factor out this BIONIC_TLS_VARIANT1-vs-BIONIC_TLS_VARIANT2 layout code somehow...
#if defined(BIONIC_TLS_VARIANT1)
    off = std::max<intptr_t>(off, sizeof(void*) * 2);
    off = __BIONIC_ALIGN(off, mod.alignment);
    // For variant 1 archs (arm/arm64), an executable's PT_TLS segment must be sufficiently aligned
    // to reserve enough space for Bionic's TLS slots. The static linker will bake TP-offsets into
    // an executable. For a shared object, we can put the TLS data at any offset, so we simply
    // reserve the needed space.
    if (off < kVariant1TlsReservation) {
      if (has_exe) {
        async_safe_fatal("Executable's underaligned PT_TLS overlaps with Bionic slots: offset %ld, required %ld",
                         static_cast<long>(off), static_cast<long>(kVariant1TlsReservation));
      } else {
        off = kVariant1TlsReservation;
      }
    }
    mod.initset_delta = off;
    off += mod.size;
#elif defined(BIONIC_TLS_VARIANT2)
    off -= mod.size;
    off &= ~(mod.alignment - 1);
    mod.initset_delta = off;
    (void)has_exe; // suppress compiler error
#else
    #error "neither BIONIC_TLS_VARIANT1 nor BIONIC_TLS_VARIANT2 defined"
#endif
    mod.initset_present = true;
    si->set_tls_initset_delta(mod.initset_delta);
  });

#if defined(BIONIC_TLS_VARIANT1)
  // XXX: This alignment might not be necessary for variant 1, but I think it
  // simplifies things anyway.
  tls_modules().initset_size = __BIONIC_ALIGN(off, max_align);
  intptr_t init_bias = 0;
#elif defined(BIONIC_TLS_VARIANT2)
  tls_modules().initset_size = -(off & ~(max_align - 1));
  intptr_t init_bias = tls_modules().initset_size;
#endif

  tls_modules().initset_alignment = max_align;
  tls_modules().initset_data_offset = -init_bias;
  tls_modules().initset_data_size = tls_modules().initset_size;
  void* initset_data = calloc(tls_modules().initset_size, 1);
  tls_modules().initset_data = initset_data;
  if (initset_data == nullptr) {
    // XXX: this code is unreachable because the linker's calloc aborts on OOM.
    // What's the right convention/API to use? Call calloc w/o checking NULL?
    async_safe_fatal("OOM on TLS initset region");
  }

  // Copy each module's .tdata into the combined initset template.
  modules.for_each([init_bias, initset_data](soinfo* si) {
    TlsModule& mod = tls_module_array[si->get_tls_module_id() - 1];
    memcpy(static_cast<char*>(initset_data) + (mod.initset_delta + init_bias),
           mod.init_ptr,
           mod.init_size);
  });
}

void register_tls_modules(const soinfo_list_t& modules, bool is_initial, bool has_exe) {
  ScopedPthreadMutexLocker locker(&tls_modules().mutex);

  uintptr_t next_mod_id = 1;
  auto new_mod_id = [&next_mod_id]() -> uintptr_t {
    while (true) {
      auto candidate = next_mod_id++;
      if (candidate > tls_module_array.size()) {
        tls_module_array.resize(candidate);
        tls_modules().module_count = tls_module_array.size();
        tls_modules().modules = tls_module_array.data();
      }
      if (tls_module_array[candidate - 1].first_generation == kTlsGenerationNone) {
        return candidate;
      }
    }
  };

  ++tls_modules().generation;

  modules.for_each([&new_mod_id](soinfo* si) {
    CHECK(si->get_tls_segment() != nullptr);
    if (si->get_tls_module_id()) {
      return;
    }
    const uintptr_t mod_id = new_mod_id();
    si->set_tls_module_id(mod_id);
    si->set_tls_generation(tls_modules().generation);
    TlsModule& mod = tls_module_array[mod_id - 1];
    const TlsSegment& seg = *si->get_tls_segment();
    mod = {
      .first_generation = tls_modules().generation,
      .size = seg.size,
      .alignment = seg.alignment,
      .init_ptr = seg.init_ptr,
      .init_size = seg.init_size,
    };
  });

  if (is_initial) {
    create_tls_initset(modules, has_exe);
  }
}

void unregister_tls_modules(const soinfo_list_t& modules) {
  ScopedPthreadMutexLocker locker(&tls_modules().mutex);

  ++tls_modules().generation;

  modules.for_each([](soinfo* si) {
    const int module_id = si->get_tls_module_id();
    if (module_id) {
      tls_module_array[module_id - 1] = {};
      si->set_tls_module_id(0);
      si->set_tls_generation(0);
    }
  });
}

void* tcb_memalign(size_t alignment, size_t size) {
  alignment = __BIONIC_ALIGN(alignment, alignof(void*));
  size = __BIONIC_ALIGN(size, alignment);
  // XXX: overflow checking?
  void* ptr = malloc(sizeof(void*) + size + alignment);
  if (!ptr) return nullptr;
  uintptr_t ret = __BIONIC_ALIGN(reinterpret_cast<uintptr_t>(ptr) + sizeof(void*), alignment);
  *reinterpret_cast<void**>(ret - sizeof(void*)) = ptr;
  return reinterpret_cast<void*>(ret);
}

void tcb_free(void* ptr) {
  if (!ptr) return;
  free(static_cast<void**>(ptr)[-1]);
}
