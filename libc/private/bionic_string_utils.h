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

#ifndef _BIONIC_STRING_UTILS_H_
#define _BIONIC_STRING_UTILS_H_

#include <stdbool.h>
#include <string.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

static inline bool ends_with(const char* s1, const char* s2) {
  size_t s1_length = strlen(s1);
  size_t s2_length = strlen(s2);
  if (s2_length > s1_length) {
    return false;
  }
  return memcmp(s1 + (s1_length - s2_length), s2, s2_length) == 0;
}

/*
 * Copy null-terminated |list| of strings to the |buf|.
 * Copy is layed out so that first part of the |buf| is a list of char** and
 * the second part contains a concatenation of values of char* pointed to by elements of
 * |list|. For example the list of {"a", "bc" and "d"} will look like:
 * memory address |0|1|2|3|4|5|6|7|8|9|10|
 * memory value    4 6 9 0 a 0 b c 0 d 0
 *
 * Returns number of bytes used at the beginning of the |buf|.
 */
int copy_list(char **list, char *buf, size_t buflen);

__END_DECLS

#endif // _BIONIC_STRING_UTILS_H_
