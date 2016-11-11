/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <assert.h>
#include <signal.h>
#include "bionic/pthread_internal.h"
#include "private/ErrnoRestorer.h"

extern "C" void __restore_rt(void);
extern "C" void __restore(void);

#if defined(__LP64__)

#ifdef BIONIC_SAFESTACK
typedef void (*sigaction_t) (int, struct siginfo*, void*);
static atomic_uintptr_t real_sighandlers[NSIG];

__attribute__((no_sanitize("safe-stack")))
static void sigaction_wrapper(int signum, struct siginfo* si, void* uc) {
  assert(signum >= 0 && signum < NSIG);
  void *altstack_start;
  size_t altstack_size;
  __get_thread()->unsafe_altstack_get(&altstack_start, &altstack_size);
  void*& unsafe_stack_ptr = __get_tls()[TLS_SLOT_SAFESTACK];
  bool switch_stacks = false;
  if (altstack_start && altstack_size) {
    uintptr_t left = reinterpret_cast<uintptr_t>(altstack_start);
    uintptr_t right = left + altstack_size;
    uintptr_t cur = reinterpret_cast<uintptr_t>(unsafe_stack_ptr);
    switch_stacks = (cur < left) || (cur >= right);
  }
  sigaction_t real_sigaction = reinterpret_cast<sigaction_t>(atomic_load(&real_sighandlers[signum]));
  if (switch_stacks) {
    void* saved_unsafe_stack_ptr = unsafe_stack_ptr;
    unsafe_stack_ptr =
        reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(altstack_start) + altstack_size);
    real_sigaction(signum, si, uc);
    unsafe_stack_ptr = saved_unsafe_stack_ptr;
  } else {
    real_sigaction(signum, si, uc);
  }
}

static bool wrap_signal(int signal, const struct sigaction* bionic_new_action) {
  if (!bionic_new_action) return false;
  sighandler_t new_handler = bionic_new_action->sa_handler;
  if (new_handler == SIG_DFL || new_handler == SIG_IGN)
    return false;
  if (signal >= NSIG)
    return false;
  if ((bionic_new_action->sa_flags & SA_ONSTACK) == 0)
    return false;
  return true;
}
#endif // BIONIC_SAFESTACK

extern "C" int __rt_sigaction(int, const struct __kernel_sigaction*, struct __kernel_sigaction*, size_t);

static void bionic_action_to_kernel_action(const struct sigaction* bionic_action,
                                           __kernel_sigaction* kernel_action) {
  kernel_action->sa_flags = bionic_action->sa_flags;
  kernel_action->sa_handler = bionic_action->sa_handler;
  kernel_action->sa_mask = bionic_action->sa_mask;
#if defined(SA_RESTORER)
  kernel_action->sa_restorer = bionic_action->sa_restorer;
#if defined(__aarch64__)
// arm64 has sa_restorer, but unwinding works best if you just let the
// kernel supply the default restorer from [vdso]. gdb doesn't care, but
// libgcc needs the nop that the kernel includes before the actual code.
// (We could add that ourselves, but why bother?)
#else
  if (!(kernel_action->sa_flags & SA_RESTORER)) {
    kernel_action->sa_flags |= SA_RESTORER;
    kernel_action->sa_restorer = &__restore_rt;
  }
#endif
#endif
}

static void kernel_action_to_bionic_action(const __kernel_sigaction* kernel_action,
                                           struct sigaction* bionic_action) {
  bionic_action->sa_flags = kernel_action->sa_flags;
  bionic_action->sa_handler = kernel_action->sa_handler;
  bionic_action->sa_mask = kernel_action->sa_mask;
#if defined(SA_RESTORER)
  bionic_action->sa_restorer = kernel_action->sa_restorer;
#endif
}

int sigaction(int signal, const struct sigaction* bionic_new_action,
              struct sigaction* bionic_old_action) {
  __kernel_sigaction kernel_new_action;
  if (bionic_new_action != nullptr) {
    bionic_action_to_kernel_action(bionic_new_action, &kernel_new_action);
  }

#ifdef BIONIC_SAFESTACK
  sighandler_t new_handler;
  sighandler_t wrapper = reinterpret_cast<sighandler_t>(sigaction_wrapper);
  bool wrap = wrap_signal(signal, bionic_new_action);
  sigset_t newmask, oldmask;
  if (wrap) {
    new_handler = kernel_new_action.sa_handler;
    kernel_new_action.sa_handler = wrapper;
    // Wrapper setup can not be done atomically. Block all signals in the current thread until it's
    // done.
    sigfillset(&newmask);
    sigprocmask(SIG_SETMASK, &newmask, &oldmask);
  }
#endif

  __kernel_sigaction kernel_old_action;
  int result = __rt_sigaction(signal, (bionic_new_action != nullptr) ? &kernel_new_action : nullptr,
                              (bionic_old_action != nullptr) ? &kernel_old_action : nullptr,
                              sizeof(sigset_t));

  if (bionic_old_action != nullptr) {
    kernel_action_to_bionic_action(&kernel_old_action, bionic_old_action);
  }

#ifdef BIONIC_SAFESTACK
  if (bionic_old_action != nullptr && bionic_old_action->sa_handler == wrapper) {
    bionic_old_action->sa_handler =
        reinterpret_cast<sighandler_t>(atomic_load(&real_sighandlers[signal]));
  }

  if (result == 0 && wrap) {
    ErrnoRestorer errno_restorer;
    atomic_store(&real_sighandlers[signal], reinterpret_cast<uintptr_t>(new_handler));
    sigprocmask(SIG_SETMASK, &oldmask, nullptr);
  }
#endif

  return result;
}

#else  // __LP64__

extern "C" int __sigaction(int, const struct sigaction*, struct sigaction*);

int sigaction(int signal, const struct sigaction* bionic_new_action, struct sigaction* bionic_old_action) {
  // The 32-bit ABI is broken. struct sigaction includes a too-small sigset_t,
  // so we have to use sigaction(2) rather than rt_sigaction(2).
  struct sigaction kernel_new_action;
  if (bionic_new_action != nullptr) {
    kernel_new_action.sa_flags = bionic_new_action->sa_flags;
    kernel_new_action.sa_handler = bionic_new_action->sa_handler;
    kernel_new_action.sa_mask = bionic_new_action->sa_mask;
#if defined(SA_RESTORER)
    kernel_new_action.sa_restorer = bionic_new_action->sa_restorer;

    if (!(kernel_new_action.sa_flags & SA_RESTORER)) {
      kernel_new_action.sa_flags |= SA_RESTORER;
      kernel_new_action.sa_restorer = (kernel_new_action.sa_flags & SA_SIGINFO) ? &__restore_rt : &__restore;
    }
#endif
  }
  return __sigaction(signal, (bionic_new_action != nullptr) ? &kernel_new_action : nullptr, bionic_old_action);
}

#endif
