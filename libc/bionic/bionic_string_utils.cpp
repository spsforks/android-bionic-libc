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

#include "private/bionic_string_utils.h"

int copy_list(char **list, char *buf, size_t buflen) {
  if (list == NULL) {
    *((char**)buf) = NULL;
    return sizeof(char**);
  }
  size_t list_len = 0;
  size_t required_space = 0;
  while (list[list_len] != NULL) {
    list_len++;
    // We need a space for NULL at the end so strlen + 1.
    required_space += strlen(list[list_len]) + 1;
  }
  // We need to count NULL at the end of the list so list_len + 1.
  required_space += (list_len + 1) * sizeof(char*);
  if (buflen < required_space) {
    // Not enough space in the buffer.
    return -1;
  }
  char **list_copy_last_elem = (char **)buf;
  char *elems_content = buf + (list_len + 1) * sizeof(char*);
  // Preconditions:
  // * |list_copy_last_elem| points to the memory where the next element of a list should be copied.
  //   Each element of a list is a pointer to string (char*).
  // * |elems_content| points to the memory where the string pointed to by the next element of
  //   a list should be copied.
  while (list != NULL) {
    // We want to copy \0 at the end as well
    int elem_len = strlen(*list) + 1;
    memcpy(elems_content, *list, elem_len);
    *list_copy_last_elem = elems_content;
    list++;
    list_copy_last_elem++;
    elems_content += elem_len;
  }
  *list_copy_last_elem = NULL;
  return required_space;
}
