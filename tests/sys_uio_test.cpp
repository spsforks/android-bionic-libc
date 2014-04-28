/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include "TemporaryFile.h"

#include <sys/uio.h>
#include <fcntl.h>

static TemporaryFile tf;
const static std::string TEMPFILE_NAME(tf.filename);

const static std::string TEST_CONTENT[] = {
    "99 bottles of beer on the wall, 99 bottles of beer.\n",
    "Take one down and pass it around, 98 bottles of beer on the wall\n",
    "98 bottles of beer on the wall, 98 bottles of beer.\n",
    "Take one down and pass it around, 97 bottles of beer on the wall.\n",
    "97 bottles of beer on the wall, 97 bottles of beer.\n",
    "Take one down and pass it around, 96 bottles of beer on the wall.\n",
    "96 bottles of beer on the wall, 96 bottles of beer.\n",
    "Take one down and pass it around, 95 bottles of beer on the wall.\n",
    "95 bottles of beer on the wall, 95 bottles of beer.\n",
    "Take one down and pass it around, 94 bottles of beer on the wall.\n",
};

/*
 * Array size constant expressions
 */

#define array_size(A) sizeof(array_to_sizeof(A))
#define concat_size(A, B) array_size(A)+array_size(B)-1

template <size_t N>
struct N_to_sizeof { typedef char type[N]; };

template <typename T, size_t N>
typename N_to_sizeof<N>::type& array_to_sizeof(T(&)[N]);


TEST(sys_uio, writev_readv)
{
  struct iovec iov[array_size(TEST_CONTENT)];

  { SCOPED_TRACE("writev()");
    int fd = open(TEMPFILE_NAME.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);
    ASSERT_FALSE( fd == -1 );

    // Populate data
    for (unsigned int j = 0; j < array_size(TEST_CONTENT); ++j)
      {
	iov[j].iov_base = const_cast <char *> (TEST_CONTENT[j].c_str());
	iov[j].iov_len  = TEST_CONTENT[j].size();
      }

    ssize_t nwrite = writev(fd, iov, array_size(iov));
    EXPECT_FALSE( nwrite == -1 );
    ASSERT_FALSE(close(fd));
  }

  { SCOPED_TRACE("readv()");
    char bufA[128];
    char bufB[64];

    int fd = open(TEMPFILE_NAME.c_str(), O_RDONLY);
    ASSERT_FALSE( fd == -1 );

    iov[0].iov_base = bufA;
    iov[0].iov_len = array_size(bufA);
    iov[1].iov_base = bufB;
    iov[1].iov_len = array_size(bufB);

    ssize_t nread = readv(fd, iov, 2);
    EXPECT_FALSE( nread == -1 );
    EXPECT_EQ('9', bufA[1]);
    EXPECT_EQ('9', bufB[21]);
    ASSERT_FALSE(close(fd));
  }

  remove(TEMPFILE_NAME.c_str());
}
