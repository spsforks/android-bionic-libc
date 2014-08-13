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

#include <stdio.h>

class Foo {
public:
    Foo();
    virtual ~Foo();
};


/* This library is used to ensure that static C++ construction
 * and destruction operate normally on dlopen() and dlclose().
 *
 * We use a global variable inside the DLL called "x"
 * and initialize it to 1 in the static C++ constructor.
 *
 * The main program can access its address through dlsym()
 * then later check that it was properly initialized.
 */
extern "C"
{
    int  x = 0;
    void set_y(int *y);
}

Foo::Foo()
{
    x = 1;
}

/* Similarly, the main program can provide the address of
 * an integer, named "y", that will be set to 2 when the
 * static C++ destructor is called on dlclose().
 *
 * This address must be provided through the "set_y" function
 * that can also be resolved through dlsym() by the program.
 */
static int *to_y = NULL;

Foo::~Foo()
{
    if (to_y == NULL) {
        *(int *)NULL = 0; // crash
    }
    *to_y = 2;
}

static Foo  f;

void set_y(int *y)
{
    to_y = y;
}
