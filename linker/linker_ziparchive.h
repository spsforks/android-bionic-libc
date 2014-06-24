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

#ifndef __LINKER_ZIPUTIL_H
#define __LINKER_ZIPUTIL_H

#include <stdlib.h>
#include <limits.h>

/*
 * Returns offset to the file data in zip archive or -1
 * in case file was not found or an error.
 *
 * zip entry for the file should satisfy following conditions:
 * 1. It must be page-aligned
 * 2. It must be uncompressed
 *
 * In case of an error the message is stored to error_buf.
 */
off_t find_in_zip(int fd, const char* file_name, char* error_buf, size_t error_buf_size);

#endif // __LINKER_ZIPUTIL_H
