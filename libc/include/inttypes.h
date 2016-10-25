/*	$OpenBSD: inttypes.h,v 1.9 2006/01/15 00:47:51 millert Exp $	*/

/*
 * Copyright (c) 1997, 2005 Todd C. Miller <Todd.Miller@courtesan.com>
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
 */

#ifndef	_INTTYPES_H_
#define	_INTTYPES_H_

#include <stdint.h>
#include <sys/cdefs.h>

#ifdef __LP64__
#define __PRI_FAST16_prefix "l"
#define __PRI_FAST32_prefix "l"
#define __PRI_64_prefix  "l"
#define __PRI_PTR_prefix "l"
#else
#define __PRI_FAST16_prefix
#define __PRI_FAST32_prefix
#define __PRI_64_prefix "ll"
#define __PRI_PTR_prefix
#endif

/*
 * 7.8.1 Macros for format specifiers
 *
 * Each of the following object-like macros expands to a string
 * literal containing a conversion specifier, possibly modified by
 * a prefix such as hh, h, l, or ll, suitable for use within the
 * format argument of a formatted input/output function when
 * converting the corresponding integer type.  These macro names
 * have the general form of PRI (character string literals for the
 * fprintf family) or SCN (character string literals for the fscanf
 * family), followed by the conversion specifier, followed by a
 * name corresponding to a similar typedef name.  For example,
 * PRIdFAST32 can be used in a format string to print the value of
 * an integer of type int_fast32_t.
 */

/* fprintf macros for signed integers */
#define	PRId8			"d"		/* int8_t */
#define	PRId16			"d"		/* int16_t */
#define	PRId32			"d"		/* int32_t */
#define	PRId64			__PRI_64_prefix"d"		/* int64_t */

#define	PRIdLEAST8		"d"		/* int_least8_t */
#define	PRIdLEAST16		"d"		/* int_least16_t */
#define	PRIdLEAST32		"d"		/* int_least32_t */
#define	PRIdLEAST64		__PRI_64_prefix"d"		/* int_least64_t */

#define	PRIdFAST8		"d"		/* int_fast8_t */
#define	PRIdFAST16		__PRI_FAST16_prefix"d"	/* int_fast16_t */
#define	PRIdFAST32		__PRI_FAST32_prefix"d"	/* int_fast32_t */
#define	PRIdFAST64		__PRI_64_prefix"d"		/* int_fast64_t */

#define	PRIdMAX			"jd"		/* intmax_t */
#define	PRIdPTR			__PRI_PTR_prefix"d"		/* intptr_t */

#define	PRIi8			"i"		/* int8_t */
#define	PRIi16			"i"		/* int16_t */
#define	PRIi32			"i"		/* int32_t */
#define	PRIi64			__PRI_64_prefix"i"		/* int64_t */

#define	PRIiLEAST8		"i"		/* int_least8_t */
#define	PRIiLEAST16		"i"		/* int_least16_t */
#define	PRIiLEAST32		"i"		/* int_least32_t */
#define	PRIiLEAST64		__PRI_64_prefix"i"		/* int_least64_t */

#define	PRIiFAST8		"i"		/* int_fast8_t */
#define	PRIiFAST16		__PRI_FAST16_prefix"i"	/* int_fast16_t */
#define	PRIiFAST32		__PRI_FAST32_prefix"i"	/* int_fast32_t */
#define	PRIiFAST64		__PRI_64_prefix"i"		/* int_fast64_t */

#define	PRIiMAX			"ji"		/* intmax_t */
#define	PRIiPTR			__PRI_PTR_prefix"i"		/* intptr_t */

/* fprintf macros for unsigned integers */
#define	PRIo8			"o"		/* int8_t */
#define	PRIo16			"o"		/* int16_t */
#define	PRIo32			"o"		/* int32_t */
#define	PRIo64			__PRI_64_prefix"o"		/* int64_t */

#define	PRIoLEAST8		"o"		/* int_least8_t */
#define	PRIoLEAST16		"o"		/* int_least16_t */
#define	PRIoLEAST32		"o"		/* int_least32_t */
#define	PRIoLEAST64		__PRI_64_prefix"o"		/* int_least64_t */

#define	PRIoFAST8		"o"		/* int_fast8_t */
#define	PRIoFAST16		__PRI_FAST16_prefix"o"	/* int_fast16_t */
#define	PRIoFAST32		__PRI_FAST32_prefix"o"	/* int_fast32_t */
#define	PRIoFAST64		__PRI_64_prefix"o"		/* int_fast64_t */

#define	PRIoMAX			"jo"		/* intmax_t */
#define	PRIoPTR			__PRI_PTR_prefix"o"		/* intptr_t */

#define	PRIu8			"u"		/* uint8_t */
#define	PRIu16			"u"		/* uint16_t */
#define	PRIu32			"u"		/* uint32_t */
#define	PRIu64			__PRI_64_prefix"u"		/* uint64_t */

#define	PRIuLEAST8		"u"		/* uint_least8_t */
#define	PRIuLEAST16		"u"		/* uint_least16_t */
#define	PRIuLEAST32		"u"		/* uint_least32_t */
#define	PRIuLEAST64		__PRI_64_prefix"u"		/* uint_least64_t */

#define	PRIuFAST8		"u"		/* uint_fast8_t */
#define	PRIuFAST16		__PRI_FAST16_prefix"u"	/* uint_fast16_t */
#define	PRIuFAST32		__PRI_FAST32_prefix"u"	/* uint_fast32_t */
#define	PRIuFAST64		__PRI_64_prefix"u"		/* uint_fast64_t */

#define	PRIuMAX			"ju"		/* uintmax_t */
#define	PRIuPTR			__PRI_PTR_prefix"u"		/* uintptr_t */

#define	PRIx8			"x"		/* uint8_t */
#define	PRIx16			"x"		/* uint16_t */
#define	PRIx32			"x"		/* uint32_t */
#define	PRIx64			__PRI_64_prefix"x"		/* uint64_t */

#define	PRIxLEAST8		"x"		/* uint_least8_t */
#define	PRIxLEAST16		"x"		/* uint_least16_t */
#define	PRIxLEAST32		"x"		/* uint_least32_t */
#define	PRIxLEAST64		__PRI_64_prefix"x"		/* uint_least64_t */

#define	PRIxFAST8		"x"		/* uint_fast8_t */
#define	PRIxFAST16		__PRI_FAST16_prefix"x"	/* uint_fast16_t */
#define	PRIxFAST32		__PRI_FAST32_prefix"x"	/* uint_fast32_t */
#define	PRIxFAST64		__PRI_64_prefix"x"		/* uint_fast64_t */

#define	PRIxMAX			"jx"		/* uintmax_t */
#define	PRIxPTR			__PRI_PTR_prefix"x"		/* uintptr_t */

#define	PRIX8			"X"		/* uint8_t */
#define	PRIX16			"X"		/* uint16_t */
#define	PRIX32			"X"		/* uint32_t */
#define	PRIX64			__PRI_64_prefix"X"		/* uint64_t */

#define	PRIXLEAST8		"X"		/* uint_least8_t */
#define	PRIXLEAST16		"X"		/* uint_least16_t */
#define	PRIXLEAST32		"X"		/* uint_least32_t */
#define	PRIXLEAST64		__PRI_64_prefix"X"		/* uint_least64_t */

#define	PRIXFAST8		"X"		/* uint_fast8_t */
#define	PRIXFAST16		__PRI_FAST16_prefix"X"	/* uint_fast16_t */
#define	PRIXFAST32		__PRI_FAST32_prefix"X"	/* uint_fast32_t */
#define	PRIXFAST64		__PRI_64_prefix"X"		/* uint_fast64_t */

#define	PRIXMAX			"jX"		/* uintmax_t */
#define	PRIXPTR			__PRI_PTR_prefix"X"		/* uintptr_t */

/* fscanf macros for signed integers */
#define	SCNd8			"hhd"		/* int8_t */
#define	SCNd16			"hd"		/* int16_t */
#define	SCNd32			"d"		/* int32_t */
#define	SCNd64			__PRI_64_prefix"d"		/* int64_t */

#define	SCNdLEAST8		"hhd"		/* int_least8_t */
#define	SCNdLEAST16		"hd"		/* int_least16_t */
#define	SCNdLEAST32		"d"		/* int_least32_t */
#define	SCNdLEAST64		__PRI_64_prefix"d"		/* int_least64_t */

#define	SCNdFAST8		"hhd"		/* int_fast8_t */
#define	SCNdFAST16		__PRI_FAST16_prefix"d"	/* int_fast16_t */
#define	SCNdFAST32		__PRI_FAST32_prefix"d"	/* int_fast32_t */
#define	SCNdFAST64		__PRI_64_prefix"d"		/* int_fast64_t */

#define	SCNdMAX			"jd"		/* intmax_t */
#define	SCNdPTR			__PRI_PTR_prefix"d"		/* intptr_t */

#define	SCNi8			"hhi"		/* int8_t */
#define	SCNi16			"hi"		/* int16_t */
#define	SCNi32			"i"		/* int32_t */
#define	SCNi64			__PRI_64_prefix"i"		/* int64_t */

#define	SCNiLEAST8		"hhi"		/* int_least8_t */
#define	SCNiLEAST16		"hi"		/* int_least16_t */
#define	SCNiLEAST32		"i"		/* int_least32_t */
#define	SCNiLEAST64		__PRI_64_prefix"i"		/* int_least64_t */

#define	SCNiFAST8		"hhi"		/* int_fast8_t */
#define	SCNiFAST16		__PRI_FAST16_prefix"i"	/* int_fast16_t */
#define	SCNiFAST32		__PRI_FAST32_prefix"i"	/* int_fast32_t */
#define	SCNiFAST64		__PRI_64_prefix"i"		/* int_fast64_t */

#define	SCNiMAX			"ji"		/* intmax_t */
#define	SCNiPTR			__PRI_PTR_prefix"i"		/* intptr_t */

/* fscanf macros for unsigned integers */
#define	SCNo8			"hho"		/* uint8_t */
#define	SCNo16			"ho"		/* uint16_t */
#define	SCNo32			"o"		/* uint32_t */
#define	SCNo64			__PRI_64_prefix"o"		/* uint64_t */

#define	SCNoLEAST8		"hho"		/* uint_least8_t */
#define	SCNoLEAST16		"ho"		/* uint_least16_t */
#define	SCNoLEAST32		"o"		/* uint_least32_t */
#define	SCNoLEAST64		__PRI_64_prefix"o"		/* uint_least64_t */

#define	SCNoFAST8		"hho"		/* uint_fast8_t */
#define	SCNoFAST16		__PRI_FAST16_prefix"o"	/* uint_fast16_t */
#define	SCNoFAST32		__PRI_FAST32_prefix"o"	/* uint_fast32_t */
#define	SCNoFAST64		__PRI_64_prefix"o"		/* uint_fast64_t */

#define	SCNoMAX			"jo"		/* uintmax_t */
#define	SCNoPTR			__PRI_PTR_prefix"o"		/* uintptr_t */

#define	SCNu8			"hhu"		/* uint8_t */
#define	SCNu16			"hu"		/* uint16_t */
#define	SCNu32			"u"		/* uint32_t */
#define	SCNu64			__PRI_64_prefix"u"		/* uint64_t */

#define	SCNuLEAST8		"hhu"		/* uint_least8_t */
#define	SCNuLEAST16		"hu"		/* uint_least16_t */
#define	SCNuLEAST32		"u"		/* uint_least32_t */
#define	SCNuLEAST64		__PRI_64_prefix"u"		/* uint_least64_t */

#define	SCNuFAST8		"hhu"		/* uint_fast8_t */
#define	SCNuFAST16		__PRI_FAST16_prefix"u"	/* uint_fast16_t */
#define	SCNuFAST32		__PRI_FAST32_prefix"u"		/* uint_fast32_t */
#define	SCNuFAST64		__PRI_64_prefix"u"		/* uint_fast64_t */

#define	SCNuMAX			"ju"		/* uintmax_t */
#define	SCNuPTR			__PRI_PTR_prefix"u"		/* uintptr_t */

#define	SCNx8			"hhx"		/* uint8_t */
#define	SCNx16			"hx"		/* uint16_t */
#define	SCNx32			"x"		/* uint32_t */
#define	SCNx64			__PRI_64_prefix"x"		/* uint64_t */

#define	SCNxLEAST8		"hhx"		/* uint_least8_t */
#define	SCNxLEAST16		"hx"		/* uint_least16_t */
#define	SCNxLEAST32		"x"		/* uint_least32_t */
#define	SCNxLEAST64		__PRI_64_prefix"x"		/* uint_least64_t */

#define	SCNxFAST8		"hhx"		/* uint_fast8_t */
#define	SCNxFAST16		__PRI_FAST16_prefix"x"	/* uint_fast16_t */
#define	SCNxFAST32		__PRI_FAST32_prefix"x"	/* uint_fast32_t */
#define	SCNxFAST64		__PRI_64_prefix"x"		/* uint_fast64_t */

#define	SCNxMAX			"jx"		/* uintmax_t */
#define	SCNxPTR			__PRI_PTR_prefix"x"		/* uintptr_t */

typedef struct {
	intmax_t quot;		/* quotient */
	intmax_t rem;		/* remainder */
} imaxdiv_t;

__BEGIN_DECLS
intmax_t imaxabs(intmax_t) __attribute_const__ __INTRODUCED_IN(19);
imaxdiv_t imaxdiv(intmax_t, intmax_t) __attribute_const__ __INTRODUCED_IN(19);
intmax_t	strtoimax(const char *, char **, int);
uintmax_t	strtoumax(const char *, char **, int);
intmax_t wcstoimax(const wchar_t* __restrict, wchar_t** __restrict, int) __INTRODUCED_IN(21);
uintmax_t wcstoumax(const wchar_t* __restrict, wchar_t** __restrict, int) __INTRODUCED_IN(21);
__END_DECLS

#endif /* _INTTYPES_H_ */
