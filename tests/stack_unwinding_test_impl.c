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

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unwind.h>

static _Unwind_Reason_Code __attribute__((noinline)) stop_fn (
    int a __attribute__((__unused__)),
    _Unwind_Action action,
    _Unwind_Exception_Class b __attribute__((__unused__)),
    struct _Unwind_Exception *c __attribute__((__unused__)),
    struct _Unwind_Context *d __attribute__((__unused__)),
    void *e __attribute__((__unused__))
    )
{
    if (action & _UA_END_OF_STACK) {
	printf("abort...\n");
	exit(0);
    }
    return _URC_NO_REASON;
}

static void __attribute__((noinline)) foo1_handler(char* param __attribute__((__unused__))) {
    printf("Success!\n");
    exit(1);
}

static void __attribute__((noinline)) foo2() {
    char* ptr = NULL;
    *ptr = 0xFF;
}

static void __attribute__((noinline)) foo1() {
    char c1 __attribute__((cleanup (foo1_handler)));
    foo2();
}

static void __attribute__((noinline)) sigsegv_handler(int param __attribute__((__unused__))) {
    struct _Unwind_Exception *exception = (struct _Unwind_Exception*)malloc (sizeof (*exception));
    memset (&exception->exception_class, 0, sizeof (exception->exception_class));
    exception->exception_cleanup = 0;

    _Unwind_ForcedUnwind (exception, stop_fn, 0);
}

static void __attribute__((noinline)) set_sigsegv_handler() {
    signal(SIGSEGV, &sigsegv_handler);
}

void do_test() {
    set_sigsegv_handler();
    foo1();
}
