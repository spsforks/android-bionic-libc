/*	$OpenBSD: mbtowc.c,v 1.3 2016/02/27 14:02:13 schwarze Exp $ */

/*-
 * Copyright (c) 2002-2004 Tim J. Robbins.
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


#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>

int
mbtowc(wchar_t * __restrict pwc, const char * __restrict s, size_t n)
{
	static mbstate_t mbs;
	size_t rval;

	if (s == NULL) {
		/* No support for state dependent encodings. */
		memset(&mbs, 0, sizeof(mbs));
		return (0);
	}

	// Android edit: mbtowc and all the mbrto* APIs behave slightly differently
	// when n is 0:
	//
	// mbrtowc returns 0 "if the next n or fewer bytes complete the multibyte 
	// character that corresponds to the null wide character"
	//
	// mbrtoc says: "If s is not a null pointer, the mbtowc function either
	// returns 0 (if s points to the null character)..."
	//
	// So mbrtowc will not provide the correct mbtowc return value for "" and
	// n = 0.
	if (*s == '\0') {
		return 0;
	}

	rval = mbrtowc(pwc, s, n, &mbs);

	switch (rval) {
	case (size_t)-2:
		errno = EILSEQ;
		/* FALLTHROUGH */
	case (size_t)-1:
		return -1;
	default:
		return (int)rval;
	}
}
