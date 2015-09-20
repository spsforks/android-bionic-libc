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

#define lockf64 __renamed_lockf64

#include <errno.h>
#include <stdio.h>

#include <fcntl.h>

#undef  lockf64

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "local.h"

/* lockf is a simplified interface to fcntl's locking facilities.  */

int lockf (int fd, int cmd, off_t len)
{
  struct flock fl;

  memset (static_cast<void *>(&fl), '\0', sizeof (fl));

  /* lockf is always relative to the current file position.  */
  fl.l_whence = SEEK_CUR;
  fl.l_start = 0;
  fl.l_len = len;

  switch (cmd)
    {
    case F_TEST:
      /* Test the lock: return 0 if FD is unlocked or locked by this process;
         return -1, set errno to EACCES, if another process holds the lock.  */
      fl.l_type = F_RDLCK;
      if (fcntl (fd, F_GETLK, &fl) < 0)
        return -1;
      if (fl.l_type == F_UNLCK || fl.l_pid == getpid ())
        return 0;
      errno = EACCES;
        return -1;

    case F_ULOCK:
      fl.l_type = F_UNLCK;
      cmd = F_SETLK;
      break;
    case F_LOCK:
      fl.l_type = F_WRLCK;
      cmd = F_SETLKW;
      break;
    case F_TLOCK:
      fl.l_type = F_WRLCK;
      cmd = F_SETLK;
      break;

    default:
      errno = EINVAL;
      return -1;
    }

  /* lockf() is a cancellation point but so is fcntl() if F_SETLKW is
     used.  Therefore we don't have to care about cancellation here,
     the fcntl() function will take care of it.  */
  return fcntl (fd, cmd, &fl);
}

__strong_alias (lockf64, lockf);
