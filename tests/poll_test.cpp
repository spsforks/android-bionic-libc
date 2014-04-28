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

#include <poll.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

const static std::string TEST_MESSAGE = "White wall with yellow bricks.";

#define PT_FAIL() pthread_exit((void *) 1)

static void* threadWriteBody(void *args __attribute__((unused)))
{
	int sock;
	struct addrinfo hints;
	struct addrinfo *sin;

	memset (&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(NULL, "4444", &hints, &sin) != 0)
	    PT_FAIL();

	usleep(50000);  // Sleep to guarantee second thread initialization

	// Create a socket
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	    PT_FAIL();

	if (connect(sock, sin->ai_addr, sin->ai_addrlen) == -1)
	    PT_FAIL();

	send(sock, TEST_MESSAGE.c_str(), TEST_MESSAGE.size(), 0);

	close(sock);

	pthread_exit(NULL);
}

static void* threadPollBody(void *args __attribute__((unused)))
{
	struct pollfd fds;
	int sock, result, reuseaddr;
	struct sockaddr_in sin;
	struct sockaddr sa;
	socklen_t size;

	// Create a socket
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	    PT_FAIL();
	sin.sin_family = AF_INET;
	sin.sin_port = htons(4444);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

    reuseaddr = 1;

    // Set socket options
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) < 0)
       PT_FAIL();
    // Bind
	if (bind(sock, (struct sockaddr *) &sin, sizeof (sin)) < 0)
		PT_FAIL();

	// Listen
	if (listen(sock, 1) < 0)
		PT_FAIL();

	memset (&fds, 0, sizeof (fds));
	fds.fd = sock;
	fds.events = POLLIN;

	// First poll
	result = poll(&fds, 1, 200);
	if ((result == -1) || (result == 0))
		PT_FAIL();
	size = sizeof (sa);
	if (accept (sock, &sa, &size) < 0)
		PT_FAIL();

	// Second poll
	result = poll(&fds, 1, 200);
	if (result != 0)
	    PT_FAIL();

	close (sock);
	pthread_exit(NULL);
}

TEST(poll, poll)
{
    pthread_t threadWrite;
    pthread_t threadPoll;
	void *retWrite;
	void *retPoll;

    { SCOPED_TRACE("poll()");
       ASSERT_FALSE( pthread_create (&threadWrite, NULL, threadWriteBody, NULL) != 0 );
       ASSERT_FALSE( pthread_create (&threadPoll, NULL, threadPollBody, NULL) != 0 );

       EXPECT_FALSE( pthread_join (threadWrite, &retWrite) != 0 );
       EXPECT_FALSE( pthread_join (threadPoll, &retPoll) != 0 );

       ASSERT_TRUE( retWrite == NULL );
       ASSERT_TRUE( retPoll == NULL );
    }
}
