/*  $OpenBSD: atexit.c,v 1.20 2014/07/11 09:51:37 kettenis Exp $ */
/*
 * Copyright (c) 2002 Daniel Hartmeier
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stddef.h>
#include <stdint.h>
#include "private/bionic_globals.h"
#include "private/thread_private.h"
#include "private/WPVector.h"
#include "atexit.h"

extern "C" void __unregister_atfork(void* dso);

void __libc_init_atexit(libc_globals* globals) {
  globals->atexit = WPVector<struct atexit_fn>::allocate("atexit handlers");
}

/*
 * Register a function to be performed at exit or when a shared object
 * with the given dso handle is unloaded dynamically.  Also used as
 * the backend for atexit().  For more info on this API, see:
 *
 *  http://www.codesourcery.com/cxx-abi/abi.html#dso-dtor
 */
extern "C" int __cxa_atexit(void (*func)(void*), void* arg, void* dso) {
  struct atexit_fn entry = { func, arg, dso };
  _ATEXIT_LOCK();
  __libc_globals->atexit->mutate().push_back(entry);
  _ATEXIT_UNLOCK();
  return 0;
}

/*
 * Call all handlers registered with __cxa_atexit() for the shared object
 * owning 'dso'.
 * Note: if 'dso' is NULL, then all remaining handlers are called.
 */
extern "C" void __cxa_finalize(void* dso) {
  _ATEXIT_LOCK();
  auto vec = __libc_globals->atexit->mutate();
  for (size_t i = vec.size() - 1; i < SIZE_MAX; --i) {
  restart:
    if (!vec[i].fn_ptr) {
      continue; /* already called */
    }
    if (dso && vec[i].fn_dso != dso) {
      continue; /* wrong DSO */
    }

    /*
     * Mark handler as having been already called to avoid
     * dupes and loops, then call the appropriate function.
     */
    atexit_fn fn = vec[i];
    vec[i].fn_ptr = nullptr;

    size_t orig_size = vec.size();
    _ATEXIT_UNLOCK();
    (*fn.fn_ptr)(fn.fn_arg);
    _ATEXIT_LOCK();

    if (orig_size != vec.size()) {
      // The handler added additional atexit handlers, restart the loop.
      i = vec.size() - 1;
      goto restart;
    }
  }
  _ATEXIT_UNLOCK();

  if (dso) {
    __unregister_atfork(dso);
  }
}

/*
 * Register the cleanup function
 */
extern "C" void __atexit_register_cleanup(void (*func)(void)) {
  struct atexit_fn entry = { reinterpret_cast<void (*)(void*)>(func), nullptr,
                             nullptr };
  _ATEXIT_LOCK();
  __libc_globals->atexit->mutate().push_back(entry);
  _ATEXIT_UNLOCK();
}
