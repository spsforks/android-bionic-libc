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

#include <android/mem_init.h>
#include <dirent.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <sys/types.h>

#include <bionic/mte.h>
#include <bionic/reserved_signals.h>

#include "pthread_internal.h"

extern "C" void scudo_malloc_set_zero_contents(int zero_contents);
extern "C" void scudo_malloc_disable_memory_tagging();

static bool run_on_all_threads(bool (*func)()) {
  static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_lock(&g_mutex);

  if (!func()) {
    pthread_mutex_unlock(&g_mutex);
    return false;
  }

  static sem_t g_sem;
  if (sem_init(&g_sem, 0, 0) != 0) {
    pthread_mutex_unlock(&g_mutex);
    return false;
  }

  DIR *dir = opendir("/proc/self/task");
  if (!dir) {
    sem_destroy(&g_sem);
    pthread_mutex_unlock(&g_mutex);
    return false;
  }

  static bool (*g_func)();
  g_func = func;

  static _Atomic(bool) g_retval;
  atomic_init(&g_retval, true);

  auto handler = [](int, siginfo_t *, void *) {
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
    closedir(dir);
    sem_destroy(&g_sem);
    pthread_mutex_unlock(&g_mutex);
    return false;
  }

  pid_t my_pid = getpid();
  pid_t my_tid = gettid();
  size_t num_tids = 0;
  while (dirent *ent = readdir(dir)) {
    char *endptr;
    pid_t tid = strtol(ent->d_name, &endptr, 10);
    if (*endptr == 0 && tid != my_tid) {
      if (tgkill(my_pid, tid, BIONIC_SIGNAL_RUN_ON_ALL_THREADS) == 0) {
        ++num_tids;
      } else if (errno != EINVAL) {
        // tgkill may race against normal thread termination, and EINVAL (invalid thread ID) is
        // expected if a thread termination won. Any other errno is unexpected and results in
        // failure.
        atomic_store(&g_retval, false);
      }
    }
  }
  closedir(dir);

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
  pthread_mutex_unlock(&g_mutex);
  return atomic_load(&g_retval);
}

static bool set_tcf_on_all_threads(int tcf) {
  // Arrange for pthread_create to issue the prctl on newly-created threads, to
  // avoid a race between reading /proc/self/task and pthread_create.
  atomic_store(&g_thread_tcf, tcf);
  atomic_store(&g_thread_tcf_pending, true);

  bool retval = run_on_all_threads(sync_thread_tcf);
  atomic_store(&g_thread_tcf_pending, false);
  return retval;
}

void android_disable_mem_init() {
#ifdef USE_SCUDO
  if (!mte_supported()) {
    scudo_malloc_set_zero_contents(0);
    return;
  }

  if (set_tcf_on_all_threads(PR_MTE_TCF_NONE)) {
    scudo_malloc_disable_memory_tagging();
  }
#endif
}
