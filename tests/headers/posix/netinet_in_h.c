/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <netinet/in.h>

#include "header_checks.h"

static void netinet_in_h() {
  TYPE(in_port_t);
  TYPE(in_addr_t);
  TYPE(sa_family_t);
  TYPE(uint8_t);
  TYPE(uint32_t);

  TYPE(struct in_addr);
  STRUCT_MEMBER(struct in_addr, in_addr_t, s_addr);

  TYPE(struct sockaddr_in);
  STRUCT_MEMBER(struct sockaddr_in, sa_family_t, sin_family);
  STRUCT_MEMBER(struct sockaddr_in, in_port_t, sin_port);
  STRUCT_MEMBER(struct sockaddr_in, struct in_addr, sin_addr);

  TYPE(struct in6_addr);
  STRUCT_MEMBER(struct in6_addr, uint8_t[16], s6_addr);

  TYPE(struct sockaddr_in6);
  STRUCT_MEMBER(struct sockaddr_in6, sa_family_t, sin6_family);
  STRUCT_MEMBER(struct sockaddr_in6, in_port_t, sin6_port);
  STRUCT_MEMBER(struct sockaddr_in6, uint32_t, sin6_flowinfo);
  STRUCT_MEMBER(struct sockaddr_in6, struct in6_addr, sin6_addr);
  STRUCT_MEMBER(struct sockaddr_in6, uint32_t, sin6_scope_id);

  struct in6_addr x = in6addr_any;
  MACRO(IN6ADDR_ANY_INIT);
  x = IN6ADDR_ANY_INIT;

  x = in6addr_loopback;
  MACRO(IN6ADDR_LOOPBACK_INIT);
  x = IN6ADDR_LOOPBACK_INIT;

  TYPE(struct ipv6_mreq);
  STRUCT_MEMBER(struct ipv6_mreq, struct in6_addr, ipv6mr_multiaddr);
  STRUCT_MEMBER(struct ipv6_mreq, unsigned, ipv6mr_interface);

  MACRO(IPPROTO_IP);
  MACRO(IPPROTO_IPV6);
  MACRO(IPPROTO_ICMP);
  MACRO(IPPROTO_RAW);
  MACRO(IPPROTO_TCP);
  MACRO(IPPROTO_UDP);

  MACRO(INADDR_ANY);
  MACRO(INADDR_BROADCAST);

  MACRO_VALUE(INET_ADDRSTRLEN, 16);

  FUNCTION(htonl, uint32_t (*f)(uint32_t));
  FUNCTION(htons, uint16_t (*f)(uint16_t));
  FUNCTION(ntohl, uint32_t (*f)(uint32_t));
  FUNCTION(ntohs, uint16_t (*f)(uint16_t));

  MACRO_VALUE(INET6_ADDRSTRLEN, 46);

  MACRO(IPV6_JOIN_GROUP);
  MACRO(IPV6_LEAVE_GROUP);
  MACRO(IPV6_MULTICAST_HOPS);
  MACRO(IPV6_MULTICAST_IF);
  MACRO(IPV6_MULTICAST_LOOP);
  MACRO(IPV6_UNICAST_HOPS);
  MACRO(IPV6_V6ONLY);

  MACRO(IN6_IS_ADDR_UNSPECIFIED);
  MACRO(IN6_IS_ADDR_LOOPBACK);
  MACRO(IN6_IS_ADDR_MULTICAST);
  MACRO(IN6_IS_ADDR_LINKLOCAL);
  MACRO(IN6_IS_ADDR_SITELOCAL);
  MACRO(IN6_IS_ADDR_V4MAPPED);
  MACRO(IN6_IS_ADDR_V4COMPAT);
  MACRO(IN6_IS_ADDR_MC_NODELOCAL);
  MACRO(IN6_IS_ADDR_MC_LINKLOCAL);
  MACRO(IN6_IS_ADDR_MC_SITELOCAL);
  MACRO(IN6_IS_ADDR_MC_ORGLOCAL);
  MACRO(IN6_IS_ADDR_MC_GLOBAL);
}
