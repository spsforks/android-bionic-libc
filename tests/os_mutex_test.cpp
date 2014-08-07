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
#define __USE_UNIX98  1  /* necessary to define pthread_mutexattr_set/gettype in Linux GLIBC headers. doh ! */
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <gtest/gtest.h>


TEST(os, mutex)
{
    pthread_mutex_t       lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutexattr_t   attr;
    int                   attr_type;

    ASSERT_EQ( pthread_mutexattr_init( &attr ), 0 );

    ASSERT_EQ( pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_NORMAL ), 0 );
    ASSERT_EQ( pthread_mutexattr_gettype( &attr, &attr_type ), 0 );
    ASSERT_TRUE( attr_type == PTHREAD_MUTEX_NORMAL );

    ASSERT_EQ( pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_ERRORCHECK ), 0 );
    ASSERT_EQ( pthread_mutexattr_gettype( &attr, &attr_type ), 0 );
    ASSERT_TRUE( attr_type == PTHREAD_MUTEX_ERRORCHECK );

    ASSERT_EQ( pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE ), 0 );
    ASSERT_EQ( pthread_mutexattr_gettype( &attr, &attr_type ), 0 );
    ASSERT_TRUE( attr_type == PTHREAD_MUTEX_RECURSIVE );

    /* standard mutexes */
    ASSERT_EQ( pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_NORMAL ), 0 );
    ASSERT_EQ( pthread_mutex_init( &lock, &attr ), 0 );
    ASSERT_EQ( pthread_mutex_lock( &lock ), 0 );
    ASSERT_EQ( pthread_mutex_unlock( &lock ), 0 );
    ASSERT_EQ( pthread_mutex_destroy( &lock ), 0 );

    /* error-check mutex */
    ASSERT_EQ( pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_ERRORCHECK ), 0 );
    ASSERT_EQ( pthread_mutex_init( &lock, &attr ), 0 );
    ASSERT_EQ( pthread_mutex_lock( &lock ), 0 );
    ASSERT_EQ( pthread_mutex_lock( &lock ), EDEADLK );
    ASSERT_EQ( pthread_mutex_unlock( &lock ), 0 );
    ASSERT_EQ( pthread_mutex_trylock( &lock ), 0 );
    ASSERT_EQ( pthread_mutex_trylock( &lock ), EDEADLK );
    ASSERT_EQ( pthread_mutex_unlock( &lock ), 0 );
    ASSERT_EQ( pthread_mutex_unlock( &lock ), EPERM );
    ASSERT_EQ( pthread_mutex_destroy( &lock ), 0 );

    /* recursive mutex */
    ASSERT_EQ( pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE ), 0 );
    ASSERT_EQ( pthread_mutex_init( &lock, &attr ), 0 );
    ASSERT_EQ( pthread_mutex_lock( &lock ), 0 );
    ASSERT_EQ( pthread_mutex_lock( &lock ), 0 );
    ASSERT_EQ( pthread_mutex_unlock( &lock ), 0 );
    ASSERT_EQ( pthread_mutex_unlock( &lock ), 0 );
    ASSERT_EQ( pthread_mutex_trylock( &lock ), 0 );
    ASSERT_EQ( pthread_mutex_unlock( &lock ), 0 );
    ASSERT_EQ( pthread_mutex_unlock( &lock ), EPERM );
    ASSERT_EQ( pthread_mutex_destroy( &lock ), 0 );
    
}
