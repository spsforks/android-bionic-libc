/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include "memory_mitigation_state.h"

#include <dirent.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/types.h>

#include <bionic/mte.h>
#include <bionic/reserved_signals.h>

#include "private/ScopedRWLock.h"
#include "pthread_internal.h"

extern "C" void scudo_malloc_set_zero_contents(int zero_contents);
extern "C" void scudo_malloc_disable_memory_tagging();

static bool run_on_all_threads(bool (*func)()) {
  ScopedWriteLock creation_locker(&g_thread_creation_lock);
  ScopedReadLock list_locker(&g_thread_list_lock);

  if (!func()) {
    return false;
  }

  static sem_t g_sem;
  if (sem_init(&g_sem, 0, 0) != 0) {
    return false;
  }

  static bool (*g_func)();
  g_func = func;

  static _Atomic(bool) g_retval;
  atomic_init(&g_retval, true);

  auto handler = [](int, siginfo_t*, void*) {
    int old_errno = errno;
    if (!g_func()) {
      atomic_store(&g_retval, false);
    }
    sem_post(&g_sem);
    errno = old_errno;
  };

  struct sigaction act = {}, oldact;
  act.sa_flags = SA_SIGINFO;
  act.sa_sigaction = handler;
  if (sigaction(BIONIC_SIGNAL_RUN_ON_ALL_THREADS, &act, &oldact) != 0) {
    sem_destroy(&g_sem);
    return false;
  }

  pid_t my_pid = getpid();
  size_t num_tids = 0;
  for (pthread_internal_t* t = g_thread_list; t != nullptr; t = t->next) {
    if (t == __get_thread()) continue;
    if (tgkill(my_pid, t->tid, BIONIC_SIGNAL_RUN_ON_ALL_THREADS) == 0) {
      ++num_tids;
    } else if (errno != EINVAL) {
      // tgkill may race against normal thread termination, and EINVAL (invalid thread ID) is
      // expected if a thread termination won. Any other errno is unexpected and results in
      // failure.
      atomic_store(&g_retval, false);
    }
  }

  for (size_t i = 0; i != num_tids; ++i) {
    while (sem_wait(&g_sem) != 0) {
      if (errno != EINTR) {
        atomic_store(&g_retval, false);
        break;
      }
    }
  }

  sigaction(BIONIC_SIGNAL_RUN_ON_ALL_THREADS, &oldact, 0);
  sem_destroy(&g_sem);
  return atomic_load(&g_retval);
}

static bool set_tcf_on_all_threads(int tcf) {
  static int g_tcf;
  g_tcf = tcf;

  return run_on_all_threads([]() {
#ifdef ANDROID_EXPERIMENTAL_MTE
    int tagged_addr_ctrl = prctl(PR_GET_TAGGED_ADDR_CTRL, 0, 0, 0, 0);
    if (tagged_addr_ctrl < 0) {
      return false;
    }

    tagged_addr_ctrl = (tagged_addr_ctrl & ~PR_MTE_TCF_MASK) | g_tcf;
    if (prctl(PR_SET_TAGGED_ADDR_CTRL, tagged_addr_ctrl, 0, 0, 0) < 0) {
      return false;
    }
#endif
    return true;
  });
}

bool DisableMemoryMitigations(void* arg, size_t arg_size) {
  if (arg || arg_size) {
    return false;
  }

#ifdef USE_SCUDO
  scudo_malloc_set_zero_contents(0);

#ifdef ANDROID_EXPERIMENTAL_MTE
  if (mte_supported() && set_tcf_on_all_threads(PR_MTE_TCF_NONE)) {
    scudo_malloc_disable_memory_tagging();
  }
#endif
#endif

  return true;
}
