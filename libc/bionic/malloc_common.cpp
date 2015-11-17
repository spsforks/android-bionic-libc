/*
 * Copyright (C) 2009 The Android Open Source Project
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

// Contains a thin layer that calls whatever real native allocator
// has been defined. For the libc shared library, this allows the
// implementation of a debug malloc that can intercept all of the allocation
// calls and add special debugging code to attempt to catch allocation
// errors. All of the debugging code is implemented in a separate shared
// library that is only loaded when the property "libc.debug.malloc.options"
// is set to a non-zero value. There are two functions exported to
// allow ddms, or other external users to get information from the debug
// allocation.
//   get_malloc_leak_info: Returns information about all of the known native
//                         allocations that are currently in use.
//   free_malloc_leak_info: Frees the data allocated by the call to
//                          get_malloc_leak_info.

#include <private/bionic_config.h>
#include <private/bionic_globals.h>
#include <private/bionic_malloc_dispatch.h>

#if defined(USE_JEMALLOC)
#include "jemalloc.h"
#define Malloc(function)  je_ ## function
#elif defined(USE_DLMALLOC)
#include "dlmalloc.h"
#define Malloc(function)  dl ## function
#else
#error "Either one of USE_DLMALLOC or USE_JEMALLOC must be defined."
#endif

static constexpr MallocDispatch __libc_malloc_default_dispatch
  __attribute__((unused)) = {
    Malloc(calloc),
    Malloc(free),
    Malloc(mallinfo),
    Malloc(malloc),
    Malloc(malloc_usable_size),
    Malloc(memalign),
    Malloc(posix_memalign),
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
    Malloc(pvalloc),
#endif
    Malloc(realloc),
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
    Malloc(valloc),
#endif
  };

// In a VM process, this is set to 1 after fork()ing out of zygote.
int gMallocLeakZygoteChild = 0;

// =============================================================================
// Allocation functions
// =============================================================================
extern "C" void* calloc(size_t n_elements, size_t elem_size) {
  auto _calloc = __libc_globals->malloc_dispatch.calloc;
  if (__predict_false(_calloc != nullptr)) {
    return _calloc(n_elements, elem_size);
  }
  return Malloc(calloc)(n_elements, elem_size);
}

extern "C" void free(void* mem) {
  auto _free = __libc_globals->malloc_dispatch.free;
  if (__predict_false(_free != nullptr)) {
    _free(mem);
  } else {
    Malloc(free)(mem);
  }
}

extern "C" struct mallinfo mallinfo() {
  auto _mallinfo = __libc_globals->malloc_dispatch.mallinfo;
  if (__predict_false(_mallinfo != nullptr)) {
    return _mallinfo();
  }
  return Malloc(mallinfo)();
}

extern "C" void* malloc(size_t bytes) {
  auto _malloc = __libc_globals->malloc_dispatch.malloc;
  if (__predict_false(_malloc != nullptr)) {
    return _malloc(bytes);
  }
  return Malloc(malloc)(bytes);
}

extern "C" size_t malloc_usable_size(const void* mem) {
  auto _malloc_usable_size = __libc_globals->malloc_dispatch.malloc_usable_size;
  if (__predict_false(_malloc_usable_size != nullptr)) {
    return _malloc_usable_size(mem);
  }
  return Malloc(malloc_usable_size)(mem);
}

extern "C" void* memalign(size_t alignment, size_t bytes) {
  auto _memalign = __libc_globals->malloc_dispatch.memalign;
  if (__predict_false(_memalign != nullptr)) {
    return _memalign(alignment, bytes);
  }
  return Malloc(memalign)(alignment, bytes);
}

extern "C" int posix_memalign(void** memptr, size_t alignment, size_t size) {
  auto _posix_memalign = __libc_globals->malloc_dispatch.posix_memalign;
  if (__predict_false(_posix_memalign != nullptr)) {
    return _posix_memalign(memptr, alignment, size);
  }
  return Malloc(posix_memalign)(memptr, alignment, size);
}

extern "C" void* realloc(void* old_mem, size_t bytes) {
  auto _realloc = __libc_globals->malloc_dispatch.realloc;
  if (__predict_false(_realloc != nullptr)) {
    return _realloc(old_mem, bytes);
  }
  return Malloc(realloc)(old_mem, bytes);
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
extern "C" void* pvalloc(size_t bytes) {
  auto _pvalloc = __libc_globals->malloc_dispatch.pvalloc;
  if (__predict_false(_pvalloc != nullptr)) {
    return _pvalloc(bytes);
  }
  return Malloc(pvalloc)(bytes);
}

extern "C" void* valloc(size_t bytes) {
  auto _valloc = __libc_globals->malloc_dispatch.valloc;
  if (__predict_false(_valloc != nullptr)) {
    return _valloc(bytes);
  }
  return Malloc(valloc)(bytes);
}
#endif

// We implement malloc debugging only in libc.so, so the code below
// must be excluded if we compile this file for static libc.a
#if !defined(LIBC_STATIC)

#include <dlfcn.h>
#include <pthread.h>
#include <stdio.h>

#include <private/bionic_malloc_debug.h>
#include <sys/system_properties.h>

static const char* DEBUG_SHARED_LIB = "libc_malloc_debug.so";
static const char* DEBUG_MALLOC_PROPERTY_OPTIONS = "libc.debug.malloc.options";
static const char* DEBUG_MALLOC_PROPERTY_PROGRAM = "libc.debug.malloc.program";

static void* libc_malloc_impl_handle = nullptr;

// =============================================================================
// Exported for use by ddms.
// =============================================================================

// Retrieve native heap information.
//
// "*info" is set to a buffer we allocate
// "*overall_size" is set to the size of the "info" buffer
// "*info_size" is set to the size of a single entry
// "*total_memory" is set to the sum of all allocations we're tracking; does
//   not include heap overhead
// "*backtrace_size" is set to the maximum number of entries in the back trace
extern "C" void get_malloc_leak_info(uint8_t** info, size_t* overall_size,
    size_t* info_size, size_t* total_memory, size_t* backtrace_size) {
  if (libc_malloc_impl_handle == nullptr) {
    return;
  }
  void* sym = dlsym(libc_malloc_impl_handle, "debug_get_malloc_leak_info");
  auto info_func = reinterpret_cast<void (*)(uint8_t**, size_t*, size_t*, size_t*, size_t*)>(sym);
  if (info_func == nullptr) {
    return;
  }
  info_func(info, overall_size, info_size, total_memory, backtrace_size);
}

extern "C" void free_malloc_leak_info(uint8_t* info) {
  if (libc_malloc_impl_handle == nullptr) {
    return;
  }
  void* sym = dlsym(libc_malloc_impl_handle, "debug_free_malloc_leak_info");
  auto free_func = reinterpret_cast<void (*)(uint8_t*)>(sym);
  if (free_func == nullptr) {
    return;
  }
  free_func(info);
}
// =============================================================================

template<typename FunctionType>
static bool InitMallocFunction(void* malloc_impl_handler, FunctionType* func, const char* prefix, const char* suffix) {
  char symbol[128];
  snprintf(symbol, sizeof(symbol), "%s_%s", prefix, suffix);
  *func = reinterpret_cast<FunctionType>(dlsym(malloc_impl_handler, symbol));
  if (*func == nullptr) {
    error_log("%s: dlsym(\"%s\") failed", getprogname(), symbol);
    return false;
  }
  return true;
}

static bool InitMalloc(void* malloc_impl_handler, MallocDispatch* table, const char* prefix) {
  if (!InitMallocFunction<MallocCalloc>(malloc_impl_handler, &table->calloc,
                                        prefix, "calloc")) {
    return false;
  }
  if (!InitMallocFunction<MallocFree>(malloc_impl_handler, &table->free,
                                      prefix, "free")) {
    return false;
  }
  if (!InitMallocFunction<MallocMallinfo>(malloc_impl_handler, &table->mallinfo,
                                          prefix, "mallinfo")) {
    return false;
  }
  if (!InitMallocFunction<MallocMalloc>(malloc_impl_handler, &table->malloc,
                                        prefix, "malloc")) {
    return false;
  }
  if (!InitMallocFunction<MallocMallocUsableSize>(
      malloc_impl_handler, &table->malloc_usable_size, prefix, "malloc_usable_size")) {
    return false;
  }
  if (!InitMallocFunction<MallocMemalign>(malloc_impl_handler, &table->memalign,
                                          prefix, "memalign")) {
    return false;
  }
  if (!InitMallocFunction<MallocPosixMemalign>(malloc_impl_handler, &table->posix_memalign,
                                               prefix, "posix_memalign")) {
    return false;
  }
  if (!InitMallocFunction<MallocRealloc>(malloc_impl_handler, &table->realloc,
                                         prefix, "realloc")) {
    return false;
  }
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  if (!InitMallocFunction<MallocPvalloc>(malloc_impl_handler, &table->pvalloc,
                                         prefix, "pvalloc")) {
    return false;
  }
  if (!InitMallocFunction<MallocValloc>(malloc_impl_handler, &table->valloc,
                                        prefix, "valloc")) {
    return false;
  }
#endif

  return true;
}

// Initializes memory allocation framework once per process.
static void malloc_init_impl(libc_globals* globals) {
  char value[PROP_VALUE_MAX];
  if (__system_property_get(DEBUG_MALLOC_PROPERTY_OPTIONS, value) != 0 || value[0] == '\0') {
    return;
  }

  // Check to see if only a specific program should have debug malloc enabled.
  if (__system_property_get(DEBUG_MALLOC_PROPERTY_PROGRAM, value) == 0 &&
      strstr(getprogname(), value) != nullptr) {
    return;
  }

  // Load the debug malloc shared library.
  void* malloc_impl_handle = dlopen(DEBUG_SHARED_LIB, RTLD_NOW);
  if (malloc_impl_handle == nullptr) {
    error_log("%s: Unable to open debug malloc shared library %s: %s",
              getprogname(), DEBUG_SHARED_LIB, dlerror());
    return;
  }

  // Initialize malloc debugging in the loaded module.
  void* sym = dlsym(malloc_impl_handle, "debug_initialize");
  auto init_func = reinterpret_cast<bool (*)(const MallocDispatch*)>(sym);
  if (init_func == nullptr) {
    error_log("%s: Initialization routine not found in %s\n", getprogname(), DEBUG_SHARED_LIB);
    dlclose(malloc_impl_handle);
    return;
  }
  if (!init_func(&__libc_malloc_default_dispatch)) {
    dlclose(malloc_impl_handle);
    return;
  }

  MallocDispatch malloc_dispatch_table;
  if (!InitMalloc(malloc_impl_handle, &malloc_dispatch_table, "debug")) {
    dlclose(malloc_impl_handle);
    return;
  }

  globals->malloc_dispatch = malloc_dispatch_table;
  libc_malloc_impl_handle = malloc_impl_handle;
}

static void malloc_fini_impl() {
  if (libc_malloc_impl_handle != nullptr) {
    void* sym = dlsym(libc_malloc_impl_handle, "debug_finalize");
    auto finalize_func = reinterpret_cast<void (*)()>(sym);
    if (finalize_func != nullptr) {
      // Our BSD stdio implementation doesn't close the standard streams,
      // it only flushes them. And it doesn't do that until its atexit
      // handler is run, and we run first! It's great that other unclosed
      // FILE*s show up as malloc leaks, but we need to manually clean up
      // the standard streams ourselves.
      fclose(stdin);
      fclose(stdout);
      fclose(stderr);

      finalize_func();
    }
  }
}
#endif  // !LIBC_STATIC

// Initializes memory allocation framework.
// This routine is called from __libc_init routines in libc_init_dynamic.cpp.
__LIBC_HIDDEN__ void __libc_init_malloc(libc_globals* globals) {
  (void)globals;
#if !defined(LIBC_STATIC)
  malloc_init_impl(globals);
#endif
}

extern "C" __LIBC_HIDDEN__ void malloc_debug_fini() {
#if !defined(LIBC_STATIC)
  static pthread_once_t malloc_fini_once_ctl = PTHREAD_ONCE_INIT;
  if (pthread_once(&malloc_fini_once_ctl, malloc_fini_impl)) {
    error_log("Unable to finalize malloc_debug component.");
  }
#endif  // !LIBC_STATIC
}
