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

#include <errno.h>
#include <paths.h>
#include <string.h>
#include <sys/cdefs.h>
#include <unistd.h>

#define SYSTEM_PREFIX "/system/"

static bool init_is_system() {
  char exe_path[strlen(SYSTEM_PREFIX)];
  ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path));
  if (len != -1 && !strncmp(exe_path, SYSTEM_PREFIX, strlen(SYSTEM_PREFIX))) {
    return true;
  }
  return false;
}

static bool is_system() {
  static bool result = init_is_system();
  return result;
}

const char* __bionic_get_shell_path() {
  static const char* shell_path = is_system() ? __BIONIC_PATH_BSHELL_SYSTEM
                                              : __BIONIC_PATH_BSHELL_VENDOR;
  return shell_path;
}

const char* __bionic_get_default_path() {
  static const char* default_path = is_system() ? __BIONIC_PATH_DEFPATH_SYSTEM
                                                : __BIONIC_PATH_DEFPATH_VENDOR;
  return default_path;
}
