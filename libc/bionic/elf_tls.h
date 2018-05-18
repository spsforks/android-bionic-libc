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

#pragma once

#include <link.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>

// XXX: DO NOT SUBMIT. Use the correct path for this.
#include "../../linker/linker_allocator.h"

struct TlsSegment {
  size_t size;
  size_t alignment;
  void* init_ptr;
  size_t init_size;
  intptr_t tpoffset;
};

__LIBC_HIDDEN__ TlsSegment __bionic_get_tls_segment(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                                    ElfW(Addr) load_bias);

__LIBC_HIDDEN__ void __bionic_validate_exe_tls_segment(const TlsSegment& seg);

// Dynamic Thread Vector. Each thread has a different DTV. For each module (executable or solib),
// the DTV has a pointer to that module's TLS memory. The DTV is initially empty and is allocated
// on-demand. It grows as more modules are dlopen'ed. See https://www.akkadia.org/drepper/tls.pdf.
struct TlsDtv {
  uintptr_t generation;
  void* modules[];
};

const int kTlsGenerationNone = 0;

// A descriptor for a single module. Typically initialized by the dynamic linker.
struct TlsModule {
  uintptr_t first_generation; // used to detect unloaded modules, can also be kTlsGenerationNone
  bool initset_present;
  intptr_t initset_delta; // offset from TP for an InitSet module
  size_t size;
  size_t alignment;
  const void* init_ptr;
  size_t init_size;
};

struct TlsModules {
  // The initset fields are unchanged after the program is loaded.
  size_t initset_size; // size of TLS region before or after TP
  size_t initset_alignment; // minimum alignment requirement for TP
  const void* initset_data; // pointer to initial data
  intptr_t initset_data_offset; // offset from TP to write the initial data
  size_t initset_data_size; // size of the initial data

  // The fields below are modified by the dynamic linker. They're protected by both the mutex and,
  // for async-signal safety, blocking signals.
  pthread_mutex_t mutex;
  atomic_uintptr_t generation;
  size_t module_count;
  TlsModule* modules;
  LinkerMemoryAllocator allocator;
};

struct TlsIndex {
  unsigned long module;
  unsigned long offset;
};

class pthread_internal_t;
__LIBC_HIDDEN__ void __free_tls_dtv(pthread_internal_t* thread);

#if defined(__i386__)
#define TLS_GET_ADDR_CCONV __attribute__((regparm(1)))
#define TLS_GET_ADDR ___tls_get_addr
#else
#define TLS_GET_ADDR_CCONV
#define TLS_GET_ADDR __tls_get_addr
#endif

extern "C" void* TLS_GET_ADDR(TlsIndex* ti) TLS_GET_ADDR_CCONV;
