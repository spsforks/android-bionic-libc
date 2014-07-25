/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "malloc_debug_common.h"

extern const MallocDebug* g_malloc_dispatch;

// =============================================================================
// Allocation functions
// =============================================================================
extern "C" void* calloc(size_t n_elements, size_t elem_size) {
  return g_malloc_dispatch->calloc(n_elements, elem_size);
}

extern "C" void free(void* mem) {
  g_malloc_dispatch->free(mem);
}

extern "C" struct mallinfo mallinfo() {
  return g_malloc_dispatch->mallinfo();
}

extern "C" void* malloc(size_t bytes) {
  return g_malloc_dispatch->malloc(bytes);
}

extern "C" size_t malloc_usable_size(const void* mem) {
  return g_malloc_dispatch->malloc_usable_size(mem);
}

extern "C" void* memalign(size_t alignment, size_t bytes) {
  return g_malloc_dispatch->memalign(alignment, bytes);
}

extern "C" int posix_memalign(void** memptr, size_t alignment, size_t size) {
  return g_malloc_dispatch->posix_memalign(memptr, alignment, size);
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
extern "C" void* pvalloc(size_t bytes) {
  return g_malloc_dispatch->pvalloc(bytes);
}
#endif

extern "C" void* realloc(void* oldMem, size_t bytes) {
  return g_malloc_dispatch->realloc(oldMem, bytes);
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
extern "C" void* valloc(size_t bytes) {
  return g_malloc_dispatch->valloc(bytes);
}
#endif

extern "C" char* strdup(const char* s) {
  size_t len = strlen(s) + 1;
  char* new_str = reinterpret_cast<char*>(g_malloc_dispatch->malloc(len));
  if (new_str == NULL) {
    return NULL;
  }
  return reinterpret_cast<char*>(memcpy(new_str, s, len));
}

// =============================================================================
// FILE functions
// =============================================================================
#define GET_FD(fp) (static_cast<int>(reinterpret_cast<intptr_t>(fp)) - 1)

extern "C" FILE* fopen(const char* path, const char* mode) {
  int flags = 0;
  if (!mode) {
    return NULL;
  }
  bool read = false, write = false;
  while (*mode++ != '\0') {
    if (tolower(*mode) == 'r') {
      read = true;
    } else if (tolower(*mode) == 'w') {
      write = true;
    }
  }
  if (read && write) {
    flags = O_RDWR;
  } else if (read) {
    flags = O_RDONLY;
  } else if (write) {
    flags = O_WRONLY;
  }

  int fd = open(path, flags);
  if (fd < 0) {
    return NULL;
  }

  return reinterpret_cast<FILE*>(static_cast<intptr_t>(fd+1));
}

extern "C" size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream) {
  int fd = GET_FD(stream);
  ssize_t bytes_read = read(fd, ptr, size * nmemb);
  if (bytes_read < 0) {
    bytes_read = 0;
  }
  return static_cast<size_t>(bytes_read/size);
}

extern "C" size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream) {
  int fd = GET_FD(stream);
  ssize_t bytes_written = write(fd, ptr, size * nmemb);
  if (bytes_written < 0) {
    bytes_written = 0;
  }
  return static_cast<size_t>(bytes_written/size);
}

extern "C" int fseek(FILE* stream, long offset, int whence) {
  int fd = GET_FD(stream);
  off_t ret = lseek(fd, offset, whence);
  if (ret == static_cast<off_t>(-1)) {
    return -1;
  }
  return 0;
}

extern "C" char* fgets(char* s, int size, FILE* stream) {
  if (size <= 0) {
    return NULL;
  }
  int fd = GET_FD(stream);
  volatile ssize_t bytes_read = read(fd, s, size - 1);
  if (bytes_read <= 0) {
    s[0] = '\0';
    return NULL;
  }
  char* newline = reinterpret_cast<char*>(memchr(s, '\n', bytes_read));
  if (newline == NULL) {
    s[bytes_read] = '\0';
  } else {
    ++*newline = '\0';
    lseek(fd, -(bytes_read - (newline - s + 1)), SEEK_CUR);
  }
  return s;
}

extern "C" int fclose(FILE* fp) {
  int fd = GET_FD(fp);
  return close(fd);
}
