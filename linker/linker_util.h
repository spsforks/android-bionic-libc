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

#ifndef __LINKER_UTIL_H
#define __LINKER_UTIL_H

#include <stdlib.h>

#define PARSE_LDTOKEN_LIB (0x1)
#define PARSE_LDTOKEN_PLATFORM (0x2)
#define PARSE_LDTOKEN_ORIGIN (0x4)

#define PARSE_LDTOKEN_ALL (PARSE_LDTOKEN_LIB | PARSE_LDTOKEN_PLATFORM | PARSE_LDTOKEN_ORIGIN)

bool parse_so_path_replace_token(char* path, size_t path_size, const char* path_with_tokens,
    const char* origin_path, const char* lib_path, int parse_flags = PARSE_LDTOKEN_ALL);

bool parse_so_paths(const char* so_path, const char** array, char* buf, size_t buf_size,
    size_t max_count, const char* origin_path, const char* lib_path);

#endif // __LINKER_UTIL_H
