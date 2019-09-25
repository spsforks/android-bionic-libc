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

#include "private/NetdClient.h"

#include <dlfcn.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <async_safe/log.h>

#ifdef __i386__
#define __socketcall __attribute__((__cdecl__))
#else
#define __socketcall
#endif

extern "C" __socketcall int __accept4(int, sockaddr*, socklen_t*, int);
extern "C" __socketcall int __connect(int, const sockaddr*, socklen_t);
extern "C" __socketcall int __sendmmsg(int, const mmsghdr*, unsigned int, int);
extern "C" __socketcall ssize_t __sendmsg(int, const msghdr*, unsigned int);
extern "C" __socketcall int __sendto(int, const void*, size_t, int, const sockaddr*, socklen_t);
extern "C" __socketcall int __socket(int, int, int);

static unsigned fallBackNetIdForResolv(unsigned netId) {
  return netId;
}

static int fallBackDnsOpenProxy() {
  return -1;
}

// This structure is modified only at startup (when libc.so is loaded) and never
// afterwards, so it's okay that it's read later at runtime without a lock.
static NetdClient __netd_client __attribute__((aligned(32))) = {
  __accept4,
  __connect,
  __sendmmsg,
  __sendmsg,
  __sendto,
  __socket,
  fallBackNetIdForResolv,
  fallBackDnsOpenProxy,
};

int accept4(int fd, sockaddr* addr, socklen_t* addr_length, int flags) {
  return __netd_client.accept4(fd, addr, addr_length, flags);
}

int connect(int fd, const sockaddr* addr, socklen_t addr_length) {
  return __netd_client.connect(fd, addr, addr_length);
}

int sendmmsg(int fd, const struct mmsghdr* msgs, unsigned int msg_count, int flags) {
  return __netd_client.sendmmsg(fd, msgs, msg_count, flags);
}

ssize_t sendmsg(int fd, const struct msghdr* msg, int flags) {
  return __netd_client.sendmsg(fd, msg, flags);
}

ssize_t sendto(int fd, const void* buf, size_t n, int flags,
               const struct sockaddr* dst_addr, socklen_t dst_addr_length) {
  return __netd_client.sendto(fd, buf, n, flags, dst_addr, dst_addr_length);
}

int socket(int domain, int type, int protocol) {
  return __netd_client.socket(domain, type, protocol);
}

template <typename FunctionType>
static void init_netd_fn(void* handle, const char* symbol, FunctionType* function) {
  typedef void (*InitFunctionType)(FunctionType*);
  InitFunctionType initFunction = reinterpret_cast<InitFunctionType>(dlsym(handle, symbol));
  if (initFunction != nullptr) initFunction(function);
}

static void netdClientInitImpl() {
  // Prevent netd from looping back fwmarkd connections to itself. It would work, but it's
  // a deadlock hazard and unnecessary overhead for the resolver.
  if (getuid() == 0 && strcmp(basename(getprogname()), "netd") == 0) {
    async_safe_format_log(ANDROID_LOG_INFO, "netdClient",
                          "Skipping libnetd_client init since *we* are netd");
    return;
  }

  void* handle = dlopen("libnetd_client.so", RTLD_NOW);
  if (handle == nullptr) {
    // If the library is not available, it's not an error. We'll just use
    // default implementations of functions that it would've overridden.
    return;
  }

  init_netd_fn(handle, "netdClientInitAccept4", &__netd_client.accept4);
  init_netd_fn(handle, "netdClientInitConnect", &__netd_client.connect);
  init_netd_fn(handle, "netdClientInitSendmmsg", &__netd_client.sendmmsg);
  init_netd_fn(handle, "netdClientInitSendmsg", &__netd_client.sendmsg);
  init_netd_fn(handle, "netdClientInitSendto", &__netd_client.sendto);
  init_netd_fn(handle, "netdClientInitSocket", &__netd_client.socket);
  init_netd_fn(handle, "netdClientInitNetIdForResolv", &__netd_client.netIdForResolv);
  init_netd_fn(handle, "netdClientInitDnsOpenProxy", &__netd_client.dnsOpenProxy);
}

static pthread_once_t netdClientInitOnce = PTHREAD_ONCE_INIT;

NetdClient* __get_netd_client() {
#if !defined(LIBC_STATIC)
  if (pthread_once(&netdClientInitOnce, netdClientInitImpl)) {
    async_safe_format_log(ANDROID_LOG_ERROR, "netdClient", "Failed to initialize libnetd_client");
  }
#endif
  return &__netd_client;
}
