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

#ifndef BIONIC_SYSTRACE_H
#define BIONIC_SYSTRACE_H

#include <cutils/trace.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
 #include <sys/_system_properties.h>

// Tracing class for bionic.  To begin a trace at a specified point:
//   android::BionicScopedTrace("Trace message");
// The trace will end when the contructor goes out of scope, or
// when the destructor is called.

#define WRITE_OFFSET   32

namespace android {
class BionicScopedTrace {
public:
  BionicScopedTrace(const char* message);
  ~BionicScopedTrace();
  static prop_info* pinfo;
  static uint64_t tags;
  static uint32_t serial;
  static int atrace_marker_fd;
};
};

#endif