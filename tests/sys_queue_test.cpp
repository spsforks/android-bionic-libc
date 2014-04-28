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

/* ===
 *
 * Tests for lists and queues helpers in libc.
 *
 * http://linux.die.net/man/3/queue
 * > Not in POSIX.1-2001. Present on the BSDs.
 * > The queue functions first appeared in 4.4BSD.
 *
 * Features covered:
 *   Single-linked lists
 *   Double-linked lists
 *   Tail queues
 *   Single-linked tail queues
 *
 * Statements covered:
 *   SLIST_ENTRY()
 *   SLIST_HEAD()
 *   SLIST_HEAD_INITIALIZER()
 *   SLIST_INIT()
 *   SLIST_INSERT_HEAD()
 *   SLIST_INSERT_AFTER()
 *   SLIST_FOREACH()
 *   SLIST_EMPTY()
 *   SLIST_FIRST()
 *   SLIST_NEXT()
 *
 *   LIST_ENTRY()
 *   LIST_HEAD()
 *   LIST_HEAD_INITIALIZER()
 *   LIST_INIT()
 *   LIST_INSERT_HEAD()
 *   LIST_INSERT_AFTER()
 *   LIST_FOREACH()
 *   LIST_EMPTY()
 *   LIST_FIRST()
 *   LIST_NEXT()
 *
 *   TAILQ_ENTRY()
 *   TAILQ_HEAD()
 *   TAILQ_HEAD_INITIALIZER()
 *   TAILQ_INIT()
 *   TAILQ_INSERT_HEAD()
 *   TAILQ_INSERT_TAIL()
 *   TAILQ_INSERT_BEFORE()
 *   TAILQ_INSERT_AFTER()
 *   TAILQ_FOREACH()
 *   TAILQ_FOREACH_REVERSE()
 *   TAILQ_EMPTY()
 *   TAILQ_FIRST()
 *   TAILQ_PREV()
 *   TAILQ_NEXT()
 *   TAILQ_LAST()
 *
 *   STAILQ_ENTRY()
 *   STAILQ_HEAD()
 *   STAILQ_HEAD_INITIALIZER()
 *   STAILQ_INIT()
 *   STAILQ_INSERT_HEAD()
 *   STAILQ_INSERT_TAIL()
 *   STAILQ_INSERT_AFTER()
 *   STAILQ_FOREACH()
 *   STAILQ_EMPTY()
 *   STAILQ_FIRST()
 *   STAILQ_NEXT()
 *
 *   SIMPLEQ_ENTRY()
 *   SIMPLEQ_HEAD()
 *   SIMPLEQ_HEAD_INITIALIZER()
 *   SIMPLEQ_INIT()
 *   SIMPLEQ_INSERT_HEAD()
 *   SIMPLEQ_INSERT_TAIL()
 *   SIMPLEQ_INSERT_AFTER()
 *   SIMPLEQ_FOREACH()
 *   SIMPLEQ_EMPTY()
 *   SIMPLEQ_FIRST()
 *   SIMPLEQ_NEXT()
 */

/*
 * Contributed by: Intel Corporation
 */

#include <gtest/gtest.h>

#include <sys/queue.h>

const static int TEST_VAL_1  = 11;
const static int TEST_VAL_2  = 22;
const static int TEST_VAL_3  = 33;
const static int TEST_VAL_4  = 44;
const static int TEST_VAL_12 = 12;
const static int TEST_VAL_23 = 23;

// Single-linked list
struct slEntry
{
    int number;
    SLIST_ENTRY(slEntry) entries;
};

// Double-linked list
struct lEntry
{
    int number;
  LIST_ENTRY(lEntry) entries;
};

// Tail queue
struct tqEntry
{
  int number;
  TAILQ_ENTRY(tqEntry) entries;
};

// Single-linked tail queue
struct stqEntry
{
  int number;
  STAILQ_ENTRY(stqEntry) entries;
};

// Simple queue
struct smplqEntry
{
  int number;
  SIMPLEQ_ENTRY(smplqEntry) entries;
};

// Circle queue
struct cirqEntry
{
  int number;
  CIRCLEQ_ENTRY(cirqEntry) entries;
};

/*
 * This test checks that
 *   single-linked list works properly.
 */
TEST(sys_queue, single_list)
{
  struct slEntry *node1;   // Node 1
  struct slEntry *node2;   // Node 2
  struct slEntry *node3;   // Node 3
  struct slEntry *node12;  // Node between node1 and node2

  // Initialize head of the single-linked list
  SLIST_HEAD(slisthead, slEntry) nodeHead = SLIST_HEAD_INITIALIZER(nodeHead);
  SLIST_INIT(&nodeHead);

  { SCOPED_TRACE("check init state");
    ASSERT_FALSE( SLIST_EMPTY(&nodeHead) == 0 );
    ASSERT_FALSE( SLIST_FIRST(&nodeHead) != NULL );
  }

  { SCOPED_TRACE("inserts");
    // = Node 1
    node1 = (slEntry *) malloc(sizeof(struct slEntry));
    ASSERT_FALSE( node1 == NULL );
    node1->number = TEST_VAL_1;

    SLIST_INSERT_HEAD(&nodeHead, node1, entries);

    // = Node 2
    node2 = (slEntry *) malloc(sizeof(struct slEntry));
    ASSERT_FALSE( node2 == NULL );
    node2->number = TEST_VAL_2;

    SLIST_INSERT_AFTER(node1, node2, entries);

    // = Node 3
    node3 = (slEntry *) malloc(sizeof(struct slEntry));
    ASSERT_FALSE( node3 == NULL );
    node3->number = TEST_VAL_3;

    SLIST_INSERT_AFTER(node2, node3, entries);
  }

  { SCOPED_TRACE("inject");
    node12 = (slEntry *) malloc(sizeof(struct slEntry));
    ASSERT_FALSE( node12 == NULL );
    node12->number = TEST_VAL_12;

    SLIST_INSERT_AFTER(node1, node12, entries);
  }

  { SCOPED_TRACE("list iterating");
    int counter = 0;
    struct slEntry *carry;  // Iterative node

    ASSERT_FALSE( SLIST_EMPTY(&nodeHead) );

    SLIST_FOREACH(carry, &nodeHead, entries)
      {
	switch (counter)
	  {
	  case 0: {
	    EXPECT_TRUE( carry == node1 );
	    break;
	  }
	  case 1: {
	    EXPECT_TRUE( carry == node12 );
	    break;
	  }
	  case 2: {
	    EXPECT_TRUE( carry == node2 );
	    break;
	  }
	  case 3: {
	    EXPECT_TRUE( carry == node3 );
	    break;
	  }
	  default: {
	    FAIL();
	  }
	  }
	counter++;
      } // END - SLIST_FOREACH

    EXPECT_EQ(4, counter);
  }

  { SCOPED_TRACE("check relations");
    EXPECT_TRUE( SLIST_FIRST(&nodeHead) == node1 );
    EXPECT_TRUE( SLIST_NEXT(node1, entries) == node12 );
    EXPECT_TRUE( SLIST_NEXT(node3, entries) == NULL );
    EXPECT_TRUE( SLIST_NEXT(SLIST_FIRST(&nodeHead), entries) == node12 );
  }

  { SCOPED_TRACE("remove node");
    SLIST_REMOVE(&nodeHead, node12, slEntry, entries);
    EXPECT_TRUE( SLIST_NEXT(SLIST_FIRST(&nodeHead), entries) == node2 );
  }

  { SCOPED_TRACE("remove head");
    SLIST_REMOVE_HEAD(&nodeHead, entries);
    EXPECT_TRUE( SLIST_FIRST(&nodeHead) == node2 );

    SLIST_REMOVE_HEAD(&nodeHead, entries);
    EXPECT_TRUE( SLIST_FIRST(&nodeHead) == node3 );

    SLIST_REMOVE_HEAD(&nodeHead, entries);
    ASSERT_FALSE( SLIST_EMPTY(&nodeHead) == 0 );
  }

  // Cleaning up
  free(node1);
  free(node2);
  free(node3);
  free(node12);
}

/*
 * This test checks that
 *   double-linked list works properly.
 */
TEST(sys_queue, double_list)
{
  struct lEntry *node1;   // Node 1
  struct lEntry *node2;   // Node 2
  struct lEntry *node3;   // Node 3
  struct lEntry *node12;  // Node between node1 and node2

  // Initialize head of the double-linked list
  LIST_HEAD(listhead, lEntry) nodeHead = LIST_HEAD_INITIALIZER(nodeHead);
  LIST_INIT(&nodeHead);

  { SCOPED_TRACE("check init state");
    ASSERT_FALSE( LIST_EMPTY(&nodeHead) == 0 );
    ASSERT_FALSE( LIST_FIRST(&nodeHead) != NULL );
  }

  { SCOPED_TRACE("inserts");
    // = Node 1
    node1 = (lEntry *) malloc(sizeof(struct lEntry));
    ASSERT_FALSE( node1 == NULL );
    node1->number = TEST_VAL_1;

    LIST_INSERT_HEAD(&nodeHead, node1, entries);

    // = Node 2
    node2 = (lEntry *) malloc(sizeof(struct lEntry));
    ASSERT_FALSE( node2 == NULL );
    node2->number = TEST_VAL_2;

    LIST_INSERT_AFTER(node1, node2, entries);

    // = Node 3
    node3 = (lEntry *) malloc(sizeof(struct lEntry));
    ASSERT_FALSE( node3 == NULL );
    node3->number = TEST_VAL_3;

    LIST_INSERT_AFTER(node2, node3, entries);
  }

  { SCOPED_TRACE("inject");
    node12 = (lEntry *) malloc(sizeof(struct lEntry));
    ASSERT_FALSE( node12 == NULL );
    node12->number = TEST_VAL_12;

    LIST_INSERT_AFTER(node1, node12, entries);
  }

    { SCOPED_TRACE("list iterating");
      int counter = 0;
      struct lEntry *carry;  // Iterative node

      ASSERT_FALSE( LIST_EMPTY(&nodeHead) );

      LIST_FOREACH(carry, &nodeHead, entries)
	{
	  switch (counter)
	    {
	    case 0: {
	      EXPECT_TRUE( carry == node1 );
	      break;
	    }
	    case 1: {
	      EXPECT_TRUE( carry == node12 );
	      break;
	    }
	    case 2: {
	      EXPECT_TRUE( carry == node2 );
	      break;
	    }
	    case 3: {
	      EXPECT_TRUE( carry == node3 );
	      break;
	    }
	    default: {
	      FAIL();
	    }
	    }
	  counter++;
	} // END - LIST_FOREACH

      EXPECT_EQ(4, counter);
    }

    { SCOPED_TRACE("check relations");
      EXPECT_TRUE( LIST_FIRST(&nodeHead) == node1 );
      EXPECT_TRUE( LIST_NEXT(node1, entries) == node12 );
      EXPECT_TRUE( LIST_NEXT(node3, entries) == NULL );
      EXPECT_TRUE( LIST_NEXT(LIST_FIRST(&nodeHead), entries) == node12 );
    }

    { SCOPED_TRACE("remove nodes");
      LIST_REMOVE(node12, entries);
      EXPECT_TRUE( LIST_NEXT(LIST_FIRST(&nodeHead), entries) == node2 );

      LIST_REMOVE(node1, entries);
      EXPECT_TRUE( LIST_FIRST(&nodeHead) == node2 );

      LIST_REMOVE(node2, entries);
      EXPECT_TRUE( LIST_FIRST(&nodeHead) == node3 );

      LIST_REMOVE(node3, entries);
      EXPECT_TRUE( LIST_EMPTY(&nodeHead) );
    }

    // Cleaning up
    free(node1);
    free(node2);
    free(node3);
    free(node12);
}

/*
 * This test checks that
 *   tail queue works properly.
 */
TEST(sys_queue, tail_queue)
{
    struct tqEntry *node1;   // Node 1
    struct tqEntry *node2;   // Node 2
    struct tqEntry *node3;   // Node 3
    struct tqEntry *node4;   // Node 4
    struct tqEntry *node12;  // Node between node1 and node2

    // Initialize head of the tail queue
    TAILQ_HEAD(tailhead, tqEntry) nodeHead = TAILQ_HEAD_INITIALIZER(nodeHead);
    TAILQ_INIT(&nodeHead);

    { SCOPED_TRACE("check init state");
      ASSERT_FALSE( TAILQ_EMPTY(&nodeHead) == 0 );
      ASSERT_FALSE( TAILQ_FIRST(&nodeHead) != NULL );
      ASSERT_FALSE( TAILQ_LAST(&nodeHead, tailhead) != NULL );
    }

    { SCOPED_TRACE("inserts");
      // = Node 1
      node1 = (tqEntry *) malloc(sizeof(struct tqEntry));
      ASSERT_FALSE( node1 == NULL );
      node1->number = TEST_VAL_1;

      TAILQ_INSERT_HEAD(&nodeHead, node1, entries);

      // = Node 2
      node2 = (tqEntry *) malloc(sizeof(struct tqEntry));
      ASSERT_FALSE( node2 == NULL );
      node2->number = TEST_VAL_2;

      TAILQ_INSERT_TAIL(&nodeHead, node2, entries);

      // = Node 3
      node3 = (tqEntry *) malloc(sizeof(struct tqEntry));
      ASSERT_FALSE( node3 == NULL );
      node3->number = TEST_VAL_3;

      TAILQ_INSERT_TAIL(&nodeHead, node3, entries);

      // = Node 4
      node4 = (tqEntry *) malloc(sizeof(struct tqEntry));
      ASSERT_FALSE( node4 == NULL );
      node4->number = TEST_VAL_4;

      TAILQ_INSERT_AFTER(&nodeHead, node2, node4, entries);
    }

    { SCOPED_TRACE("inject");
      node12 = (tqEntry *) malloc(sizeof(struct tqEntry));
      ASSERT_FALSE( node12 == NULL );
      node12->number = TEST_VAL_12;

      TAILQ_INSERT_BEFORE(node2, node12, entries);
    }

    { SCOPED_TRACE("queue foreach");
      int counter = 0;
      struct tqEntry *carry;  // Iterative node

      ASSERT_FALSE( TAILQ_EMPTY(&nodeHead) );

      TAILQ_FOREACH(carry, &nodeHead, entries)
	{
	  switch (counter)
	    {
	    case 0: {
	      EXPECT_TRUE( carry == node1 );
	      break;
	    }
	    case 1: {
	      EXPECT_TRUE( carry == node12 );
	      break;
	    }
	    case 2: {
	      EXPECT_TRUE( carry == node2 );
	      break;
	    }
	    case 3: {
	      EXPECT_TRUE( carry == node4 );
	      break;
	    }
	    case 4: {
	      EXPECT_TRUE( carry == node3 );
	      break;
	    }
	    default: {
	      FAIL();
	    }
	    }
	  counter++;
	} // END - TAILQ_FOREACH

      EXPECT_EQ(5, counter);
    }

    { SCOPED_TRACE("queue reverse foreach");
      int counter = 0;
      struct tqEntry *carry;  // Iterative node

      ASSERT_FALSE( TAILQ_EMPTY(&nodeHead) );

      TAILQ_FOREACH_REVERSE(carry, &nodeHead, tailhead, entries)
	{
	  switch (counter)
	    {
	    case 0: {
	      EXPECT_TRUE( carry == node3 );
	      break;
	    }
	    case 1: {
	      EXPECT_TRUE( carry == node4 );
	      break;
	    }
	    case 2: {
	      EXPECT_TRUE( carry == node2 );
	      break;
	    }
	    case 3: {
	      EXPECT_TRUE( carry == node12 );
	      break;
	    }
	    case 4: {
	      EXPECT_TRUE( carry == node1 );
	      break;
	    }
	    default: {
	      FAIL();
	    }
	    }
	  counter++;
	} // END - TAILQ_FOREACH_REVERSE

      EXPECT_EQ(5, counter);
    }

    { SCOPED_TRACE("check relations");
      EXPECT_TRUE( TAILQ_FIRST(&nodeHead) == node1 );
      EXPECT_TRUE( TAILQ_NEXT(node1, entries) == node12 );
      EXPECT_TRUE( TAILQ_NEXT(node3, entries) == NULL );
      EXPECT_TRUE( TAILQ_NEXT(TAILQ_FIRST(&nodeHead), entries) == node12 );
      EXPECT_TRUE( TAILQ_PREV(node1, tailhead, entries) == NULL );
      EXPECT_TRUE( TAILQ_PREV(node12, tailhead, entries) == node1 );
      EXPECT_TRUE( TAILQ_PREV(TAILQ_LAST(&nodeHead, tailhead), tailhead, entries) == node4 );
    }

    { SCOPED_TRACE("remove nodes");
      TAILQ_REMOVE(&nodeHead, node12, entries);
      EXPECT_TRUE( TAILQ_NEXT(TAILQ_FIRST(&nodeHead), entries) == node2 );

      TAILQ_REMOVE(&nodeHead, node1, entries);
      EXPECT_TRUE( TAILQ_FIRST(&nodeHead) == node2 );

      TAILQ_REMOVE(&nodeHead, node2, entries);
      EXPECT_TRUE( TAILQ_FIRST(&nodeHead) == node4 );

      TAILQ_REMOVE(&nodeHead, node4, entries);
      EXPECT_TRUE( TAILQ_FIRST(&nodeHead) == node3 );

      TAILQ_REMOVE(&nodeHead, node3, entries);
      EXPECT_TRUE( TAILQ_EMPTY(&nodeHead) );
    }

    // Cleaning up
    free(node1);
    free(node2);
    free(node3);
    free(node4);
    free(node12);
}

/*
 * This test checks that
 *   single-linked tail queue works properly.
 */
TEST(sys_queue, single_tail_queue)
{
  struct stqEntry *node1;   // Node 1
  struct stqEntry *node2;   // Node 2
  struct stqEntry *node3;   // Node 3
  struct stqEntry *node4;   // Node 4
  struct stqEntry *node12;  // Node between node1 and node2

  // Initialize head of the single-linked tail queue
  STAILQ_HEAD(stailhead, stqEntry) nodeHead = STAILQ_HEAD_INITIALIZER(nodeHead);
  STAILQ_INIT(&nodeHead);

  { SCOPED_TRACE("check init state");
    ASSERT_FALSE( STAILQ_EMPTY(&nodeHead) == 0 );
    ASSERT_FALSE( STAILQ_FIRST(&nodeHead) != NULL );
  }

  { SCOPED_TRACE("inserts");
    // = Node 1
    node1 = (stqEntry *) malloc(sizeof(struct stqEntry));
    ASSERT_FALSE( node1 == NULL );
    node1->number = TEST_VAL_1;

    STAILQ_INSERT_HEAD(&nodeHead, node1, entries);

    // = Node 2
    node2 = (stqEntry *) malloc(sizeof(struct stqEntry));
    ASSERT_FALSE( node2 == NULL );
    node2->number = TEST_VAL_2;

    STAILQ_INSERT_TAIL(&nodeHead, node2, entries);

    // = Node 3
    node3 = (stqEntry *) malloc(sizeof(struct stqEntry));
    ASSERT_FALSE( node3 == NULL );
    node3->number = TEST_VAL_3;

    STAILQ_INSERT_TAIL(&nodeHead, node3, entries);

    // = Node 4
    node4 = (stqEntry *) malloc(sizeof(struct stqEntry));
    ASSERT_FALSE( node4 == NULL );
    node4->number = TEST_VAL_4;

    STAILQ_INSERT_AFTER(&nodeHead, node2, node4, entries);
  }

  { SCOPED_TRACE("inject");
    node12 = (stqEntry *) malloc(sizeof(struct stqEntry));
    ASSERT_FALSE( node12 == NULL );
    node12->number = TEST_VAL_12;

    STAILQ_INSERT_AFTER(&nodeHead, node1, node12, entries);
  }

  { SCOPED_TRACE("queue foreach");
    int counter = 0;
    struct stqEntry *carry;  // Iterative node

    ASSERT_FALSE( STAILQ_EMPTY(&nodeHead) );

    STAILQ_FOREACH(carry, &nodeHead, entries)
      {
	switch (counter)
	  {
	  case 0: {
	    EXPECT_TRUE( carry == node1 );
	    break;
	  }
	  case 1: {
	    EXPECT_TRUE( carry == node12 );
	    break;
	  }
	  case 2: {
	    EXPECT_TRUE( carry == node2 );
	    break;
	  }
	  case 3: {
	    EXPECT_TRUE( carry == node4 );
	    break;
	  }
	  case 4: {
	    EXPECT_TRUE( carry == node3 );
	    break;
	  }
	  default: {
	    FAIL();
	  }
	  }
	counter++;
      } // END - STAILQ_FOREACH

    EXPECT_EQ(5, counter);
  }

  { SCOPED_TRACE("check relations");
    EXPECT_TRUE( STAILQ_FIRST(&nodeHead) == node1 );
    EXPECT_TRUE( STAILQ_NEXT(node1, entries) == node12 );
    EXPECT_TRUE( STAILQ_NEXT(node3, entries) == NULL );
    EXPECT_TRUE( STAILQ_NEXT(STAILQ_FIRST(&nodeHead), entries) == node12 );
  }

  { SCOPED_TRACE("remove nodes");
    STAILQ_REMOVE_HEAD(&nodeHead, entries);
    EXPECT_TRUE( STAILQ_FIRST(&nodeHead) == node12 );

    STAILQ_REMOVE(&nodeHead, node12, stqEntry, entries);
    EXPECT_TRUE( STAILQ_FIRST(&nodeHead) == node2 );

    STAILQ_REMOVE(&nodeHead, node2, stqEntry, entries);
    EXPECT_TRUE( STAILQ_FIRST(&nodeHead) == node4 );

    STAILQ_REMOVE(&nodeHead, node4, stqEntry, entries);
    EXPECT_TRUE( STAILQ_FIRST(&nodeHead) == node3 );

    STAILQ_REMOVE(&nodeHead, node3, stqEntry, entries);
    EXPECT_TRUE( STAILQ_EMPTY(&nodeHead) );
  }

  // Cleaning up
  free(node1);
  free(node2);
  free(node3);
  free(node4);
  free(node12);
}

/*
 * This test checks that
 *   simple queue works properly.
 */
TEST(sys_queue, simple_queue)
{
  struct smplqEntry *node1;   // Node 1
  struct smplqEntry *node2;   // Node 2
  struct smplqEntry *node3;   // Node 3
  struct smplqEntry *node4;   // Node 4
  struct smplqEntry *node12;  // Node between node1 and node2

  // Initialize head of the simple queue
  SIMPLEQ_HEAD(simplehead, smplqEntry) nodeHead = SIMPLEQ_HEAD_INITIALIZER(nodeHead);
  SIMPLEQ_INIT(&nodeHead);

  { SCOPED_TRACE("check init state");
    ASSERT_FALSE( SIMPLEQ_EMPTY(&nodeHead) == 0 );
    ASSERT_FALSE( SIMPLEQ_FIRST(&nodeHead) != NULL );
  }

  { SCOPED_TRACE("inserts");
    // = Node 1
    node1 = (smplqEntry *) malloc(sizeof(struct smplqEntry));
    ASSERT_FALSE( node1 == NULL );
    node1->number = TEST_VAL_1;

    SIMPLEQ_INSERT_HEAD(&nodeHead, node1, entries);

    // = Node 2
    node2 = (smplqEntry *) malloc(sizeof(struct smplqEntry));
    ASSERT_FALSE( node2 == NULL );
    node2->number = TEST_VAL_2;

    SIMPLEQ_INSERT_TAIL(&nodeHead, node2, entries);

    // = Node 3
    node3 = (smplqEntry *) malloc(sizeof(struct smplqEntry));
    ASSERT_FALSE( node3 == NULL );
    node3->number = TEST_VAL_3;

    SIMPLEQ_INSERT_TAIL(&nodeHead, node3, entries);

    // = Node 4
    node4 = (smplqEntry *) malloc(sizeof(struct smplqEntry));
    ASSERT_FALSE( node4 == NULL );
    node4->number = TEST_VAL_4;

    SIMPLEQ_INSERT_AFTER(&nodeHead, node2, node4, entries);
  }

  { SCOPED_TRACE("inject");
    node12 = (smplqEntry *) malloc(sizeof(struct smplqEntry));
    ASSERT_FALSE( node12 == NULL );
    node12->number = TEST_VAL_12;

    SIMPLEQ_INSERT_AFTER(&nodeHead, node1, node12, entries);
  }

  { SCOPED_TRACE("queue foreach");
    int counter = 0;
    struct smplqEntry *carry;  // Iterative node

    ASSERT_FALSE( SIMPLEQ_EMPTY(&nodeHead) );

    SIMPLEQ_FOREACH(carry, &nodeHead, entries)
      {
	switch (counter)
	  {
	  case 0: {
	    EXPECT_TRUE( carry == node1 );
	    break;
	  }
	  case 1: {
	    EXPECT_TRUE( carry == node12 );
	    break;
	  }
	  case 2: {
	    EXPECT_TRUE( carry == node2 );
	    break;
	  }
	  case 3: {
	    EXPECT_TRUE( carry == node4 );
	    break;
	  }
	  case 4: {
	    EXPECT_TRUE( carry == node3 );
	    break;
	  }
	  default: {
	    FAIL();
	  }
	  }
	counter++;
      } // END - SIMPLEQ_FOREACH

    EXPECT_EQ(5, counter);
  }

  { SCOPED_TRACE("check relations");
    EXPECT_TRUE( SIMPLEQ_FIRST(&nodeHead) == node1 );
    EXPECT_TRUE( SIMPLEQ_NEXT(node1, entries) == node12 );
    EXPECT_TRUE( SIMPLEQ_NEXT(node3, entries) == NULL );
    EXPECT_TRUE( SIMPLEQ_NEXT(SIMPLEQ_FIRST(&nodeHead), entries) == node12 );
  }

  { SCOPED_TRACE("remove nodes");
    SIMPLEQ_REMOVE_HEAD(&nodeHead, entries);
    EXPECT_TRUE( SIMPLEQ_FIRST(&nodeHead) == node12 );

    SIMPLEQ_REMOVE(&nodeHead, node12, smplqEntry, entries);
    EXPECT_TRUE( SIMPLEQ_FIRST(&nodeHead) == node2 );

    SIMPLEQ_REMOVE(&nodeHead, node2, smplqEntry, entries);
    EXPECT_TRUE( SIMPLEQ_FIRST(&nodeHead) == node4 );

    SIMPLEQ_REMOVE(&nodeHead, node4, smplqEntry, entries);
    EXPECT_TRUE( SIMPLEQ_FIRST(&nodeHead) == node3 );

    SIMPLEQ_REMOVE(&nodeHead, node3, smplqEntry, entries);
    EXPECT_TRUE( SIMPLEQ_EMPTY(&nodeHead) );
  }

  // Cleaning up
  free(node1);
  free(node2);
  free(node3);
  free(node4);
  free(node12);
}
