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
#include "linker_util.h"
#include "linker.h"
#include "linker_debug.h"


#if defined(_BIONIC_LD_PLATFORM)
#define _Q(x) #x
#define QUOTE(x) _Q(x)
#define LDPLATFORM QUOTE(_BIONIC_LD_PLATFORM)
#else
#error "Please specify appropriate -D_BIONIC_LD_PLATFORM"
#endif

#define LDTOKEN_ORIGIN "ORIGIN"
#define LDTOKEN_PLATFORM "PLATFORM"
#define LDTOKEN_LIB "LIB"

/*
 * This function parses DT_RPATH/DT_RUNPATH and expands following tokens
 * $ORIGIN - is replaced with origin_path
 * $PLATFORM - with processor type (arm/arm64/...)
 * $LIB - with /system/lib (or /system/lib64 for 64bit arch)
 */
bool parse_so_path_replace_token(char* path, size_t path_size, const char* path_with_tokens,
    const char* origin_path, const char* lib_path, int parse_flags) {
  if (path == NULL) {
    return false;
  }

  char* dst = path;
  const char* src = path_with_tokens;
  const char* token_start;

  while( (token_start = strchr(src, '$')) != NULL ) {
    const char* token_end;
    const char* token = NULL;
    const char* replacement = NULL;

    const char* name_start = token_start+1;
    if (token_start[1] == '{')
      ++name_start;
    switch (*name_start) {
    case 'O':
      if ((parse_flags & PARSE_LDTOKEN_ORIGIN) != 0 && token_start == src) {
        token = LDTOKEN_ORIGIN;
        replacement = origin_path;
      }
      break;
    case 'L':
      if ((parse_flags & PARSE_LDTOKEN_LIB) != 0 && token_start == src) {
        token = LDTOKEN_LIB;
        replacement = lib_path;
      }
      break;
    case 'P':
      if ((parse_flags & PARSE_LDTOKEN_PLATFORM) != 0) {
        token = LDTOKEN_PLATFORM;
        replacement = LDPLATFORM;
      }
      break;
    }

    if (token != NULL && replacement != NULL && *replacement != '\0'
        && strstr(name_start, token) == name_start
        && (token_start[1] != '{' || name_start[strlen(LDTOKEN_ORIGIN)] == '}')) {

      token_end = name_start + strlen(token) + (token_start[1] == '{' ? 1 : 0);
      size_t replacement_len = strlen(replacement);
      char* new_dst = dst + (token_start-src) + replacement_len;

      if ( static_cast<size_t>(new_dst-path) >= (path_size-1) ) {
        path[0] = '\0';
        PRINT("Replacement path for '%s' is too long\n", path_with_tokens);
        return false;
      }

      size_t len = token_start - src;
      strncpy(dst, src, token_start - src);
      dst += len;
      strcpy(dst, replacement);
      dst += replacement_len;

      src = token_end;
    } else {
      if (static_cast<size_t>(dst-path) >= (path_size-2)) {
        path[0] = '\0';
        PRINT("Replacement path for '%s' is too long\n", path_with_tokens);
        return false;
      }
      *dst++ = *src++;
    }
  }

  size_t len = strlen(src); // the rest of the path
  if (static_cast<size_t>(dst-path) >= (path_size - 1 - len)) {
    path[0] = '\0';
    PRINT("Replacement path for '%s' is too long\n", path_with_tokens);
    return false;
  }

  strcpy(dst, src);

  return true;
}

bool parse_so_paths(const char* so_path, const char** array, char* buf, size_t buf_size, size_t max_count, const char* origin_path, const char* lib_path) {
  if (so_path == NULL) {
    return false;
  }

  size_t i = 0;
  char* dst = buf;
  const char* src = so_path;

  while (i < max_count && src != NULL) {
    const char* end = strchr(src, ':');
    size_t len = end != NULL ? (end - src) : strlen(src);
    size_t bytes_left_in_buf = buf_size - (dst - buf);
    // check boundaries
    if (bytes_left_in_buf < len - 1) {
      PRINT("Path is too long: %s\n", so_path);
      return false;
    }

    char path_with_tokens[bytes_left_in_buf];
    if (len > 0) {
      strncpy(path_with_tokens, src, len);
      path_with_tokens[len] = '\0';

      // looking for tokens
      if (parse_so_path_replace_token(dst, bytes_left_in_buf, path_with_tokens, origin_path, lib_path)) {
        array[i++] = dst;
        dst += strlen(dst) + 1;
      } else {
        INFO("Skipping %s\n", path_with_tokens);
      }
    }

    src = end == NULL ? NULL : end + 1;
  }
  return true;
}

