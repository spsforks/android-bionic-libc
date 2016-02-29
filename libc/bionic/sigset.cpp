/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <signal.h>
#include <string.h>

sighandler_t sigset(int sig, sighandler_t disp) {
  struct sigaction new_sa, *new_sa_ptr;
  int proc_mask_how;

  if (disp == SIG_HOLD) {
    new_sa_ptr = nullptr;
    proc_mask_how = SIG_BLOCK;
  } else {
    memset(&new_sa, 0, sizeof(new_sa));
    new_sa.sa_handler = disp;
    sigemptyset(&new_sa.sa_mask);

    new_sa_ptr = &new_sa;
    proc_mask_how = SIG_UNBLOCK;
  }

  struct sigaction old_sa;
  int err = sigaction(sig, new_sa_ptr, &old_sa);
  if (err)
    return SIG_ERR;

  sigset_t old_proc_mask, new_proc_mask;
  sigemptyset(&new_proc_mask);
  sigaddset(&new_proc_mask, sig);
  err = sigprocmask(proc_mask_how, &new_proc_mask, &old_proc_mask);
  if (err)
    return SIG_ERR;

  if (sigismember(&old_proc_mask, sig))
    return SIG_HOLD;
  return old_sa.sa_handler;
}
