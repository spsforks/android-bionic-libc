/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include "private/NetdClientDispatch.h"

#include <sys/socket.h>

NetdClientDispatch* __netd_client_init();

unsigned __netIdForResolv_fallback(unsigned netId) {
  return netId;
}

int __dnsOpenProxy_fallback() {
  return -1;
}

int accept4(int fd, sockaddr* addr, socklen_t* addr_length, int flags) {
  return __netd_client()->accept4(fd, addr, addr_length, flags);
}

int connect(int fd, const sockaddr* addr, socklen_t addr_length) {
  return __netd_client()->connect(fd, addr, addr_length);
}

int sendmmsg(int fd, const struct mmsghdr* msgs, unsigned int msg_count, int flags) {
  return __netd_client()->sendmmsg(fd, msgs, msg_count, flags);
}

ssize_t sendmsg(int fd, const struct msghdr* msg, int flags) {
  return __netd_client()->sendmsg(fd, msg, flags);
}

ssize_t sendto(int fd, const void* buf, size_t n, int flags,
               const struct sockaddr* dst_addr, socklen_t dst_addr_length) {
  return __netd_client()->sendto(fd, buf, n, flags, dst_addr, dst_addr_length);
}

int socket(int domain, int type, int protocol) {
  return __netd_client()->socket(domain, type, protocol);
}

NetdClientDispatch* __netd_client() {
  static NetdClientDispatch* instance = __netd_client_init();
  return instance;
}
