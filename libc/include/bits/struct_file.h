/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef BITS_FILE_H
#define BITS_FILE_H

#include <bits/fpos_t.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

struct __sbuf {
  unsigned char* _base;
#if defined(__LP64__)
  size_t _size;
#else
  int _size;
#endif
};

struct __sFILE {
  unsigned char* _p; /* current position in (some) buffer */
  int _r;            /* read space left for getc() */
  int _w;            /* write space left for putc() */
#if defined(__LP64__)
  int _flags; /* flags, below; this FILE is free if 0 */
  int _file;  /* fileno, if Unix descriptor, else -1 */
#else
  short _flags; /* flags, below; this FILE is free if 0 */
  short _file;  /* fileno, if Unix descriptor, else -1 */
#endif
  struct __sbuf _bf; /* the buffer (at least 1 byte, if !NULL) */
  int _lbfsize;      /* 0 or -_bf._size, for inline putc */

  // Function pointers used by `funopen`.
  // Note that `_seek` is ignored if `_seek64` (in __sfileext) is set.
  // TODO: NetBSD has `funopen2` which corrects the `int`s to `size_t`s.
  // TODO: glibc has `fopencookie` which passes the function pointers in a struct.
  void* _cookie; /* cookie passed to io functions */
  int (*_close)(void*);
  int (*_read)(void*, char*, int);
  fpos_t (*_seek)(void*, fpos_t, int);
  int (*_write)(void*, const char*, int);

  /* extension data, to avoid further ABI breakage */
  struct __sbuf _ext;
  /* data for long sequences of ungetc() */
  unsigned char* _up; /* saved _p when _p is doing ungetc data */
  int _ur;            /* saved _r when _r is counting ungetc data */

  /* tricks to meet minimum requirements even when malloc() fails */
  unsigned char _ubuf[3]; /* guarantee an ungetc() buffer */
  unsigned char _nbuf[1]; /* guarantee a getc() buffer */

  /* separate buffer for fgetln() when line crosses buffer boundary */
  struct __sbuf _lb; /* buffer for fgetln() */

  /* Unix stdio files get aligned to block boundaries on fseek() */
  int _blksize; /* stat.st_blksize (may be != _bf._size) */

  fpos_t _unused_0;  // This was the `_offset` field (see below).

  // Do not add new fields here. (Or remove or change the size of any above.)
  // Although bionic currently exports `stdin`, `stdout`, and `stderr` symbols,
  // that still hasn't made it to the NDK. All NDK-built apps index directly
  // into an array of this struct (which was in <stdio.h> historically), so if
  // you need to make any changes, they need to be in the `__sfileext` struct
  // below, and accessed via `_EXT`.
};

__END_DECLS

#endif /* BITS_FILE_H */
