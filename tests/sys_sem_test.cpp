/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <errno.h>
#include <sys/sem.h>

#include "TemporaryFile.h"

TEST(sys_sem, smoke) {
  // Create a semaphore.
  TemporaryDir dir;
  key_t key = ftok(dir.dirname, 1);
  int id = semget(key, 1, IPC_CREAT|0666);
  ASSERT_NE(id, -1);

  // Check semaphore info.
  semid_ds ds;
  memset(&ds, 0, sizeof(ds));
  ASSERT_EQ(0, semctl(id, 0, IPC_STAT, &ds));
  ASSERT_EQ(1U, ds.sem_nsems);

  ASSERT_EQ(0, semctl(id, 0, GETVAL));

  // Increment.
  sembuf ops[] = {{ .sem_num = 0, .sem_op = 1, .sem_flg = 0 }};
  ASSERT_EQ(0, semop(id, ops, 1));
  ASSERT_EQ(1, semctl(id, 0, GETVAL));

  // Decrement.
  ops[0] = { .sem_num = 0, .sem_op = -1, .sem_flg = 0 };
  ASSERT_EQ(0, semop(id, ops, 1));
  ASSERT_EQ(0, semctl(id, 0, GETVAL));

  // Destroy the semaphore.
  ASSERT_EQ(0, semctl(id, 0, IPC_RMID));
}
