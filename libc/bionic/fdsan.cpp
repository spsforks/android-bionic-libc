/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <android/fdsan.h>

#include <errno.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <unistd.h>

#include <async_safe/log.h>

#include "private/bionic_globals.h"

extern "C" int ___close(int fd);

struct FdEntry {
  _Atomic(void*) close_tag;
};

struct FdTable {
  int table_length;
  FdEntry table[0];

  static FdTable* Allocate(int nfds) {
    size_t required = sizeof(FdTable) + nfds * sizeof(FdEntry);

    void* p = mmap(nullptr, required, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED) {
      async_safe_fatal("fdsan: failed to allocate FdTable");
    }

    FdTable* table = static_cast<FdTable*>(p);
    table->table_length = nfds;
    return table;
  }
};

// TODO: Stick this into bionic_shared_globals, once that exists.
static FdTable* fd_table;
static bool fdsan_enabled;

static FdEntry* GetFdEntry(int fd) {
  if (fd < 0) {
    return nullptr;
  }

  if (!fd_table) {
    return nullptr;
  }

  if (fd > fd_table->table_length) {
    return nullptr;
  }

  return &fd_table->table[fd];
}

__printflike(1, 0) static void fdsan_error(const char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  async_safe_fatal_va_list("fdsan", fmt, va);
  va_end(va);

  abort();
}

void __libc_init_fdsan() {
  // TODO: Get this value from a system property?
  fd_table = FdTable::Allocate(16384);
  fdsan_enabled = true;
}

int __libc_close_with_tag(int fd, void* expected_tag) {
  if (!fdsan_enabled) {
    return ___close(fd);
  }

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

void __libc_exchange_close_tag(int fd, void* expected_tag, void* new_tag) {
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

bool __libc_set_fdsan_enabled(bool enabled) {
  bool previous = fdsan_enabled;
  fdsan_enabled = enabled;
  return previous;
}

int close(int fd) {
  int rc = __libc_close_with_tag(fd, nullptr);
  if (rc == -1 && errno == EINTR) {
    return 0;
  }
  return rc;
}
