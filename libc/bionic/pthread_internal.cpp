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

#include "pthread_internal.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <async_safe/log.h>

#include "private/ScopedRWLock.h"
#include "private/bionic_futex.h"
#include "private/bionic_tls.h"

static pthread_internal_t* g_thread_list = nullptr;
static pthread_rwlock_t g_thread_list_lock = PTHREAD_RWLOCK_INITIALIZER;

#if defined(__i386__) || defined(__i686__)

__LIBC_HIDDEN__ _Atomic(int) __libc_keep_tcb_map = 0;

struct tcb_map_t {
  tcb_map_t* next;
  tcb_map_t* prev;

  bionic_tcb* tcb;
  pid_t tid;
};

// FIXME: Maybe make an actual map for faster lookup.
static tcb_map_t* tcb_map = nullptr;
static pthread_rwlock_t g_thread_tcb_map_lock = PTHREAD_RWLOCK_INITIALIZER;

static tcb_map_t* __lookup_tcb_map(pid_t tid) {
  tcb_map_t* cur = tcb_map;
  while (cur != nullptr) {
    if (cur->tid == tid) return cur;
    cur = cur->next;
  }
  return nullptr;
}

void __pthread_internal_add_tcb_mapping(bionic_tcb* tcb, pid_t tid) {
  if (!atomic_load(&__libc_keep_tcb_map)) return;

  void* allocation =
      mmap(nullptr, sizeof(tcb_map_t), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (allocation == MAP_FAILED) {
    async_safe_fatal("Failed to allocate memory for tcb_map_t: error %d", errno);
  }
  ScopedWriteLock locker(&g_thread_tcb_map_lock);
  tcb_map_t* new_node = static_cast<tcb_map_t*>(allocation);

  new_node->tcb = tcb;
  new_node->tid = tid;
  new_node->next = tcb_map;
  new_node->prev = nullptr;
  if (new_node->next != nullptr) {
    new_node->next->prev = new_node;
  }
  tcb_map = new_node;
}

bionic_tcb* __pthread_internal_find_tcb(pid_t tid) {
  if (!atomic_load(&__libc_keep_tcb_map)) return nullptr;

  ScopedReadLock locker(&g_thread_tcb_map_lock);
  tcb_map_t* node = __lookup_tcb_map(tid);
  if (node == nullptr) return nullptr;
  return node->tcb;
}

void __pthread_internal_remove_tcb_mapping(pid_t tid) {
  if (!atomic_load(&__libc_keep_tcb_map)) return;

  ScopedWriteLock locker(&g_thread_tcb_map_lock);
  tcb_map_t* node = __lookup_tcb_map(tid);
  if (node == nullptr) return;

  if (node->next != nullptr) {
    node->next->prev = node->prev;
  }
  if (node->prev != nullptr) {
    node->prev->next = node->next;
  } else {
    tcb_map = node->next;
  }

  munmap(node, sizeof(tcb_map_t));
};

#endif

pthread_t __pthread_internal_add(pthread_internal_t* thread) {
  ScopedWriteLock locker(&g_thread_list_lock);

  // We insert at the head.
  thread->next = g_thread_list;
  thread->prev = nullptr;
  if (thread->next != nullptr) {
    thread->next->prev = thread;
  }
  g_thread_list = thread;
  return reinterpret_cast<pthread_t>(thread);
}

void __pthread_internal_remove(pthread_internal_t* thread) {
  ScopedWriteLock locker(&g_thread_list_lock);

  if (thread->next != nullptr) {
    thread->next->prev = thread->prev;
  }
  if (thread->prev != nullptr) {
    thread->prev->next = thread->next;
  } else {
    g_thread_list = thread->next;
  }
}

static void __pthread_internal_free(pthread_internal_t* thread) {
  if (thread->mmap_size != 0) {
    // Free mapped space, including thread stack and pthread_internal_t.
    munmap(thread->mmap_base, thread->mmap_size);
  }
}

void __pthread_internal_remove_and_free(pthread_internal_t* thread) {
  __pthread_internal_remove(thread);
  __pthread_internal_free(thread);
}

pid_t __pthread_internal_gettid(pthread_t thread_id, const char* caller) {
  pthread_internal_t* thread = __pthread_internal_find(thread_id, caller);
  return thread ? thread->tid : -1;
}

pthread_internal_t* __pthread_internal_find(pthread_t thread_id, const char* caller) {
  pthread_internal_t* thread = reinterpret_cast<pthread_internal_t*>(thread_id);

  // Check if we're looking for ourselves before acquiring the lock.
  if (thread == __get_thread()) return thread;

  {
    // Make sure to release the lock before the abort below. Otherwise,
    // some apps might deadlock in their own crash handlers (see b/6565627).
    ScopedReadLock locker(&g_thread_list_lock);
    for (pthread_internal_t* t = g_thread_list; t != nullptr; t = t->next) {
      if (t == thread) return thread;
    }
  }

  // Historically we'd return null, but from API level 26 we catch this error.
  if (android_get_application_target_sdk_version() >= 26) {
    if (thread == nullptr) {
      // This seems to be a common mistake, and it's relatively harmless because
      // there will never be a valid thread at address 0, whereas other invalid
      // addresses might sometimes contain threads or things that look enough like
      // threads for us to do some real damage by continuing.
      // TODO: try getting rid of this when Treble lets us keep vendor blobs on an old API level.
      async_safe_format_log(ANDROID_LOG_WARN, "libc", "invalid pthread_t (0) passed to %s", caller);
    } else {
      async_safe_fatal("invalid pthread_t %p passed to %s", thread, caller);
    }
  }
  return nullptr;
}
