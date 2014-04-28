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

#include <sys/atomics.h>

static int valGlobal = 200;
static const int VALS[] = {346, 456, 565, 67, 34, 354, 565, 12, 324, 63, 234, 673};

/*
 * Array size constant expressions
 */

#define array_size(A) sizeof(array_to_sizeof(A))
#define concat_size(A, B) array_size(A)+array_size(B)-1

template <size_t N>
struct N_to_sizeof { typedef char type[N]; };

template <typename T, size_t N>
typename N_to_sizeof<N>::type& array_to_sizeof(T(&)[N]);


TEST(sys_atomics, cas_single)
{
  { SCOPED_TRACE("__atomic_cmpxchg() CAS");
    int valOld = 200;
    int valNew = 300;

    __atomic_cmpxchg(valOld, valNew, &valGlobal);
    ASSERT_EQ(valNew, valGlobal);
  }

  // Reset global value
  valGlobal = 200;
}

TEST(sys_atomics, cas_multi)
{
  { SCOPED_TRACE("foo() bar");
    int status;
    pid_t pid = fork();
    ASSERT_FALSE( pid == -1 );

    if (pid == 0)
      { // Child process
	int valOld, valNew;

	for (unsigned int j = 0; j < array_size(VALS) * 10000; ++j)
	  {
	    if ((j % array_size(VALS)) == 5)
	      valGlobal = 15;
	    valOld = valGlobal;
	    valNew = VALS[ j % array_size(VALS) ];

	    __atomic_cmpxchg(valOld, valNew, &valGlobal);
	    if ((valNew != valGlobal) && (valOld != valGlobal))
	      exit(1);
	  }
	exit(0);
      }
    else
      { // Parent process
	int valOld, valNew;

	for (unsigned int j = 0; j < array_size(VALS) * 10000; ++j)
	  {
	    if ((j % array_size(VALS)) == 5)
	      valGlobal = 15;
	    valOld = valGlobal;
	    valNew = VALS[ j % array_size(VALS) ];

	    __atomic_cmpxchg(valOld, valNew, &valGlobal);
	    ASSERT_TRUE((valNew == valGlobal) || (valOld == valGlobal));
	  }

	// Wait for child
	wait(&status);
	ASSERT_EQ(0, WEXITSTATUS(status));
      }
  }
}

TEST(sys_atomics, swap)
{
  { SCOPED_TRACE("__atomic_swap()");
    int valNew = 300;
    valGlobal = 0;

    __atomic_swap(valNew, &valGlobal);
    ASSERT_EQ(valNew, valGlobal);
  }
}

TEST(sys_atomics, inc)
{
  { SCOPED_TRACE("__atomic_inc()");
    valGlobal = 0xFE;

    __atomic_inc(&valGlobal);
    ASSERT_EQ(0xFF, valGlobal);
  }
}

TEST(sys_atomics, dec)
{
  { SCOPED_TRACE("__atomic_dec()");
    valGlobal = 0x100;

    __atomic_dec(&valGlobal);
    ASSERT_EQ(0xFF, valGlobal);
  }
}
