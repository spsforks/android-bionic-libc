#include <ctype.h>
#include <inttypes.h>
#include <limits.h>
#include <locale.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <wctype.h>
#include "local.h"

#include <platform/bionic/macros.h>
#include <private/bionic_fortify.h>
#include <private/bionic_mbstate.h>

#define BUF 513 /* Maximum length of numeric string. */

// Flags used during conversion.
// Size/type:
#define LONG 0x00001        // l: long or double
#define LONGDBL 0x00002     // L: long double
#define SHORT 0x00004       // h: short
#define SHORTSHORT 0x00008  // hh: 8 bit integer
#define LLONG 0x00010       // ll: long long (+ deprecated q: quad)
#define POINTER 0x00020     // p: void* (as hex)
#define SIZEINT 0x00040     // z: (signed) size_t
#define MAXINT 0x00080      // j: intmax_t
#define PTRINT 0x00100      // t: ptrdiff_t
#define NOSKIP 0x00200      // [ or c: do not skip blanks
// Modifiers:
#define SUPPRESS 0x00400  // *: suppress assignment
#define UNSIGNED 0x00800  // %[oupxX] conversions
#define ALLOCATE 0x01000  // m: allocate a char*
// Internal use during integer parsing:
#define SIGNOK 0x02000    // +/- is (still) legal
#define HAVESIGN 0x04000  // Sign detected
#define NDIGITS 0x08000   // No digits detected
#define PFXOK 0x10000     // "0x" prefix is (still) legal
#define PFBOK 0x20000     // "0b" prefix is (still) legal
#define NZDIGITS 0x40000  // No zero digits detected

// Conversion types.
#define CT_CHAR 0    // %c conversion
#define CT_CCL 1     // %[...] conversion
#define CT_STRING 2  // %s conversion
#define CT_INT 3     // Integer: strtoimax/strtoumax
#define CT_FLOAT 4   // Float: strtod
