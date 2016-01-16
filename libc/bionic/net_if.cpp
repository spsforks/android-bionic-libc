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

#include <net/if.h>

#include <errno.h>
#include <ifaddrs.h>
#include <linux/if_packet.h>
#include <linux/sockios.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "private/ErrnoRestorer.h"

static void __close_quietly(int fd) {
  ErrnoRestorer er;
  close(fd);
}

char* if_indextoname(unsigned ifindex, char* ifname) {
  int s = socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC, 0);
  if (s == -1) return nullptr;

  struct ifreq ifr = {};
  ifr.ifr_ifindex = ifindex;

  int rc = ioctl(s, SIOCGIFNAME, &ifr);
  __close_quietly(s);
  return (rc == -1) ? nullptr : strncpy(ifname, ifr.ifr_name, IFNAMSIZ);
}

unsigned if_nametoindex(const char* ifname) {
  int s = socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC, 0);
  if (s == -1) return 0;

  struct ifreq ifr = {};
  strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
  ifr.ifr_name[IFNAMSIZ - 1] = 0;

  int rc = ioctl(s, SIOCGIFINDEX, &ifr);
  __close_quietly(s);
  return (rc == -1) ? 0 : ifr.ifr_ifindex;
}

struct if_nameindex* if_nameindex() {
  ifaddrs* ifas;
  if (getifaddrs(&ifas) == -1) return nullptr;

  size_t interface_count = 0;
  size_t string_byte_count = 0;
  for (ifaddrs* ifa = ifas; ifa != nullptr; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_PACKET) {
      ++interface_count;
      string_byte_count += strlen(ifa->ifa_name) + 1;
    }
  }

  size_t byte_count = (interface_count + 1) * sizeof(struct if_nameindex) + string_byte_count;
  struct if_nameindex* result = reinterpret_cast<struct if_nameindex*>(malloc(byte_count));
  if (result) {
    struct if_nameindex* i = result;
    char* p = reinterpret_cast<char*>(&result[interface_count + 1]);
    for (ifaddrs* ifa = ifas; ifa != nullptr; ifa = ifa->ifa_next) {
      if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_PACKET) {
        i->if_index = reinterpret_cast<sockaddr_ll*>(ifa->ifa_addr)->sll_ifindex;
        i->if_name = p;
        strcpy(p, ifa->ifa_name);
        p += strlen(ifa->ifa_name) + 1;
        ++i;
      }
    }
    i->if_index = 0;
    i->if_name = nullptr;
  }

  freeifaddrs(ifas);
  return result;
}

void if_freenameindex(struct if_nameindex* ptr) {
  free(ptr);
}
