/*
 * Copyright 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fuzzer/FuzzedDataProvider.h>

#include <net/if.h>
#include <stddef.h>
#include <sys/ioctl.h>
#include <unistd.h>

static const int IOCTLS[] = {
    SIOCGIFNAME,        SIOCGIFINDEX,   SIOCGIFFLAGS,   SIOCSIFFLAGS,   SIOCGIFPFLAGS,
    SIOCSIFPFLAGS,      SIOCGIFADDR,    SIOCSIFADDR,    SIOCGIFDSTADDR, SIOCSIFDSTADDR,
    SIOCGIFBRDADDR,     SIOCSIFBRDADDR, SIOCGIFNETMASK, SIOCSIFNETMASK, SIOCGIFMETRIC,
    SIOCSIFMETRIC,      SIOCGIFMTU,     SIOCSIFMTU,     SIOCGIFHWADDR,  SIOCSIFHWADDR,
    SIOCSIFHWBROADCAST, SIOCGIFMAP,     SIOCSIFMAP,     SIOCADDMULTI,   SIOCDELMULTI,
    SIOCGIFTXQLEN,      SIOCSIFTXQLEN,  SIOCSIFNAME,    SIOCGIFCONF};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  FuzzedDataProvider fdp(data, size);

  // Get a valid interface name.
  int index = fdp.ConsumeIntegral<int>();
  char if_name[IFNAMSIZ];
  if (if_indextoname(index, if_name) == nullptr) return 0;

  ifreq ifr;
  strcpy(ifr.ifr_name, if_name);
  // Populate the largest member of the union.
  fdp.ConsumeData(&ifr.ifr_addr, sizeof(struct sockaddr));
  // Choose a random ioctl from the list.
  int ioctl_request = fdp.PickValueInArray(IOCTLS);

  int sock = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
  ioctl(sock, ioctl_request, &ifr);
  close(sock);

  return 0;
}