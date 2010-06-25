/*
 * Copyright (C) 2008 The Android Open Source Project
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
#include <unistd.h>
#include "pthread_internal.h"

extern int  __fork(void);

int  fork(void)
{
    int  ret;
    pthread_atfork_t *cursor;

    /* Posix mandates that the timers of a fork child process be
     * disarmed, but not destroyed. To avoid a race condition, we're
     * going to stop all timers now, and only re-start them in case
     * of error, or in the parent process
     */
    __timer_table_start_stop(1);

    /* Call pthread_atfork() prepare handlers.
     * NOTE: Posix states that the prepare handlers should be called in the reverse order of the parent/child
     *       handlers, so we iterate using prev instead of next
     */
    for (cursor = pthread_atfork_handlers.prev; cursor != &pthread_atfork_handlers; cursor = cursor->prev) {
        if (cursor->prepare != NULL) {
            cursor->prepare();
        }
    }

    ret = __fork();
    if (ret == 0) { /* child process */
        /* Call pthread_atfork() child handlers */
        for (cursor = pthread_atfork_handlers.next; cursor != &pthread_atfork_handlers; cursor = cursor->next) {
            if (cursor->child != NULL) {
                cursor->child();
            }
        }

        /*
         * Newly created process must update cpu accounting.
         * Call cpuacct_add passing in our uid, which will take
         * the current task id and add it to the uid group passed
         * as a parameter.
         */
        cpuacct_add(getuid());
    } else  {       /* parent process */
        /* Call pthread_atfork() parent handlers */
        for (cursor = pthread_atfork_handlers.next; cursor != &pthread_atfork_handlers; cursor = cursor->next) {
            if (cursor->parent != NULL) {
                cursor->parent();
            }
        }
        __timer_table_start_stop(0);
    }

    return ret;
}
