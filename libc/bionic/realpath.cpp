/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <limits.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "private/ErrnoRestorer.h"

/*
 * Find the real name of path, by removing all ".", ".." and symlink
 * components.  Returns (resolved) on success, or (NULL) on failure,
 * in which case (resolved) is set to the empty string.
 */
char *
realpath(const char * __restrict path, char * __restrict resolved)
{
  ssize_t res;
  char* allocated = NULL;
  size_t copy_amount = PATH_MAX;
  char buf[40];
  int fd = -1;

  if (path == NULL) {
    errno = EINVAL;
    goto error;
  }

  fd = openat(AT_FDCWD, path, O_PATH | O_CLOEXEC);
  if (fd == -1) {
    goto error;
  }

  snprintf(buf, sizeof(buf), "/proc/self/fd/%d", fd);

  if (resolved == NULL) {
    struct stat sb;
    if (lstat(buf, &sb) == -1) {
      goto error;
    }

    allocated = reinterpret_cast<char*>(malloc(sb.st_size + 1));
    if (allocated == NULL) {
      errno = ENOMEM;
      goto error;
    }
    resolved = allocated;
    copy_amount = sb.st_size + 1;
  }

  res = readlink(buf, resolved, copy_amount - 1);
  if (res == -1) {
    goto error;
  }
  resolved[res] = '\0';
  goto ret;

error:
  // On error, "the contents of the array resolved_path are undefined"
  // We zero it out for safety.
  if (resolved != NULL) {
    resolved[0] = '\0';
    resolved = NULL;
  }
  free(allocated);

ret:
  ErrnoRestorer errno_restorer;
  if (fd != -1) {
    close(fd);
  }
  return resolved;
}
