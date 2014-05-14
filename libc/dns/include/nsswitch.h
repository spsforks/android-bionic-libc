/*	$NetBSD: nsswitch.h,v 1.18 2005/11/29 03:12:58 christos Exp $	*/

/*-
 * Copyright (c) 1997, 1998, 1999, 2004 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Luke Mewburn.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _NSSWITCH_H
#define _NSSWITCH_H	1

#include <sys/types.h>
#include <stdarg.h>

/*
 * Layout of:
 *	uint32_t ns_src.flags
 */
	/* nsswitch.conf status codes and nsdispatch(3) return values */
#define	NS_SUCCESS	(1<<0)		/* entry was found */
#define	NS_UNAVAIL	(1<<1)		/* source not responding, or corrupt */
#define	NS_NOTFOUND	(1<<2)		/* source responded 'no such entry' */
#define	NS_TRYAGAIN	(1<<3)		/* source busy, may respond to retrys */
#define	NS_STATUSMASK	0x000000ff	/* bitmask to get the status flags */

	/* internal nsdispatch(3) flags; not settable in nsswitch.conf(5)  */
#define	NS_FORCEALL	(1<<8)		/* force all methods to be invoked; */

/*
 * Currently implemented sources.
 */
#define NSSRC_FILES	"files"		/* local files */
#define	NSSRC_DNS	"dns"		/* DNS; IN for hosts, HS for others */

/*
 * Currently implemented databases.
 */
#define NSDB_HOSTS		"hosts"

/*
 * ns_dtab `callback' function signature.
 */
typedef	int (*nss_method)(void *, void *, va_list);

/*
 * ns_dtab - `nsswitch dispatch table'
 * Contains an entry for each source and the appropriate function to call.
 */
typedef struct {
	const char	 *src;
	nss_method	 callback;
	void		 *cb_data;
} ns_dtab;

/*
 * Macros to help build an ns_dtab[]
 */
#define NS_FILES_CB(F,C)	{ NSSRC_FILES,	F,	__UNCONST(C) },
#define NS_DNS_CB(F,C)
#define NS_NIS_CB(F,C)

/*
 * ns_src - `nsswitch source'
 * Used by the nsparser routines to store a mapping between a source
 * and its dispatch control flags for a given database.
 */
typedef struct {
	const char	*name;
	uint32_t	 flags;
} ns_src;

#include <sys/cdefs.h>

__BEGIN_DECLS
int	nsdispatch(void *, const ns_dtab [], const char *,
			const char *, const ns_src [], ...);

__END_DECLS

#endif /* !_NSSWITCH_H */
