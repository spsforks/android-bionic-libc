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

#ifndef _WCHAR_H_
#define _WCHAR_H_

#include <sys/cdefs.h>
#include <stdio.h>

#include <stdarg.h>
#include <stddef.h>
#include <time.h>
#include <xlocale.h>

#include <bits/mbstate_t.h>
#include <bits/wchar_limits.h>
#include <bits/wctype.h>

__BEGIN_DECLS

wint_t            btowc(int);
int               fwprintf(FILE *, const wchar_t *, ...);
int               fwscanf(FILE *, const wchar_t *, ...);
wint_t            fgetwc(FILE *);
wchar_t          *fgetws(wchar_t *, int, FILE *);
wint_t            fputwc(wchar_t, FILE *);
int               fputws(const wchar_t *, FILE *);
int               fwide(FILE *, int);
wint_t            getwc(FILE *);
wint_t            getwchar(void);
int               mbsinit(const mbstate_t *);
size_t            mbrlen(const char *, size_t, mbstate_t *);
size_t            mbrtowc(wchar_t *, const char *, size_t, mbstate_t *);
size_t mbsrtowcs(wchar_t*, const char**, size_t, mbstate_t*);
size_t mbsnrtowcs(wchar_t*, const char**, size_t, size_t, mbstate_t*) __INTRODUCED_IN(21);
wint_t            putwc(wchar_t, FILE *);
wint_t            putwchar(wchar_t);
int               swprintf(wchar_t *, size_t, const wchar_t *, ...);
int               swscanf(const wchar_t *, const wchar_t *, ...);
wint_t            ungetwc(wint_t, FILE *);
int vfwprintf(FILE*, const wchar_t*, va_list);
int vfwscanf(FILE*, const wchar_t*, va_list) __INTRODUCED_IN(21);
int vswprintf(wchar_t*, size_t, const wchar_t*, va_list);
int vswscanf(const wchar_t*, const wchar_t*, va_list) __INTRODUCED_IN(21);
int vwprintf(const wchar_t*, va_list);
int vwscanf(const wchar_t*, va_list) __INTRODUCED_IN(21);
wchar_t* wcpcpy (wchar_t*, const wchar_t *);
wchar_t* wcpncpy (wchar_t*, const wchar_t *, size_t);
size_t            wcrtomb(char *, wchar_t, mbstate_t *);
int               wcscasecmp(const wchar_t *, const wchar_t *);
int wcscasecmp_l(const wchar_t*, const wchar_t*, locale_t) __INTRODUCED_IN(23);
wchar_t          *wcscat(wchar_t *, const wchar_t *);
wchar_t          *wcschr(const wchar_t *, wchar_t);
int               wcscmp(const wchar_t *, const wchar_t *);
int               wcscoll(const wchar_t *, const wchar_t *);
wchar_t          *wcscpy(wchar_t *, const wchar_t *);
size_t            wcscspn(const wchar_t *, const wchar_t *);
size_t            wcsftime(wchar_t *, size_t, const wchar_t *, const struct tm *);
size_t            wcslen(const wchar_t *);
int               wcsncasecmp(const wchar_t *, const wchar_t *, size_t);
int wcsncasecmp_l(const wchar_t*, const wchar_t*, size_t, locale_t) __INTRODUCED_IN(23);
wchar_t          *wcsncat(wchar_t *, const wchar_t *, size_t);
int               wcsncmp(const wchar_t *, const wchar_t *, size_t);
wchar_t          *wcsncpy(wchar_t *, const wchar_t *, size_t);
size_t wcsnrtombs(char*, const wchar_t**, size_t, size_t, mbstate_t*) __INTRODUCED_IN(21);
wchar_t          *wcspbrk(const wchar_t *, const wchar_t *);
wchar_t          *wcsrchr(const wchar_t *, wchar_t);
size_t wcsrtombs(char*, const wchar_t**, size_t, mbstate_t*);
size_t            wcsspn(const wchar_t *, const wchar_t *);
wchar_t          *wcsstr(const wchar_t *, const wchar_t *);
double wcstod(const wchar_t*, wchar_t**);
float wcstof(const wchar_t*, wchar_t**) __INTRODUCED_IN(21);
wchar_t* wcstok(wchar_t*, const wchar_t*, wchar_t**);
long wcstol(const wchar_t*, wchar_t**, int);
long long wcstoll(const wchar_t*, wchar_t**, int) __INTRODUCED_IN(21);
long double wcstold(const wchar_t*, wchar_t**) __INTRODUCED_IN(21);
unsigned long wcstoul(const wchar_t*, wchar_t**, int);
unsigned long long wcstoull(const wchar_t*, wchar_t**, int) __INTRODUCED_IN(21);
int               wcswidth(const wchar_t *, size_t);
size_t            wcsxfrm(wchar_t *, const wchar_t *, size_t);
int               wctob(wint_t);
int               wcwidth(wchar_t);
wchar_t          *wmemchr(const wchar_t *, wchar_t, size_t);
int               wmemcmp(const wchar_t *, const wchar_t *, size_t);
wchar_t          *wmemcpy(wchar_t *, const wchar_t *, size_t);
=======
wint_t btowc(int);
int fwprintf(FILE* _Nonnull, const wchar_t* _Nonnull, ...);
int fwscanf(FILE* _Nonnull, const wchar_t* _Nonnull, ...);
wint_t fgetwc(FILE* _Nonnull);
wchar_t* _Nullable fgetws(wchar_t* _Nonnull, int, FILE* _Nonnull);
wint_t fputwc(wchar_t, FILE* _Nonnull);
int fputws(const wchar_t* _Nonnull, FILE* _Nonnull);
int fwide(FILE* _Nonnull, int);
wint_t getwc(FILE* _Nonnull);
wint_t getwchar(void);
int mbsinit(const mbstate_t* _Nonnull);
size_t mbrlen(const char* _Nonnull, size_t, mbstate_t* _Nullable);
size_t mbrtowc(wchar_t* _Nullable, const char* _Nullable, size_t, mbstate_t* _Nullable);
size_t mbsrtowcs(wchar_t* _Nullable, const char* _Nonnull* _Nonnull, size_t, mbstate_t* _Nullable);
size_t mbsnrtowcs(wchar_t* _Nullable, const char* _Nonnull* _Nonnull, size_t, size_t, mbstate_t* _Nullable) __INTRODUCED_IN(21);
wint_t putwc(wchar_t, FILE* _Nonnull);
wint_t putwchar(wchar_t);
int swprintf(wchar_t* _Nonnull, size_t, const wchar_t* _Nonnull, ...);
int swscanf(const wchar_t* _Nonnull, const wchar_t* _Nonnull, ...);
wint_t ungetwc(wint_t, FILE* _Nonnull);
int vfwprintf(FILE* _Nonnull, const wchar_t* _Nonnull, va_list);
int vfwscanf(FILE* _Nonnull, const wchar_t* _Nonnull, va_list) __INTRODUCED_IN(21);
int vswprintf(wchar_t* _Nonnull, size_t, const wchar_t* _Nonnull, va_list);
int vswscanf(const wchar_t* _Nonnull, const wchar_t* _Nonnull, va_list) __INTRODUCED_IN(21);
int vwprintf(const wchar_t* _Nonnull, va_list);
int vwscanf(const wchar_t* _Nonnull, va_list) __INTRODUCED_IN(21);
wchar_t* _Nonnull wcpcpy(wchar_t* _Nonnull, const wchar_t* _Nonnull);
wchar_t* _Nonnull wcpncpy(wchar_t* _Nonnull, const wchar_t* _Nonnull, size_t);
size_t wcrtomb(char* _Nullable, wchar_t, mbstate_t* _Nullable);
int wcscasecmp(const wchar_t* _Nonnull, const wchar_t* _Nonnull);
int wcscasecmp_l(const wchar_t* _Nonnull, const wchar_t* _Nonnull, locale_t _Nonnull) __INTRODUCED_IN(23);
wchar_t* _Nonnull wcscat(wchar_t* _Nonnull, const wchar_t* _Nonnull);
wchar_t* _Nullable wcschr(const wchar_t* _Nonnull, wchar_t);
int wcscmp(const wchar_t* _Nonnull, const wchar_t* _Nonnull);
int wcscoll(const wchar_t* _Nonnull, const wchar_t* _Nonnull);
wchar_t* _Nonnull wcscpy(wchar_t* _Nonnull, const wchar_t* _Nonnull);
size_t wcscspn(const wchar_t* _Nonnull, const wchar_t* _Nonnull);
size_t wcsftime(wchar_t* _Nonnull, size_t, const wchar_t* _Nonnull, const struct tm* _Nonnull);
size_t wcslen(const wchar_t* _Nonnull);
int wcsncasecmp(const wchar_t* _Nonnull, const wchar_t* _Nonnull, size_t);
int wcsncasecmp_l(const wchar_t* _Nonnull, const wchar_t* _Nonnull, size_t, locale_t _Nonnull) __INTRODUCED_IN(23);
wchar_t* _Nonnull wcsncat(wchar_t* _Nonnull, const wchar_t* _Nonnull, size_t);
int wcsncmp(const wchar_t* _Nonnull, const wchar_t* _Nonnull, size_t);
wchar_t* _Nonnull wcsncpy(wchar_t* _Nonnull, const wchar_t* _Nonnull, size_t);
size_t wcsnrtombs(char* _Nullable, const wchar_t* _Nonnull* _Nonnull, size_t, size_t, mbstate_t* _Nullable) __INTRODUCED_IN(21);
wchar_t* _Nullable wcspbrk(const wchar_t* _Nonnull, const wchar_t* _Nonnull);
wchar_t* _Nullable wcsrchr(const wchar_t* _Nonnull, wchar_t);
size_t wcsrtombs(char* _Nullable, const wchar_t* _Nonnull* _Nonnull, size_t, mbstate_t* _Nullable);
size_t wcsspn(const wchar_t* _Nonnull, const wchar_t* _Nonnull);
wchar_t* _Nullable wcsstr(const wchar_t* _Nonnull, const wchar_t* _Nonnull);
double wcstod(const wchar_t* _Nonnull, wchar_t* _Nullable* _Nullable);
float wcstof(const wchar_t* _Nonnull, wchar_t* _Nullable* _Nullable) __INTRODUCED_IN(21);
wchar_t* _Nullable wcstok(wchar_t* _Nullable, const wchar_t* _Nonnull, wchar_t* _Nullable* _Nonnull);
long wcstol(const wchar_t* _Nonnull, wchar_t* _Nullable* _Nullable, int);
long long wcstoll(const wchar_t* _Nonnull, wchar_t* _Nullable* _Nullable, int) __INTRODUCED_IN(21);
long double wcstold(const wchar_t* _Nonnull, wchar_t* _Nullable* _Nullable) __INTRODUCED_IN(21);
unsigned long wcstoul(const wchar_t* _Nonnull, wchar_t* _Nullable* _Nullable, int);
unsigned long long wcstoull(const wchar_t* _Nonnull, wchar_t* _Nullable* _Nullable, int) __INTRODUCED_IN(21);
int wcswidth(const wchar_t* _Nonnull, size_t);
size_t wcsxfrm(wchar_t* _Nullable, const wchar_t* _Nonnull, size_t);
int wctob(wint_t);
int wcwidth(wchar_t);
wchar_t* _Nullable wmemchr(const wchar_t* _Nonnull, wchar_t, size_t);
int wmemcmp(const wchar_t* _Nonnull, const wchar_t* _Nonnull, size_t);
wchar_t* _Nonnull wmemcpy(wchar_t* _Nonnull, const wchar_t* _Nonnull, size_t);

#if defined(__USE_GNU)
wchar_t* _Nonnull wmempcpy(wchar_t* _Nonnull, const wchar_t* _Nonnull, size_t) __INTRODUCED_IN(23);
#endif
wchar_t* _Nonnull wmemmove(wchar_t* _Nonnull, const wchar_t* _Nonnull, size_t);
wchar_t* _Nonnull wmemset(wchar_t* _Nonnull, wchar_t, size_t);
int wprintf(const wchar_t* _Nonnull, ...);
int wscanf(const wchar_t* _Nonnull, ...);

#if __ANDROID_API__ >= __ANDROID_API_L__
long long wcstoll_l(const wchar_t* _Nonnull, wchar_t* _Nullable* _Nullable, int, locale_t _Nonnull) __INTRODUCED_IN(21);
unsigned long long wcstoull_l(const wchar_t* _Nonnull, wchar_t* _Nullable* _Nullable, int, locale_t _Nonnull) __INTRODUCED_IN(21);
long double wcstold_l(const wchar_t* _Nonnull, wchar_t* _Nullable* _Nullable, locale_t _Nonnull) __INTRODUCED_IN(21);

int wcscoll_l(const wchar_t* _Nonnull, const wchar_t* _Nonnull, locale_t _Nonnull) __attribute_pure__ __INTRODUCED_IN(21);
size_t wcsxfrm_l(wchar_t* _Nullable, const wchar_t* _Nonnull, size_t, locale_t _Nonnull) __INTRODUCED_IN(21);
#else
/* Implemented as static inlines before 21. */
#endif

size_t wcslcat(wchar_t* _Nonnull, const wchar_t* _Nonnull, size_t);
size_t wcslcpy(wchar_t* _Nonnull, const wchar_t* _Nonnull, size_t);

FILE* _Nullable open_wmemstream(wchar_t* _Nullable* _Nonnull, size_t* _Nonnull) __INTRODUCED_IN(23);
wchar_t* _Nullable wcsdup(const wchar_t* _Nonnull);
size_t wcsnlen(const wchar_t* _Nonnull, size_t);

__END_DECLS

#endif /* _WCHAR_H_ */
