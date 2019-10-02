/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "private/NetdClientDispatch.h"

#include <dlfcn.h>
#include <string.h>
#include <unistd.h>

#include <async_safe/log.h>

template <typename FunctionType>
static void netd_init_fn(void* handle, const char* symbol, FunctionType* function_ptr) {
  typedef void (*InitFunctionType)(FunctionType*);
  InitFunctionType fn = reinterpret_cast<InitFunctionType>(dlsym(handle, symbol));
  if (fn != nullptr) fn(function_ptr);
}

NetdClientDispatch* __netd_client_init() {
  static NetdClientDispatch instance = {
    __accept4,
    __connect,
    __sendmmsg,
    __sendmsg,
    __sendto,
    __socket,
    __netIdForResolv_fallback,
    __dnsOpenProxy_fallback,
  };

  // Prevent netd from looping back fwmarkd connections to itself.
  // It would work, but it's  a deadlock hazard and unnecessary overhead for the resolver.
  if (getuid() == 0 && strcmp(basename(getprogname()), "netd") == 0) return &instance;

  // If the library is not available, it's not an error.
  // We'll just use default implementations of functions that it would've overridden.
  void* handle = dlopen("libnetd_client.so", RTLD_NOW);
  if (handle == nullptr) return &instance;

  netd_init_fn(handle, "netdClientInitAccept4", &instance.accept4);
  netd_init_fn(handle, "netdClientInitConnect", &instance.connect);
  netd_init_fn(handle, "netdClientInitSendmmsg", &instance.sendmmsg);
  netd_init_fn(handle, "netdClientInitSendmsg", &instance.sendmsg);
  netd_init_fn(handle, "netdClientInitSendto", &instance.sendto);
  netd_init_fn(handle, "netdClientInitSocket", &instance.socket);
  netd_init_fn(handle, "netdClientInitNetIdForResolv", &instance.netIdForResolv);
  netd_init_fn(handle, "netdClientInitDnsOpenProxy", &instance.dnsOpenProxy);

  return &instance;
}
