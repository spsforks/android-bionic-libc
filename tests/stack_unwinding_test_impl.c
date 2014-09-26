/*
 * Copyright (C) 2013 The Android Open Source Project
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

/*
 * Contributed by: Intel Corporation
 */

#include <dlfcn.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unwind.h>

#define noinline __attribute__((__noinline__))

#ifndef __unused
#define __unused __attribute__((__unused__))
#endif

static noinline _Unwind_Reason_Code cleanup_unwind_fn(int a __unused,
    _Unwind_Action action,
    _Unwind_Exception_Class b __unused,
    struct _Unwind_Exception* c __unused,
    struct _Unwind_Context* ctx __unused,
    void* e __unused) {

  static int counter;

  if ((action & _UA_END_OF_STACK) != 0) {
    abort(); // We reached the end of the stack without executing foo_cleanup (which would have exited). Test failed.
  } else {
    // We just check that backtrace has more than 3 frames which is fine enough estimation that unwind is done through signal frame:
    // cleanup_sigsegv_handler() -> __restore[_rt]() -> crash() -> unwind_through_frame_with_cleanup_function() -> TestBody()
    // We used to use attribute((cleanup)) function to make sure we go through particular function,
    // but it's appeared to be not reliable in this case.
    // printf("%d\n",counter);
    if (counter > 3)
       exit(42);
  }
  counter++;
  return _URC_NO_REASON;
}

static void noinline crash() {
  *((int *)1) = 0;
}

static void noinline cleanup_sigsegv_handler(int param __unused) {
  struct _Unwind_Exception* exception = (struct _Unwind_Exception*) calloc(1, sizeof(*exception));
  _Unwind_ForcedUnwind(exception, cleanup_unwind_fn, 0);
}

void unwind_through_frame_with_sigsegv() {
  signal(SIGSEGV, &cleanup_sigsegv_handler);
  crash();
}
