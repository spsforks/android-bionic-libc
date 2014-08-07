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

/* this program is used to check that static C++ destructors are
 * properly called when dlclose() is called. We do this by using
 * a helper C++ shared library.
 *
 * See libdlclosetest1.cpp for details.
 */
#include <dlfcn.h>
#include <stdio.h>
#include <gtest/gtest.h>


static int
check_library(const char*  libname)
{
    void*  lib = dlopen(libname, RTLD_NOW);
    int*   to_x;
    void  (*set_y)(int *);
    int    y = 0;

    if (lib == NULL){
       ADD_FAILURE();
    }

    to_x =(int *)dlsym(lib, "x");
    if (to_x == NULL){
   
       ADD_FAILURE();
    }

    if (*to_x != 1){

       ADD_FAILURE();
    }

    set_y =(void (*)(int*))dlsym(lib, "set_y");
    if (set_y == NULL){
       
      printf("error %s\n",dlerror());
       ADD_FAILURE();
    }

    y = 0;
    (*set_y)(&y);

    if (dlclose(lib) < 0){
   
       ADD_FAILURE();
    }

    if (y != 2){

       ADD_FAILURE();
    }
    
    return 0;
}

TEST(os, dlclose_destruction)
{
    /* Testing static C++ construction/destruction */
   ASSERT_EQ(0,check_library("libdlclosetest1.so"));

   ASSERT_EQ(0,check_library("libdlclosetest2.so"));

}
