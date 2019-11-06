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

#pragma once

#include <sys/cdefs.h>
#include <stdatomic.h>

#include <android/fd_track.h>

extern "C" _Atomic(android_fd_track_hook_t) __android_fd_track_hook;

#define FD_TRACK_CREATE_NAME(name, fd_value)                      \
  ({                                                              \
    int __fd = (fd_value);                                        \
    if (__fd != -1 && __predict_false(__android_fd_track_hook)) { \
      android_fd_track_event event;                               \
      event.fd = __fd;                                            \
      event.type = ANDROID_FD_TRACK_EVENT_TYPE_CREATE;            \
      event.data.create.function_name = name;                     \
      __android_fd_track_hook(&event);                            \
    }                                                             \
    __fd;                                                         \
  })

#define FD_TRACK_CREATE(fd_value) FD_TRACK_CREATE_NAME(__func__, (fd_value))

#define FD_TRACK_CLOSE(fd_value)                                  \
  ({                                                              \
    int __fd = (fd_value);                                        \
    if (__fd != -1 && __predict_false(__android_fd_track_hook)) { \
      android_fd_track_event event;                               \
      event.fd = __fd;                                            \
      event.type = ANDROID_FD_TRACK_EVENT_TYPE_CLOSE;             \
      __android_fd_track_hook(&event);                            \
    }                                                             \
    __fd;                                                         \
  })
