/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <async_safe/log.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>

#define error_log(format, ...) \
  async_safe_format_log(ANDROID_LOG_ERROR, "bind", (format), ##__VA_ARGS__)
#define info_log(format, ...) \
  async_safe_format_log(ANDROID_LOG_INFO, "bind", (format), ##__VA_ARGS__)

extern "C" int __bind(int fd, const struct sockaddr* addr, socklen_t addr_length);

// b/170214442: Intercept bind calls. This will be reverted before release.
int bind(int fd, const struct sockaddr* addr, socklen_t addr_length) {
  int sock_type;
  socklen_t sock_length = sizeof(sock_type);
  int err;
  if ((err = getsockopt(fd, SOL_SOCKET, SO_TYPE, &sock_type, &sock_length) < 0)) {
    error_log("Could not get socket type: %s", strerror(errno));
  }
  // TODO: this is where we would do the actual filtering. Print for now.
  info_log("Called bind() with a socket of type %d", sock_type);

  return __bind(fd, addr, addr_length);
}
