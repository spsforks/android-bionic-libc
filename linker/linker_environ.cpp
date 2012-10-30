/*
 * Copyright (C) 2010 The Android Open Source Project
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

#include "linker_environ.h"

#include <linux/auxvec.h>
#include <stddef.h>
#include <unistd.h>

static char** _envp;

/* Returns 1 if 'str' points to a valid environment variable definition.
 * For now, we check that:
 *  - It is smaller than MAX_ENV_LEN (to detect non-zero terminated strings)
 *  - It contains at least one equal sign that is not the first character
 */
static int _is_valid_definition(const char* str) {
    int pos = 0;
    int first_equal_pos = -1;

    // According to its sources, the kernel uses 32*PAGE_SIZE by default
    // as the maximum size for an env. variable definition.
    const int MAX_ENV_LEN = 32*4096;

    if (str == NULL) {
        return 0;
    }

    // Parse the string, looking for the first '=' there, and its size.
    while (pos < MAX_ENV_LEN) {
        if (str[pos] == '\0') {
            break;
        }
        if (str[pos] == '=' && first_equal_pos < 0) {
            first_equal_pos = pos;
        }
        pos++;
    }

    if (pos >= MAX_ENV_LEN) {
        return 0; // Too large.
    }

    if (first_equal_pos < 1) {
        return 0; // No equals sign, or it's the first character.
    }

    return 1;
}

static bool __has_AT_SECURE(unsigned* auxv, bool& is_setuid) {
  // Check auxv for AT_SECURE first to see if program is setuid, setgid,
  // has file caps, or caused a SELinux/AppArmor domain transition.
  for (unsigned* v = auxv; v[0]; v += 2) {
    if (v[0] == AT_SECURE) {
      // Kernel told us whether to enable secure mode.
      is_setuid = v[1];
      return true;
    }
  }
  return false;
}

static void __remove_insecure_environment_variables() {
  // None of these should be allowed in setuid programs.
  static const char* const INSECURE_VARIABLE_NAMES[] = {
      "GCONV_PATH",
      "GETCONF_DIR",
      "HOSTALIASES",
      "LD_AOUT_LIBRARY_PATH",
      "LD_AOUT_PRELOAD",
      "LD_AUDIT",
      "LD_DEBUG",
      "LD_DEBUG_OUTPUT",
      "LD_DYNAMIC_WEAK",
      "LD_LIBRARY_PATH",
      "LD_ORIGIN_PATH",
      "LD_PRELOAD",
      "LD_PROFILE",
      "LD_SHOW_AUXV",
      "LD_USE_LOAD_BIAS",
      "LOCALDOMAIN",
      "LOCPATH",
      "MALLOC_CHECK_",
      "MALLOC_TRACE",
      "NIS_PATH",
      "NLSPATH",
      "RESOLV_HOST_CONF",
      "RES_OPTIONS",
      "TMPDIR",
      "TZDIR",
      NULL
  };
  for (size_t i = 0; INSECURE_VARIABLE_NAMES[i] != NULL; ++i) {
    linker_env_unset(INSECURE_VARIABLE_NAMES[i]);
  }
}

static void __remove_invalid_environment_variables() {
  char** src  = _envp;
  char** dst = _envp;
  for (; src[0] != NULL; ++src) {
    if (!_is_valid_definition(src[0])) {
      continue;
    }
    dst[0] = src[0];
    ++dst;
  }
  dst[0] = NULL;
}

unsigned* linker_env_init(unsigned* vectors, bool& is_setuid) {
  // Store environment pointer - can't be NULL.
  _envp = reinterpret_cast<char**>(vectors);

  // Skip over all environment variable definitions.
  // The end of the environment block is marked by two NULL pointers.
  while (vectors[0] != 0) {
    ++vectors;
  }
  ++vectors;
  unsigned* aux_vectors = vectors;

  __remove_invalid_environment_variables();

  if (!__has_AT_SECURE(aux_vectors, is_setuid)) {
    // Kernel did not provide AT_SECURE - fall back on legacy test.
    is_setuid = (getuid() != geteuid()) || (getgid() != getegid());
  }

  // Sanitize environment if we're loading a setuid program.
  if (is_setuid) {
    __remove_insecure_environment_variables();
  }

  return aux_vectors;
}

/* Check if the environment variable definition at 'envstr'
 * starts with '<name>=', and if so return the address of the
 * first character after the equal sign. Otherwise return NULL.
 */
static char* env_match(char* envstr, const char* name) {
    size_t i = 0;

    while (envstr[i] == name[i] && name[i] != '\0') {
        ++i;
    }

    if (name[i] == '\0' && envstr[i] == '=') {
        return envstr + i + 1;
    }

    return NULL;
}

const char* linker_env_get(const char* name) {
    char** readp = _envp;

    if (name == NULL || name[0] == '\0') {
        return NULL;
    }

    for ( ; readp[0] != NULL; readp++ ) {
        char* val = env_match(readp[0], name);
        if (val != NULL) {
            /* Return NULL for empty strings, or if it is too large */
            if (val[0] == '\0') {
                val = NULL;
            }
            return val;
        }
    }
    return NULL;
}

void linker_env_unset(const char* name) {
    char** readp = _envp;
    char** writep = readp;

    if (name == NULL || name[0] == '\0') {
        return;
    }

    for ( ; readp[0] != NULL; readp++ ) {
        if (env_match(readp[0], name)) {
            continue;
        }
        writep[0] = readp[0];
        writep++;
    }
    /* end list with a NULL */
    writep[0] = NULL;
}
