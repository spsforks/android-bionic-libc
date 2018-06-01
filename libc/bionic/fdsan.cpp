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

#include <android/fdsan.h>

#include <errno.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <async_safe/log.h>

#include "private/bionic_globals.h"
#include "pthread_internal.h"

extern "C" int ___close(int fd);

static FdTable<128>* GetFdTable() {
  if (!__libc_shared_globals) {
    return nullptr;
  }

  return &__libc_shared_globals->fd_table;
}

static FdEntry* GetFdEntry(int fd) {
  if (fd < 0) {
    return nullptr;
  }

  auto* fd_table = GetFdTable();
  if (!fd_table) {
    return nullptr;
  }

  return fd_table->at(fd);
}

static bool FdsanIsDisabled() {
  auto* fd_table = GetFdTable();
  if (!fd_table) {
    return true;
  }

  return fd_table->disabled;
}

static pid_t get_cached_pid() {
  pthread_internal_t* self = __get_thread();
  if (__predict_true(self)) {
    pid_t cached_pid;
    if (__predict_true(self->get_cached_pid(&cached_pid))) {
      return cached_pid;
    }
  }
  return 0;
}

__printflike(1, 0) static void fdsan_error(const char* fmt, ...) {
  pid_t cached_pid = get_cached_pid();
  if (cached_pid == 0 || cached_pid != syscall(__NR_getpid) || FdsanIsDisabled()) {
    // We're vforked or cloned or something, probably preparing to exec.
    // Don't treat this as an error.
    return;
  }

  va_list va;
  va_start(va, fmt);
  async_safe_fatal_va_list("fdsan", fmt, va);
  va_end(va);

  abort();
}

int android_fdsan_close_with_tag(int fd, void* expected_tag) {
  FdEntry* fde = GetFdEntry(fd);
  if (!fde) {
    return ___close(fd);
  }

  void* tag = expected_tag;
  if (!atomic_compare_exchange_strong(&fde->close_tag, &tag, nullptr)) {
    fdsan_error("attempted to close file descriptor %d with incorrect tag: expected %p, actual %p",
                fd, expected_tag, tag);
  }

  int rc = ___close(fd);
  // If we were expecting to close with a tag, abort on EBADF.
  if (expected_tag && rc == -1 && errno == EBADF) {
    fdsan_error("double-close of file descriptor %d detected", fd);
  }
  return rc;
}

void android_fdsan_exchange_close_tag(int fd, void* expected_tag, void* new_tag) {
  FdEntry* fde = GetFdEntry(fd);
  if (!fde) {
    return;
  }

  void* tag = expected_tag;
  if (!atomic_compare_exchange_strong(&fde->close_tag, &tag, new_tag)) {
    fdsan_error("failed to exchange close tag for fd %d: expected %p, actual %p", fd, expected_tag,
                tag);
  }
}

bool android_fdsan_set_enabled(bool enabled) {
  auto* fd_table = GetFdTable();
  if (!fd_table) {
    async_safe_fatal("attempted to disable fdsan before libc initialization?");
  }

  bool previous = fd_table->disabled;
  fd_table->disabled = !enabled;
  return !previous;
}

int close(int fd) {
  int rc = android_fdsan_close_with_tag(fd, nullptr);
  if (rc == -1 && errno == EINTR) {
    return 0;
  }
  return rc;
}
