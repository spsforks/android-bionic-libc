/*	$OpenBSD: citrus_utf8.c,v 1.6 2012/12/05 23:19:59 deraadt Exp $ */

/*-
 * Copyright (c) 2002-2004 Tim J. Robbins
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <errno.h>
#include <string.h>
#include <sys/param.h>
#include <wchar.h>

//
// This file is basically OpenBSD's citrus_utf8.c but rewritten to not require a
// 12-byte mbstate_t so we're backwards-compatible with our LP32 ABI where
// mbstate_t was only 4 bytes.
//
// The state is the UTF-8 sequence. We only support <= 4-bytes sequences so LP32
// mbstate_t already offer enough space (out of the 4 available bytes we only
// need 3 since we should never get to store the entire sequence in the
// intermediary state).
//
// The C standard leaves the conversion state undefined after a bad conversion.
// To avoid unexpected failures due to the possible use of the internal private
// state we always reset the conversion state when encountering illegal
// sequences.
//
// We also implement the POSIX interface directly rather than being accessed via
// function pointers.
//

// This is the private state used if PS is NULL.
static mbstate_t __state;

#define ERR_ILLEGAL_SEQUENCE static_cast<size_t>(-1)
#define ERR_INCOMPLETE_SEQUENCE static_cast<size_t>(-2)

int mbsinit(const mbstate_t* ps) {
  return (ps == NULL || ps->__seq == 0);
}

static size_t reset_and_return_illegal(int _errno, mbstate_t* ps) {
  errno = _errno;
  ps->__seq = 0;
  return ERR_ILLEGAL_SEQUENCE;
}

size_t mbrtowc(wchar_t* pwc, const char* s, size_t n, mbstate_t* ps) {
  mbstate_t* state = (ps == NULL) ? &__state : ps;

  // We should never get to a sate which has all 4 bytes of the sequence set.
  // Full state verification is done when decoding the sequence (after we have
  // all the bytes).
  if ((state->__seq >> 24) != 0) {
    return reset_and_return_illegal(EINVAL, ps);
  }

  if (s == NULL) {
    s = "";
    n = 1;
    pwc = NULL;
  }

  if (n == 0) {
    return 0;
  }

  int ch;
  if (((ch = static_cast<uint8_t>(*s)) & ~0x7f) == 0) {
    // Fast path for plain ASCII characters.
    if (pwc != NULL) {
      *pwc = ch;
    }
    return (ch != '\0' ? 1 : 0);
  }

  // Determine the number of octets that make up this character
  // from the first octet, and a mask that extracts the
  // interesting bits of the first octet. We already know
  // the character is at least two bytes long.
  size_t length;
  int mask;

  // We also specify a lower bound for the character code to
  // detect redundant, non-"shortest form" encodings. For
  // example, the sequence C0 80 is _not_ a legal representation
  // of the null character. This enforces a 1-to-1 mapping
  // between character codes and their multibyte representations.
  wchar_t lower_bound;

  size_t state_length =
    ((state->__seq >> 16) != 0) ? 3 :
    ((state->__seq >> 8) != 0) ? 2 :
    (state->__seq != 0) ? 1 : 0;

  // The first byte in the state (if any) tells the length
  ch = (state_length == 0)
    ? static_cast<uint8_t>(*s)
    : state->__seq >> (8 * (state_length - 1));
  if ((ch & 0x80) == 0) {
    mask = 0x7f;
    length = 1;
    lower_bound = 0;
  } else if ((ch & 0xe0) == 0xc0) {
    mask = 0x1f;
    length = 2;
    lower_bound = 0x80;
  } else if ((ch & 0xf0) == 0xe0) {
    mask = 0x0f;
    length = 3;
    lower_bound = 0x800;
  } else if ((ch & 0xf8) == 0xf0) {
    mask = 0x07;
    length = 4;
    lower_bound = 0x10000;
  } else {
    // Malformed input; input is not UTF-8. See RFC 3629.
    return reset_and_return_illegal(EILSEQ, state);
  }

  // Fill in the state.
  size_t i;
  for (i = 0; i < MIN(length - state_length, n); i++) {
    if ((state->__seq != 0) && ((*s & 0xc0) != 0x80)) {
      // Malformed input; bad characters in the middle of a character.
      errno = EILSEQ;
      return ERR_ILLEGAL_SEQUENCE;
    }
    state->__seq <<= 8;
    state->__seq |= static_cast<uint8_t>(*s++);
  }
  if (length - state_length > n) {
    return ERR_INCOMPLETE_SEQUENCE;
  }

  // Decode the octet sequence representing the character in chunks
  // of 6 bits, most significant first.
  wchar_t wch = static_cast<uint8_t>(state->__seq >> 8 * (length - 1)) & mask;
  for (i = 1; i < length; i++) {
    wch <<= 6;
    wch |= (state->__seq >> 8 * (length - i - 1)) & 0x3f;
  }

  if (wch < lower_bound) {
    // Malformed input; redundant encoding.
    return reset_and_return_illegal(EILSEQ, state);
  }
  if ((wch >= 0xd800 && wch <= 0xdfff) || wch == 0xfffe || wch == 0xffff) {
    // Malformed input; invalid code points.
    return reset_and_return_illegal(EILSEQ, state);
  }
  if (pwc != NULL) {
    *pwc = wch;
  }
  state->__seq = 0;
  return (wch == L'\0' ? 0 : (length - state_length));
}

size_t mbsnrtowcs(wchar_t* dst, const char** src, size_t nmc, size_t len, mbstate_t* ps) {
  size_t i, o, r;

  if (dst == NULL) {
    for (i = o = 0; i < nmc; i += r, o++) {
      if (static_cast<uint8_t>((*src)[i]) < 0x80) {
        // Fast path for plain ASCII characters.
        if ((*src)[i] == '\0') {
          return o;
        }
        r = 1;
      } else {
        r = mbrtowc(NULL, *src + i, nmc - i, ps);
        if (r == ERR_ILLEGAL_SEQUENCE) {
          return r;
        }
        if (r == ERR_INCOMPLETE_SEQUENCE) {
          return o;
        }
        if (r == 0) {
          return o;
        }
      }
    }
    return o;
  }

  for (i = o = 0; i < nmc && o < len; i += r, o++) {
    if (static_cast<uint8_t>((*src)[i]) < 0x80) {
      // Fast path for plain ASCII characters.
      dst[o] = (*src)[i];
      if ((*src)[i] == '\0') {
        *src = NULL;
        return o;
      }
      r = 1;
    } else {
      r = mbrtowc(dst + o, *src + i, nmc - i, ps);
      if (r == ERR_ILLEGAL_SEQUENCE) {
        *src += i;
        return r;
      }
      if (r == ERR_INCOMPLETE_SEQUENCE) {
        *src += nmc;
        return o;
      }
      if (r == 0) {
        *src = NULL;
        return o;
      }
    }
  }
  *src += i;
  return o;
}

size_t mbsrtowcs(wchar_t* dst, const char** src, size_t len, mbstate_t* ps) {
  return mbsnrtowcs(dst, src, SIZE_MAX, len, ps);
}

size_t wcrtomb(char* s, wchar_t wc, mbstate_t*) {
  // We don't use the sate.

  if (s == NULL) {
    // Reset to initial shift state (no-op).
    return 1;
  }

  if ((wc & ~0x7f) == 0) {
    // Fast path for plain ASCII characters.
    *s = wc;
    return 1;
  }

  // Determine the number of octets needed to represent this character.
  // We always output the shortest sequence possible. Also specify the
  // first few bits of the first octet, which contains the information
  // about the sequence length.
  uint8_t lead;
  size_t length;
  if ((wc & ~0x7f) == 0) {
    lead = 0;
    length = 1;
  } else if ((wc & ~0x7ff) == 0) {
    lead = 0xc0;
    length = 2;
  } else if ((wc & ~0xffff) == 0) {
    lead = 0xe0;
    length = 3;
  } else if ((wc & ~0x1fffff) == 0) {
    lead = 0xf0;
    length = 4;
  } else {
    errno = EILSEQ;
    return ERR_ILLEGAL_SEQUENCE;
  }

  // Output the octets representing the character in chunks
  // of 6 bits, least significant last. The first octet is
  // a special case because it contains the sequence length
  // information.
  for (size_t i = length - 1; i > 0; i--) {
    s[i] = (wc & 0x3f) | 0x80;
    wc >>= 6;
  }
  *s = (wc & 0xff) | lead;

  return length;
}

size_t wcsnrtombs(char* dst, const wchar_t** src, size_t nwc, size_t len, mbstate_t* ps) {
  char buf[MB_LEN_MAX];
  size_t i, o, r;
  if (dst == NULL) {
    for (i = o = 0; i < nwc; i++, o += r) {
      wchar_t wc = (*src)[i];
      if (static_cast<uint32_t>(wc) < 0x80) {
        // Fast path for plain ASCII characters.
        if (wc == 0) {
          return o;
        }
        r = 1;
      } else {
        r = wcrtomb(buf, wc, ps);
        if (r == ERR_ILLEGAL_SEQUENCE) {
          return r;
        }
      }
    }
    return o;
  }

  for (i = o = 0; i < nwc && o < len; i++, o += r) {
    wchar_t wc = (*src)[i];
    if (static_cast<uint32_t>(wc) < 0x80) {
      // Fast path for plain ASCII characters.
      dst[o] = wc;
      if (wc == 0) {
        *src = NULL;
        return o;
      }
      r = 1;
    } else if (len - o >= sizeof(buf)) {
      // Enough space to translate in-place.
      r = wcrtomb(dst + o, wc, ps);
      if (r == ERR_ILLEGAL_SEQUENCE) {
        *src += i;
        return r;
      }
    } else {
      // May not be enough space; use temp buffer.
      r = wcrtomb(buf, wc, ps);
      if (r == ERR_ILLEGAL_SEQUENCE) {
        *src += i;
        return r;
      }
      if (r > len - o) {
        break;
      }
      memcpy(dst + o, buf, r);
    }
  }
  *src += i;
  return o;
}

size_t wcsrtombs(char* dst, const wchar_t** src, size_t len, mbstate_t* ps) {
  return wcsnrtombs(dst, src, SIZE_MAX, len, ps);
}
