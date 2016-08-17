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
#include <sys/shm.h>

#include "TemporaryFile.h"

TEST(sys_shm, smoke) {
  // Create a segment.
  TemporaryDir dir;
  key_t key = ftok(dir.dirname, 1);
  int id = shmget(key, 1234, IPC_CREAT|0666);
  ASSERT_NE(id, -1);

  // Check segment info.
  shmid_ds ds;
  memset(&ds, 0, sizeof(ds));
  ASSERT_EQ(0, shmctl(id, IPC_STAT, &ds));
  ASSERT_EQ(1234U, ds.shm_segsz);

  // Attach.
  void* p = shmat(id, 0, SHM_RDONLY);
  ASSERT_NE(p, nullptr);

  // Detach.
  ASSERT_EQ(0, shmdt(p));

  // Destroy the segment.
  ASSERT_EQ(0, shmctl(id, IPC_RMID, 0));
}
