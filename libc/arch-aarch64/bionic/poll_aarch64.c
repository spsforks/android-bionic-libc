/*
 * poll_aarch64.c
 *
 * Copyright (C) 2013 ARM Ltd.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * *Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * *Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
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

#include <sys/time.h>
#include <poll.h>
#include <signal.h>

extern int ppoll(struct pollfd*, unsigned int, struct timespec *,
                 const sigset_t *, size_t);

int poll(struct pollfd *fds, nfds_t nfds, long timeout)
{

    /*
     * Specifying a negative value in timeout means an infinite timeout.
     * Passing a NULL tm_n to ppoll() will cause the syscall to block
     * indefinitely.
     */
    if (timeout >= 0) {
        /*
         * timeout is zero or positive, so use a non-NULL tm_n
         */
        struct timespec tm_n;
        tm_n.tv_sec = timeout/1000;
        tm_n.tv_nsec = (((long)timeout)*1000000L) - (tm_n.tv_sec*1000000000L);
        return ppoll(fds, nfds, &tm_n, NULL, sizeof(sigset_t));
    }
    return ppoll(fds, nfds, NULL, NULL, sizeof(sigset_t));
}
