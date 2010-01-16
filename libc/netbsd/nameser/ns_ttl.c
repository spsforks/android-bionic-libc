/*	$NetBSD: ns_ttl.c,v 1.2 2004/05/20 20:35:05 christos Exp $	*/

/*
 * Copyright (c) 2004 by Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1996,1999 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/cdefs.h>
#ifndef lint
#ifdef notdef
static const char rcsid[] = "Id: ns_ttl.c,v 1.1.206.1 2004/03/09 08:33:45 marka Exp";
#else
__RCSID("$NetBSD: ns_ttl.c,v 1.2 2004/05/20 20:35:05 christos Exp $");
#endif
#endif

/* Import. */

#include "arpa_nameser.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

/* Public. */
int
ns_format_ttl(u_long src, char *dst, size_t dstlen) {
  char tmp[23];
  char *p = &tmp[0];

  int secs, mins, hours, days, weeks, len;

  secs = src % 60;   src /= 60;
  mins = src % 60;   src /= 60;
  hours = src % 24;  src /= 24;
  days = src % 7;    src /= 7;
  weeks = src;

  if (weeks) {
    if (weeks > 999) {
      /* Handle less reasonable values for week all the way to the u_long max of 2006136047 */
      int digitDivisor = 1000000000;
      int leadingZero = 1;
      int digit;
      do {
	digit = ((weeks / digitDivisor) % 10) + '0';
	digitDivisor /= 10;
	if (leadingZero && (digit != '0')) {
	  leadingZero = 0;
	}
	if (!leadingZero) {
	  *p++ = digit;
	}
      } while (digitDivisor != 0);
    } else {
      /* Handle more reasonable values for week quickly */
      if (weeks >= 100) {
	*p++ = ((weeks / 100) % 10) + '0';
      }
      if (weeks >= 10) {
	*p++ = ((weeks / 10) % 10) + '0';
      }
      *p++ = (weeks % 10) + '0';
    }
    *p++ = 'w';
  }
  if (days) {
    *p++ = days + '0';
    *p++ = 'd';
  }
  if (hours) {
    if (hours >= 10) {
      *p++ = (hours / 10) + '0';
    }
    *p++ = (hours % 10) + '0';
    *p++ = 'h';
  }
  if (mins) {
    if (mins >= 10) {
      *p++ = (mins / 10) + '0';
    }
    *p++ = (mins % 10) + '0';
    *p++ = 'm';
  }
  if (secs || !(weeks || days || hours || mins)) {
    if (secs >= 10) {
      *p++ = (secs / 10) + '0';
    }
    *p++ = (secs % 10) + '0';
    *p++ = 's';
  }

  *p = 0;
  len = p - &tmp[0];

  if ((len + 1) <= dstlen) {
    memcpy(dst, tmp, len + 1);
    return len;
  } else {
    return -1;
  }
}

#ifndef _LIBC
int
ns_parse_ttl(const char *src, u_long *dst) {
	u_long ttl, tmp;
	int ch, digits, dirty;

	ttl = 0;
	tmp = 0;
	digits = 0;
	dirty = 0;
	while ((ch = *src++) != '\0') {
		if (isdigit(ch)) {
			tmp = (tmp * 10) + (ch - '0');
			digits++;
			continue;
		}
		if (digits == 0)
			goto einval;
		switch (ch) {
		case 'w':
		case 'W':  tmp *= 7;	/*FALLTHROUGH*/
		case 'd':
		case 'D':  tmp *= 24;	/*FALLTHROUGH*/
		case 'h':
		case 'H':  tmp *= 60;	/*FALLTHROUGH*/
		case 'm':
		case 'M':  tmp *= 60;	/*FALLTHROUGH*/
		case 's':
		case 'S':  break;
		default:   goto einval;
		}
		ttl += tmp;
		tmp = 0;
		digits = 0;
		dirty = 1;
	}
	if (digits > 0) {
		if (dirty)
			goto einval;
		else
			ttl += tmp;
	}
	*dst = ttl;
	return (0);

 einval:
	errno = EINVAL;
	return (-1);
}
#endif
