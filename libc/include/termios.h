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
#ifndef _TERMIOS_H_
#define _TERMIOS_H_

#include <sys/cdefs.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/termios.h>

__BEGIN_DECLS

#if __ANDROID_API__ >= 21
// Implemented as static inlines before 21.

#if __ANDROID_API__ >= 21
speed_t cfgetispeed(const struct termios*) __INTRODUCED_IN(21);
speed_t cfgetospeed(const struct termios*) __INTRODUCED_IN(21);
void cfmakeraw(struct termios*) __INTRODUCED_IN(21);
int cfsetispeed(struct termios*, speed_t) __INTRODUCED_IN(21);
int cfsetospeed(struct termios*, speed_t) __INTRODUCED_IN(21);
int tcflow(int, int) __INTRODUCED_IN(21);
int tcflush(int, int) __INTRODUCED_IN(21);
int tcgetattr(int, struct termios*) __INTRODUCED_IN(21);
pid_t tcgetsid(int) __INTRODUCED_IN(21);
int tcsendbreak(int, int) __INTRODUCED_IN(21);
int tcsetattr(int, int, const struct termios*) __INTRODUCED_IN(21);
#endif /* __ANDROID_API__ >= 21 */

#endif


#if __ANDROID_API__ >= 21
int cfsetspeed(struct termios*, speed_t) __INTRODUCED_IN(21);
int tcdrain(int) __INTRODUCED_IN(21);
#endif /* __ANDROID_API__ >= 21 */


__END_DECLS

#include <android/legacy_termios_inlines.h>

#endif /* _TERMIOS_H_ */
