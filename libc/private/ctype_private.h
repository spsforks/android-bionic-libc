#ifndef DEBUG
#pragma GCC visibility push(hidden)
#endif

/* $OpenBSD: ctype_private.h,v 1.1 2005/08/08 05:53:00 espie Exp $ */
/* Written by Marc Espie, public domain */
#define CTYPE_NUM_CHARS       256
extern const char _C_ctype_[];
extern const short _C_toupper_[];
extern const short _C_tolower_[];

#ifndef DEBUG
#pragma GCC visibility pop
#endif
