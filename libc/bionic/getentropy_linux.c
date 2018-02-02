/*	$OpenBSD: getentropy_linux.c,v 1.42 2016/04/19 20:20:24 tj Exp $	*/

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
 * http://man.openbsd.org/getentropy.2
 */

#define	_POSIX_C_SOURCE	199309L
#define	_GNU_SOURCE	1
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <linux/random.h>

static int gotdata(char *buf, size_t len);
static int getentropy_getrandom(void *buf, size_t len);
static int getentropy_urandom(void *buf, size_t len);

int getentropy(void* buf, size_t len) {
  if (len > 256) {
    errno = EIO;
    return -1;
  }

  int ret = getentropy_getrandom(buf, len);
  if (ret != -1 || errno != ENOSYS) return ret;

  return getentropy_urandom(buf, len);
}

/*
 * Basic sanity checking; wish we could do better.
 */
static int
gotdata(char *buf, size_t len)
{
	char	any_set = 0;
	size_t	i;

	for (i = 0; i < len; ++i)
		any_set |= buf[i];
	if (any_set == 0)
		return (-1);
	return (0);
}

static int getentropy_getrandom(void* buf, size_t len) {
  int saved_errno = errno;

  // Use GRND_NONBLOCK to avoid blocking before the
  // entropy pool has been initialized
  int ret = TEMP_FAILURE_RETRY(syscall(SYS_getrandom, buf, len, GRND_NONBLOCK));
  if ((size_t)ret != len) return -1;
  errno = saved_errno;
  return 0;
}

static int getentropy_urandom(void* buf, size_t len) {
  int saved_errno = errno;

  int fd = TEMP_FAILURE_RETRY(open("/dev/urandom", O_RDONLY | O_NOFOLLOW | O_CLOEXEC, 0));
  if (fd == -1) return -1;

  // Lightly verify that the device node looks sane.
  struct stat st;
  int cnt;
  if (fstat(fd, &st) == -1 || !S_ISCHR(st.st_mode) || ioctl(fd, RNDGETENTCNT, &cnt) == -1) {
    close(fd);
    errno = EIO;
    return -1;
  }

  for (size_t i = 0; i < len; ) {
    size_t wanted = len - i;
    ssize_t ret = TEMP_FAILURE_RETRY(read(fd, (char *)buf + i, wanted));
    if (ret == -1) {
      close(fd);
      return -1;
    }
    i += ret;
  }
  close(fd);

  if (gotdata(buf, len) == 0) {
    errno = saved_errno;
    return 0;  // Satisfied.
  }

  errno = EIO;
  return -1;
}
