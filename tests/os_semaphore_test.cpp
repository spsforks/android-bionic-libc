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
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <gtest/gtest.h>

/* a simple semaphore test, using three threads
 *
 *  a semaphore is initialized with a value of 1
 *
 *  Thread 1, 2 and 3 start at the same time
 *
 *  Thread 1 takes the semaphore, then sleeps for 2 seconds, then post the semaphore
 *  Thread 2 sleeps for 1 second, then waits the semaphore, sleeps for 2 seconds, then post the semaphoe
 *  Thread 3 sleeps 3 seconds, waits for the semaphore
 */

static  sem_t   semaphore;

static void*
_thread1(void *__u __attribute__((unused)))
{
    if(sem_wait( &semaphore ) < 0)
    {
        ADD_FAILURE();
    }
    
    sleep( 2 );
    
    if(sem_post( &semaphore ) < 0)
    {
        ADD_FAILURE();
    }

    return NULL;
}

static void*
_thread2(void *__u __attribute__((unused)))
{
    sleep(1);
    
    if ( sem_wait( &semaphore ) < 0 ){

        ADD_FAILURE();
    }
    
    sleep( 2 );
    if ( sem_post( &semaphore ) < 0 ){
        ADD_FAILURE();
    }
    
    return NULL;
}


static void*
_thread3(void *__u __attribute__((unused)))
{
    sleep(3);
    if ( sem_wait( &semaphore ) < 0 ){
        ADD_FAILURE();
    }
    
    return NULL;
}

typedef void*  (*thread_func)(void*);

static const  thread_func  thread_routines[] =
{
    &_thread1,
    &_thread2,
    &_thread3
};

TEST(os, semaphore)
{
    pthread_t   t[3];
    int         nn;

    ASSERT_TRUE( sem_init( &semaphore, 0, 1 ) >= 0 );

    for ( nn = 0; nn < 3; nn++ ) {
        ASSERT_TRUE(pthread_create( &t[nn], NULL, thread_routines[nn], NULL ) >= 0 );
    }
    sleep( 5 );
}
