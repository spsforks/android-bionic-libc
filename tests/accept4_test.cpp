/*
 * Copyright (C) 2013 The Android Open Source Project
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

/*
 * Contributed by: Intel Corporation, 2014
 */

#include <gtest/gtest.h>

#if !defined(__GLIBC__)

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#define SOCK_PATH "test"

static void connect_cb(void)
{
    struct sockaddr_un addr;
    int fd, err;

    fd = socket(PF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
    ASSERT_NE(fd, -1) << strerror(errno);

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    addr.sun_path[0] = '\0';
    strcpy(addr.sun_path + 1, SOCK_PATH);

    err = connect(fd, (struct sockaddr *) &addr, sizeof(addr));
    ASSERT_NE(err, -1) << strerror(errno);

    close(fd);
}

static void *thread(void *)
{
    connect_cb();

    return NULL;
}

TEST(accept4, smoke) {
    struct sockaddr_un addr;
    socklen_t len = sizeof(addr);
    int fd, fd_acc, err, flag;
    pthread_t t;

    fd = socket(PF_UNIX, SOCK_SEQPACKET, 0);
    ASSERT_NE(fd, -1) << strerror(errno);

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    addr.sun_path[0] = '\0';
    strcpy(addr.sun_path + 1, SOCK_PATH);

    err = bind(fd, (struct sockaddr *) &addr, len);
    ASSERT_NE(err, -1) << strerror(errno);

    err = listen(fd, 1);
    ASSERT_NE(err, -1) << strerror(errno);

    ASSERT_EQ(0, pthread_create(&t, NULL, thread, NULL));

    fd_acc = accept4(fd, (struct sockaddr *) &addr, &len, SOCK_CLOEXEC);
    ASSERT_NE(fd_acc, -1) << strerror(errno);

    /* Check the flag */
    flag = fcntl(fd_acc, F_GETFD);
    ASSERT_EQ(flag & FD_CLOEXEC, 1);

    pthread_join(t, NULL);

    close(fd_acc);
    close(fd);
}

#endif
