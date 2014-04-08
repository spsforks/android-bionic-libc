/*
 * Copyright (C) 2008 The Android Open Source Project
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
#include <errno.h>
#include <locale.h>
#include <nl_types.h>
#include <stdio.h> /* for logging of UNIMPLEMENTED */
#include <stdlib.h>
#include <wctype.h>

#include "private/libc_logging.h"

// newlocale(3) always fails on bionic.
locale_t newlocale(int category_mask, const char *locale, locale_t base)
{
    return NULL;
}

// uselocale(3) always fails on bionic.
long int* uselocale(locale_t newloc)
{
    return NULL;
}

static void unimplemented_stub(const char* function)
{
  const char* fmt = "%s(3) is not implemented on Android\n";
  __libc_format_log(ANDROID_LOG_WARN, "libc", fmt, function);
  fprintf(stderr, fmt, function);
}

#define UNIMPLEMENTED unimplemented_stub(__PRETTY_FUNCTION__)

int isalnum_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int isalpha_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int isascii_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int isblank_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iscntrl_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int isdigit_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int isgraph_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int islower_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int isprint_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int ispunct_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int isspace_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int isupper_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int isxdigit_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iswalnum_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iswalpha_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iswascii_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iswblank_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iswcntrl_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iswdigit_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iswgraph_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iswlower_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iswprint_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iswpunct_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iswspace_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iswupper_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int iswxdigit_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

void freelocale(locale_t locobj)
{
	UNIMPLEMENTED;
}

struct lconv *localeconv(void)
{
	UNIMPLEMENTED;
	return NULL;
}

size_t strftime_l(char *s,
				  size_t max,
				  const char *format,
				  const struct tm * tm,
				  locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

nl_catd catopen(const char *name, int flag)
{
	UNIMPLEMENTED;
	return (nl_catd)-1;
}

int catclose(nl_catd catalog)
{
	UNIMPLEMENTED;
	return -1;
}

char *catgets(nl_catd catalog,
			  int set_number,
			  int message_number,
			  const char *message)
{
	UNIMPLEMENTED;
	return (char *)message;
}

int toupper_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return -1;
}

int tolower_l(int c, locale_t locale)
{
	UNIMPLEMENTED;
	return -1;
}


wint_t towupper_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return -1;
}

wint_t towlower_l(wint_t c, locale_t locale)
{
	UNIMPLEMENTED;
	return -1;
}

int strcoll_l(const char *s1, const char *s2, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

size_t strxfrm_l(char *__restrict dest,
				 const char *__restrict src,
				 size_t n,
				 locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

int wcscoll_l(const wchar_t *s1, const wchar_t *s2, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

size_t wcsxfrm_l(wchar_t *dest, const wchar_t *src, size_t n, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

size_t wcsnrtombs(char *dest,
				  const wchar_t **src,
				  size_t nwc,
				  size_t len,
				  mbstate_t *ps)
{
	UNIMPLEMENTED;
	errno = EILSEQ;
	return (size_t)-1;
}

size_t mbsnrtowcs(wchar_t *dest,
				  const char **src,
				  size_t nms,
				  size_t len,
				  mbstate_t *ps)
{
	UNIMPLEMENTED;
	errno = EILSEQ;
	return (size_t)-1;
}

int mbtowc(wchar_t *pwc, const char *s, size_t n)
{
	UNIMPLEMENTED;
	return -1;
}

long double strtold_l(const char *nptr, char **endptr, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

long long strtoll_l(const char *nptr,
					char **endptr,
					int base,
					locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

unsigned long long strtoull_l(const char *nptr,
							  char **endptr,
							  int base,
							  locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

long long wcstoll(const wchar_t *nptr, wchar_t **endptr, int base)
{
	UNIMPLEMENTED;
	return 0;
}

unsigned long long wcstoull(const wchar_t *nptr, wchar_t **endptr, int base)
{
	UNIMPLEMENTED;
	return 0;
}

float wcstof(const wchar_t *nptr, wchar_t **endptr)
{
	UNIMPLEMENTED;
	return 0;
}

long double wcstold(const wchar_t *nptr, wchar_t **endptr)
{
	UNIMPLEMENTED;
	return 0;
}

long long wcstoll_l(const wchar_t *nptr, wchar_t **endptr, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

unsigned long long wcstoull_l(const wchar_t *nptr,
							  wchar_t **endptr,
							  locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}

long double wcstold_l(const wchar_t *nptr, wchar_t **endptr, locale_t locale)
{
	UNIMPLEMENTED;
	return 0;
}
