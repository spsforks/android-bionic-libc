/*	$OpenBSD: getentropy_linux.c,v 1.28 2014/07/20 03:24:10 deraadt Exp $	*/

/*
 * Copyright (c) 2014 Theo de Raadt <deraadt@openbsd.org>
 * Copyright (c) 2014 Bob Beck <beck@obtuse.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Emulation of getentropy(2) as documented at:
 * http://www.openbsd.org/cgi-bin/man.cgi/OpenBSD-current/man2/getentropy.2
 */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <linux/random.h>
#include <linux/sysctl.h>
#include <sys/vfs.h>

extern "C" int getrandom(void* buf, size_t len, unsigned int flags);

static int getentropy_getrandom(void* buf, size_t len);
static int getentropy_urandom(void* buf, size_t len);

extern "C" int getentropy(void* buf, size_t len) {
  int ret = -1;

  if (len > 256) {
    errno = EIO;
    return -1;
  }

  /*
   * Try descriptor-less getrandom()
   */
  ret = getentropy_getrandom(buf, len);
  if (ret != -1) {
    return ret;
  }

  /*
   * Try to get entropy with /dev/urandom
   *
   * This can fail if the process is inside a chroot or if file
   * descriptors are exhausted.
   */
  ret = getentropy_urandom(buf, len);
  if (ret != -1) {
    return ret;
  }

  /*
   * Entropy collection via /dev/urandom and sysctl have failed.
   *
   * No other API exists for collecting entropy.  See the large
   * comment block above.
   *
   * We have very few options:
   *     - Even syslog_r is unsafe to call at this low level, so
   *	 there is no way to alert the user or program.
   *     - Cannot call abort() because some systems have unsafe
   *	 corefiles.
   *     - Could raise(SIGKILL) resulting in silent program termination.
   *     - Return EIO, to hint that arc4random's stir function
   *       should raise(SIGKILL)
   *     - Do the best under the circumstances....
   *
   * This code path exists to bring light to the issue that Linux
   * does not provide a failsafe API for entropy collection.
   *
   * We hope this demonstrates that Linux should either retain their
   * sysctl ABI, or consider providing a new failsafe API which
   * works in a chroot or when file descriptors are exhausted.
   */
  raise(SIGKILL);
  __builtin_trap();
}

/*
 * Basic sanity checking; wish we could do better.
 */
static bool gotdata(const void* buf, size_t len) {
  char any_set = 0;
  size_t i;

  for (i = 0; i < len; ++i) {
    any_set |= static_cast<const char*>(buf)[i];
  }

  return any_set;
}

static int getentropy_getrandom(void* buf, size_t len) {
  int ret = -1;
  if (len > 256) {
    return -1;
  }

  ret = getrandom(buf, len, 0);
  if (ret == static_cast<int>(len)) {
    return 0;
  }
  return -1;
}

static int getentropy_urandom(void* buf, size_t len) {
  struct stat st;
  size_t i;
  int fd, cnt, flags;
  int save_errno = errno;

start:
  flags = O_RDONLY | O_NOFOLLOW | O_CLOEXEC;
  fd = open("/dev/urandom", flags, 0);
  if (fd == -1) {
    if (errno == EINTR) goto start;
    goto nodevrandom;
  }

  /* Lightly verify that the device node looks sane */
  if (fstat(fd, &st) == -1 || !S_ISCHR(st.st_mode)) {
    close(fd);
    goto nodevrandom;
  }
  if (ioctl(fd, RNDGETENTCNT, &cnt) == -1) {
    close(fd);
    goto nodevrandom;
  }
  for (i = 0; i < len;) {
    size_t wanted = len - i;
    ssize_t ret = TEMP_FAILURE_RETRY(read(fd, static_cast<char*>(buf) + i, wanted));

    if (ret == -1) {
      if (errno == EAGAIN) {
        continue;
      }
      close(fd);
      goto nodevrandom;
    }
    i += ret;
  }
  close(fd);
  if (gotdata(buf, len)) {
    errno = save_errno;
    return 0; /* satisfied */
  }
nodevrandom:
  errno = EIO;
  return -1;
}
