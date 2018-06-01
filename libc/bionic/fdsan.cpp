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
#include <inttypes.h>
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
pid_t __get_cached_pid();

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

__printflike(1, 0) static void fdsan_error(const char* fmt, ...) {
  if (FdsanIsDisabled()) {
    return;
  }

  // Lots of code will (sensibly) fork, blindly call close on all of their fds,
  // and then exec. Compare our cached pid value against the real one to detect
  // this scenario and permit it.
  pid_t cached_pid = __get_cached_pid();
  if (cached_pid == 0 || cached_pid != syscall(__NR_getpid)) {
    return;
  }

  va_list va;
  va_start(va, fmt);
  async_safe_fatal_va_list("fdsan", fmt, va);
  va_end(va);

  abort();
}

uint64_t android_fdsan_create_owner_tag(android_fdsan_owner_type type, uint64_t tag) {
  if (tag == 0) {
    return 0;
  }

  if (__predict_false((type & 0xff) != type)) {
    async_safe_fatal("invalid android_fdsan_owner_type value: %x", type);
  }

  uint64_t result = static_cast<uint64_t>(type) << 56;
  uint64_t mask = (static_cast<uint64_t>(1) << 56) - 1;
  result |= tag & mask;
  return result;
}

int android_fdsan_close_with_tag(int fd, uint64_t expected_tag) {
  FdEntry* fde = GetFdEntry(fd);
  if (!fde) {
    return ___close(fd);
  }

  uint64_t tag = expected_tag;
  if (!atomic_compare_exchange_strong(&fde->close_tag, &tag, 0)) {
    fdsan_error("attempted to close file descriptor %d with incorrect tag: expected 0x%" PRIx64
                ", actual 0x%" PRIx64,
                fd, expected_tag, tag);
  }

  int rc = ___close(fd);
  // If we were expecting to close with a tag, abort on EBADF.
  if (expected_tag && rc == -1 && errno == EBADF) {
    fdsan_error("double-close of file descriptor %d detected", fd);
  }
  return rc;
}

static const char* __tag_to_type(uint64_t tag) {
  uint64_t type = tag >> 56;
  switch (type) {
    case ANDROID_FDSAN_OWNER_TYPE_FILE:
      return "FILE*";
    case ANDROID_FDSAN_OWNER_TYPE_DIR:
      return "DIR*";
    case ANDROID_FDSAN_OWNER_TYPE_UNIQUE_FD:
      return "unique_fd";
    case ANDROID_FDSAN_OWNER_TYPE_FILEINPUTSTREAM:
      return "FileInputStream";
    case ANDROID_FDSAN_OWNER_TYPE_FILEOUTPUTSTREAM:
      return "FileOutputStream";
    case ANDROID_FDSAN_OWNER_TYPE_RANDOMACCESSFILE:
      return "RandomAccessFile";
    case ANDROID_FDSAN_OWNER_TYPE_PARCELFILEDESCRIPTOR:
      return "ParcelFileDescriptor";

    case ANDROID_FDSAN_OWNER_TYPE_GENERIC:
    default:
      return "unknown type";
  }
}

static uint64_t __tag_to_owner(uint64_t tag) {
  return tag;
}

void android_fdsan_exchange_owner_tag(int fd, uint64_t expected_tag, uint64_t new_tag) {
  FdEntry* fde = GetFdEntry(fd);
  if (!fde) {
    return;
  }

  uint64_t tag = expected_tag;
  if (!atomic_compare_exchange_strong(&fde->close_tag, &tag, new_tag)) {
    if (expected_tag == 0) {
      fdsan_error(
          "failed to take ownership of already-owned file descriptor: fd %d is owned by %s "
          "%" PRIx64,
          fd, __tag_to_type(tag), __tag_to_owner(tag));
    } else {
      fdsan_error(
          "failed to exchange ownership of unowned file descriptor: expected fd %d to be owned "
          "by %s %" PRIx64,
          fd, __tag_to_type(expected_tag), __tag_to_owner(expected_tag));
    }
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
  int rc = android_fdsan_close_with_tag(fd, 0);
  if (rc == -1 && errno == EINTR) {
    return 0;
  }
  return rc;
}
