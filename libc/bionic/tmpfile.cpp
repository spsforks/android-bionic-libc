/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

FILE* tmpfile() {
  // TODO: get this app's temporary directory from the framework ("/data/data/app/cache").
  const char* tmp_dir = NULL;
  // Fall back to $EXTERNAL_STORAGE...
  if (tmp_dir == NULL) {
    tmp_dir = getenv("EXTERNAL_STORAGE");
  }
  // Fall back to /data...
  if (tmp_dir == NULL) {
    tmp_dir = "/data";
  }

  char buf[PATH_MAX];
  snprintf(buf, sizeof(buf), "%s/tmp.XXXXXXXXXX", tmp_dir);

  sigset_t set, old_set;
  sigfillset(&set);
  sigprocmask(SIG_BLOCK, &set, &old_set);

  int fd = mkstemp(buf);
  if (fd != -1) {
    // Unlink the file now so that it's removed when closed.
    unlink(buf);
  }

  sigprocmask(SIG_SETMASK, &old_set, NULL);

  if (fd == -1) {
    return NULL;
  }

  FILE* fp = fdopen(fd, "w+");
  if (fp != NULL) {
    // Success.
    return fp;
  }

  // Failure. Clean up. We already unlinked, so we just need to close.
  int old_errno = errno;
  close(fd);
  errno = old_errno;
  return NULL;
}
