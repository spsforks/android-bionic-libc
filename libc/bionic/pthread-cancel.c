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

#include <pthread.h>
#include <errno.h>

/* PTHREAD CANCELLATION IS NOT SUPPORTED IN BIONIC !
 *
 * We only provide these functions for the benefit of the GNU Toolchain
 * that probes pthread_cancel() at configure time to determine the thread
 * model. This helps reduce the number of Android-specific patches required
 * to build the Android toolchain binaries.
 */
int  pthread_cancel(pthread_t  thread)
{
    (void)thread;
    /* WE DO NOT SUPPORT THREAD CANCELLATION AT ALL ! */
    return ENOTSUP;
}

int  pthread_setcancelset(int  state, int *oldstate)
{
    (void)state;
    *oldstate = PTHREAD_CANCEL_DISABLE;
    return ENOTSUP;
}

int pthread_setcanceltype(int type, int *oldtype)
{
    (void)type;
    *oldtype = PTHREAD_CANCEL_DEFERRED;
    return ENOTSUP;
}

void pthread_testcancel(void)
{
    /* nothing */
}
