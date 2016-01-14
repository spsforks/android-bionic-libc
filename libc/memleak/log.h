/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef LIBMEMLEAK_LOG_H_
#define LIBMEMLEAK_LOG_H_

#include <unistd.h>
#include <sys/syscall.h>

#include "private/libc_logging.h"

#define log_fatal(fmt, ...) __libc_fatal(fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) __libc_format_log(ANDROID_LOG_ERROR, "libmemleak", "%d: " fmt, (pid_t)syscall(SYS_gettid), ##__VA_ARGS__)
#define log_info(fmt, ...) __libc_format_log(ANDROID_LOG_INFO, "libmemleak", "%d: " fmt, (pid_t)syscall(SYS_gettid), ##__VA_ARGS__)
#define log_debug(fmt, ...) __libc_format_log(ANDROID_LOG_DEBUG, "libmemleak", "%d: " fmt, (pid_t)syscall(SYS_gettid), ##__VA_ARGS__)

#endif // LIBMEMLEAK_LOG_H_
