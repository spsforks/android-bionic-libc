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

#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>

struct TlsDtv {
  uintptr_t generation;
  void* modules[];
};

const int kTlsGenerationNone = 0;

struct TlsModule {
  uintptr_t first_generation; // can also be kTlsGenerationNone
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
  intptr_t initset_data_offset; // offset from TP to write initial data to
  size_t initset_data_size; // size of the initial data

  pthread_mutex_t mutex;
  atomic_uintptr_t generation;
  size_t module_count;
  TlsModule* modules;
};

struct TlsIndex {
  unsigned long module;
  unsigned long offset;
};

constexpr intptr_t kMaxTlsSlots = 16;
constexpr intptr_t kVariant1TlsReservation = kMaxTlsSlots * sizeof(void*);

// XXX: I thought I'd need __update_tls_dtv when creating a new thread or at
// thread startup to guarantee DTV allocation. So far, I don't need it, so
// __update_tls_dtv could be removed. __free_tls_dtv could lose its
// pthread_internal_t parameter. Maybe we need this for gdb/lldb integration,
// because the debugger will prefer to see an up-to-date DTV.
class pthread_internal_t;
__LIBC_HIDDEN__ bool __update_tls_dtv(pthread_internal_t* thread);
__LIBC_HIDDEN__ void __free_tls_dtv(pthread_internal_t* thread);

#if defined(__i386__)
#define TLS_GET_ADDR_CCONV __attribute__((regparm(1)))
#define TLS_GET_ADDR ___tls_get_addr
#else
#define TLS_GET_ADDR_CCONV
#define TLS_GET_ADDR __tls_get_addr
#endif

extern "C" void* TLS_GET_ADDR(TlsIndex* ti) TLS_GET_ADDR_CCONV;
