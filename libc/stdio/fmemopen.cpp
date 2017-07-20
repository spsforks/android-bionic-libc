/*-
 * Copyright (C) 2013 Pietro Cerutti <gahr@FreeBSD.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "local.h"

struct fmemopen_cookie {
  char* buf;
  char* allocation;
  size_t capacity;
  size_t size;
  size_t offset;
  bool binary_mode;
};

static int fmemopen_read(void* cookie, char* buf, int nbytes) {
  fmemopen_cookie* ck = reinterpret_cast<fmemopen_cookie*>(cookie);

  if (static_cast<size_t>(nbytes) > ck->size - ck->offset) nbytes = ck->size - ck->offset;

  if (nbytes > 0) {
    memcpy(buf, ck->buf + ck->offset, nbytes);
    ck->offset += nbytes;
  }
  return nbytes;
}

static int fmemopen_write(void* cookie, const char* buf, int n) {
  fmemopen_cookie* ck = reinterpret_cast<fmemopen_cookie*>(cookie);

  // We don't need to add the trailing NUL in binary mode, nor if there's
  // already a trailing NUL in the data we're writing.
  size_t space_for_null = (!ck->binary_mode && n > 0 && buf[n - 1] != '\0') ? 1 : 0;

  // How much can we actually fit?
  if (static_cast<size_t>(n) + space_for_null > ck->capacity - ck->offset) {
    n = ck->capacity - ck->offset - space_for_null;
    // Give up if we don't even have room for one byte of userdata.
    if (n < 0) {
      errno = ENOSPC;
      return -1;
    }
  }

  if (n > 0) {
    memcpy(ck->buf + ck->offset, buf, n);
    ck->offset += n;
    // Is this the furthest we've ever been?
    if (ck->offset >= ck->size) {
      if (!ck->binary_mode && buf[n - 1] != '\0') ck->buf[ck->offset] = '\0';
      ck->size = ck->offset;
    }
  }
  return n;
}

static fpos_t fmemopen_seek(void* cookie, fpos_t offset, int whence) {
  fmemopen_cookie* ck = reinterpret_cast<fmemopen_cookie*>(cookie);

  if (whence == SEEK_SET && (offset >= 0 && static_cast<size_t>(offset) <= ck->capacity)) {
    return (ck->offset = offset);
  } else if (whence == SEEK_CUR && (ck->offset + offset <= ck->capacity)) {
    return (ck->offset += offset);
  } else if (whence == SEEK_END) {
    int end = (ck->binary_mode) ? ck->capacity : ck->size;
    if (offset <= 0 && -offset <= end) return (ck->offset = end + offset);
  }
  errno = EINVAL;
  return -1;
}

static int fmemopen_close(void* cookie) {
  fmemopen_cookie* ck = reinterpret_cast<fmemopen_cookie*>(cookie);
  free(ck->allocation);
  free(ck);
  return 0;
}

FILE* fmemopen(void* buf, size_t capacity, const char* mode) {
  if (capacity == 0) {
    errno = EINVAL;
    return nullptr;
  }

  int flags;
  if (__sflags(mode, &flags) == 0) {
    errno = EINVAL;
    return nullptr;
  }

  fmemopen_cookie* ck = reinterpret_cast<fmemopen_cookie*>(calloc(sizeof(fmemopen_cookie), 1));
  if (ck == nullptr) return nullptr;

  ck->offset = 0;
  ck->buf = reinterpret_cast<char*>(buf);
  ck->capacity = capacity;
  ck->binary_mode = strchr(mode, 'b') != nullptr;

  if (ck->buf == nullptr) ck->buf = ck->allocation = reinterpret_cast<char*>(calloc(capacity, 1));
  if (ck->buf == nullptr) {
    free(ck);
    return nullptr;
  }

  FILE* fp = funopen(ck,
                     (flags & O_WRONLY) ? nullptr : fmemopen_read,
                     (flags & O_RDONLY) ? nullptr : fmemopen_write,
                     fmemopen_seek,
                     fmemopen_close);
  if (fp == nullptr) {
    fmemopen_close(ck);
    return nullptr;
  }

  if (mode[0] == 'a') {
    ck->size = ck->offset = strnlen(ck->buf, ck->capacity);
    fp->_flags |= __SAPP;
  } else if (mode[0] == 'r') {
    ck->size = capacity;
  } else if (mode[0] == 'w') {
    ck->size = 0;
    ck->buf[0] = '\0';
  }

  return fp;
}
