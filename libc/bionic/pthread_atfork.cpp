/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

#include "private/bionic_globals.h"
#include "private/bionic_macros.h"
#include "private/WPVector.h"

static pthread_mutex_t g_atfork_list_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

void __libc_init_atfork(libc_globals* globals) {
  globals->atfork = WPVector<atfork_t>::allocate("atfork handlers");
}

void __bionic_atfork_run_prepare() {
  // We lock the atfork list here, unlock it in the parent, and reset it in the child.
  // This ensures that nobody can modify the handler array between the calls
  // to the prepare and parent/child handlers.
  pthread_mutex_lock(&g_atfork_list_mutex);

  // Call pthread_atfork() prepare handlers. POSIX states that the prepare
  // handlers should be called in the reverse order of the parent/child
  // handlers, so we iterate backwards.
  const auto& atfork_list = *__libc_globals->atfork;
  for (size_t i = atfork_list.size() - 1; i != SIZE_MAX; --i) {
    if (atfork_list[i].prepare) {
      atfork_list[i].prepare();
    }
  }
}

void __bionic_atfork_run_child() {
  const auto& atfork_list = *__libc_globals->atfork;
  for (auto&& atfork : atfork_list) {
    if (atfork.child) {
      atfork.child();
    }
  }

  g_atfork_list_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
}

void __bionic_atfork_run_parent() {
  const auto& atfork_list = *__libc_globals->atfork;
  for (auto&& atfork : atfork_list) {
    if (atfork.parent) {
      atfork.parent();
    }
  }

  pthread_mutex_unlock(&g_atfork_list_mutex);
}

// __register_atfork is the name used by glibc
extern "C" int __register_atfork(void (*prepare)(void), void (*parent)(void),
                                 void(*child)(void), void* dso) {
  atfork_t entry;
  entry.prepare = prepare;
  entry.parent = parent;
  entry.child = child;
  entry.dso_handle = dso;

  pthread_mutex_lock(&g_atfork_list_mutex);
  __libc_globals->atfork->mutate().push_back(entry);
  pthread_mutex_unlock(&g_atfork_list_mutex);

  return 0;
}

extern "C" __LIBC_HIDDEN__ void __unregister_atfork(void* dso) {
  pthread_mutex_lock(&g_atfork_list_mutex);

  auto atfork_list = __libc_globals->atfork->mutate();
  for (atfork_t& entry : atfork_list) {
    if (entry.dso_handle == dso) {
      memset(&entry, 0, sizeof(entry));
    }
  }

  pthread_mutex_unlock(&g_atfork_list_mutex);
}

