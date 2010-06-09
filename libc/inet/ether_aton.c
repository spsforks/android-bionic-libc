/*
 * Copyright (C) 2010 The Android Open Source Project
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <net/if_ether.h>
#include <ctype.h>

/*
 * Convert Ethernet address in the standard hex-digits-and-colons to binary
 * representation.
 * Re-entrant version (GNU extensions)
 */
struct ether_addr *
ether_aton_r (const char *asc, struct ether_addr * addr)
{
    int i;
    for (i = 0; i < ETHER_ADDR_LEN; ++i) {
        char *p = 0;
        if (!isxdigit(asc[0]))
            return NULL;
        unsigned long el = strtoul(asc, &p, 16);
        if ((asc == p) || (el > 255))
            return NULL;
        asc = p;
        addr->ether_addr_octet[i] = (u_int8_t)el;
        if (i < ETHER_ADDR_LEN - 1) {
            if (*asc != ':')
                return NULL;
            ++asc;
        }
    }
    if (*asc != '\0')
        return NULL;
    return addr;
}

/*
 * Convert Ethernet address in the standard hex-digits-and-colons to binary
 * representation.
 */
struct ether_addr *
ether_aton (const char *asc)
{
    static struct ether_addr addr;
    return ether_aton_r(asc, &addr);
}
