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

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#if defined(__BIONIC__)
  #define ACCEPT4_SUPPORTED 1
  #define RECVMMSG_SUPPORTED 1
  #define SENDMMSG_SUPPORTED 1
#elif defined(__GLIBC_PREREQ)
  #if __GLIBC_PREREQ(2, 9)
    #define ACCEPT4_SUPPORTED 1
  #endif
  #if __GLIBC_PREREQ(2, 12)
    #define RECVMMSG_SUPPORTED 1
  #endif
  #if __GLIBC_PREREQ(2, 14)
    #define SENDMMSG_SUPPORTED 1
  #endif
#endif

#if defined(ACCEPT4_SUPPORTED) || defined(RECVMMSG_SUPPORTED) || defined(SENDMMSG_SUPPORTED)

#define SOCK_PATH "test"

static void* ConnectFn(void* data) {
  bool (*callback_fn)(int) = reinterpret_cast<bool (*)(int)>(data);
  void* return_value = NULL;

  int fd = socket(PF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
  if (fd < 0) {
    GTEST_LOG_(ERROR) << "socket call failed: " << strerror(errno);
    return reinterpret_cast<void*>(-1);
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  addr.sun_path[0] = '\0';
  strcpy(addr.sun_path + 1, SOCK_PATH);

  if (connect(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
    GTEST_LOG_(ERROR) << "connect call failed: " << strerror(errno);
    return_value = reinterpret_cast<void*>(-1);
  }
  else if (callback_fn != NULL && !callback_fn(fd)) {
    return_value = reinterpret_cast<void*>(-1);
  }

  close(fd);

  return return_value;
}

static void RunTest(void (*test_fn)(struct sockaddr_un*, int),
                    bool (*callback_fn)(int fd)) {
  int fd = socket(PF_UNIX, SOCK_SEQPACKET, 0);
  ASSERT_NE(fd, -1) << strerror(errno);

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  addr.sun_path[0] = '\0';
  strcpy(addr.sun_path + 1, SOCK_PATH);

  ASSERT_NE(-1, bind(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr))) << strerror(errno);

  ASSERT_NE(-1, listen(fd, 1)) << strerror(errno);

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, ConnectFn, (void*)callback_fn));

  fd_set read_set;
  FD_ZERO(&read_set);
  FD_SET(fd, &read_set);
  timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;
  ASSERT_LT(0, select(fd+1, &read_set, NULL, NULL, &tv));

  test_fn(&addr, fd);

  void* ret_val;
  pthread_join(thread, &ret_val);
  ASSERT_EQ(NULL, ret_val);

  close(fd);
}
#endif

TEST(sys_socket, accept4_error) {
#if defined(ACCEPT4_SUPPORTED)
  ASSERT_EQ(-1, accept4(-1, NULL, NULL, 0));
  ASSERT_EQ(EBADF, errno);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

#if defined(ACCEPT4_SUPPORTED)
static void TestAccept4(struct sockaddr_un *addr, int fd) {
  socklen_t len = sizeof(*addr);
  int fd_acc = accept4(fd, reinterpret_cast<struct sockaddr*>(addr), &len, SOCK_CLOEXEC);
  ASSERT_NE(fd_acc, -1) << strerror(errno);

  // Check that the flag was set properly.
  ASSERT_EQ(FD_CLOEXEC, fcntl(fd_acc, F_GETFD) & FD_CLOEXEC);

  close(fd_acc);
}
#endif

TEST(sys_socket, accept4_smoke) {
#if defined(ACCEPT4_SUPPORTED)
  RunTest(TestAccept4, NULL);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

#if defined(RECVMMSG_SUPPORTED)
static bool SendMultiple(int fd) {
  if (send(fd, "SEND_MSG_ONE", 13, 0) < 0) {
    GTEST_LOG_(ERROR) << "send call failed: " << strerror(errno);
    return false;
  }
  if (send(fd, "SEND_MSG_TWO", 13, 0) < 0) {
    GTEST_LOG_(ERROR) << "send call failed: " << strerror(errno);
    return false;
  }

  return true;
}

static void TestRecvMMsg(struct sockaddr_un *addr, int fd) {
  socklen_t len = sizeof(*addr);
  int fd_acc = accept(fd, reinterpret_cast<struct sockaddr*>(addr), &len);
  ASSERT_NE(fd_acc, -1) << strerror(errno);

  struct iovec io[2];
  char bufs[2][100];
  io[0].iov_base = reinterpret_cast<void*>(bufs[0]);
  io[0].iov_len = sizeof(bufs[0]);
  io[1].iov_base = reinterpret_cast<void*>(bufs[1]);
  io[1].iov_len = sizeof(bufs[1]);

  struct mmsghdr msgs[2];
  memset(msgs, 0, sizeof(struct mmsghdr)*2);
  msgs[0].msg_hdr.msg_iov = &io[0];
  msgs[0].msg_hdr.msg_iovlen = 1;
  msgs[0].msg_len = sizeof(struct msghdr);

  msgs[1].msg_hdr.msg_iov = &io[1];
  msgs[1].msg_hdr.msg_iovlen = 1;
  msgs[1].msg_len = sizeof(struct msghdr);

  struct timespec ts;
  memset(&ts, 0, sizeof(ts));
  ts.tv_sec = 5;
  ts.tv_nsec = 0;
  ASSERT_EQ(2, recvmmsg(fd_acc, msgs, 2, 0, &ts)) << strerror(errno);
  ASSERT_STREQ("SEND_MSG_ONE", bufs[0]);
  ASSERT_STREQ("SEND_MSG_TWO", bufs[1]);

  close(fd_acc);
}
#endif

TEST(sys_socket, recvmmsg_smoke) {
#if defined(RECVMMSG_SUPPORTED)
  RunTest(TestRecvMMsg, SendMultiple);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

TEST(sys_socket, recvmmsg_error) {
#if defined(RECVMMSG_SUPPORTED)
  ASSERT_EQ(-1, recvmmsg(-1, NULL, 0, 0, NULL));
  ASSERT_EQ(EBADF, errno);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

#if defined(SENDMMSG_SUPPORTED)
static bool SendMMsg(int fd) {
  struct iovec io[3];
  char* bufs[3];
  bufs[0] = const_cast<char*>("MSG_ONE");
  bufs[1] = const_cast<char*>("MSG_TWO");
  bufs[2] = const_cast<char*>("MSG_THREE");
  io[0].iov_base = reinterpret_cast<void*>(bufs[0]);
  io[0].iov_len = 8;
  io[1].iov_base = reinterpret_cast<void*>(bufs[1]);
  io[1].iov_len = 8;
  io[2].iov_base = reinterpret_cast<void*>(bufs[2]);
  io[2].iov_len = 10;

  struct mmsghdr msgs[3];
  memset(msgs, 0, sizeof(struct mmsghdr)*3);
  msgs[0].msg_hdr.msg_iov = &io[0];
  msgs[0].msg_hdr.msg_iovlen = 1;
  msgs[0].msg_len = sizeof(struct msghdr);

  msgs[1].msg_hdr.msg_iov = &io[1];
  msgs[1].msg_hdr.msg_iovlen = 1;
  msgs[1].msg_len = sizeof(struct msghdr);

  msgs[2].msg_hdr.msg_iov = &io[2];
  msgs[2].msg_hdr.msg_iovlen = 2;
  msgs[2].msg_len = sizeof(struct msghdr);
  if (sendmmsg(fd, msgs, 3, 0) < 0) {
    GTEST_LOG_(ERROR) << "sendmmsg call failed: " << strerror(errno);
    return false;
  }
  return true;
}

static void TestSendMMsg(struct sockaddr_un *addr, int fd) {
  socklen_t len = sizeof(*addr);
  int fd_acc = accept(fd, reinterpret_cast<struct sockaddr*>(addr), &len);
  ASSERT_NE(fd_acc, -1) << strerror(errno);

  fd_set read_set;
  FD_ZERO(&read_set);
  FD_SET(fd_acc, &read_set);

  timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;
  ASSERT_LT(0, select(fd_acc+1, &read_set, NULL, NULL, &tv));

  char buffer[100];
  ASSERT_EQ(8, recv(fd_acc, buffer, sizeof(buffer), 0));
  ASSERT_STREQ("MSG_ONE", buffer);

  tv.tv_sec = 5;
  tv.tv_usec = 0;
  ASSERT_LT(0, select(fd_acc+1, &read_set, NULL, NULL, &tv));
  ASSERT_EQ(8, recv(fd_acc, buffer, sizeof(buffer), 0));
  ASSERT_STREQ("MSG_TWO", buffer);

  tv.tv_sec = 5;
  tv.tv_usec = 0;
  ASSERT_LT(0, select(fd_acc+1, &read_set, NULL, NULL, &tv));
  ASSERT_EQ(10, recv(fd_acc, buffer, sizeof(buffer), 0));
  ASSERT_STREQ("MSG_THREE", buffer);

  close(fd_acc);
}
#endif

TEST(sys_socket, sendmmsg_smoke) {
#if defined(SENDMMSG_SUPPORTED)
  RunTest(TestSendMMsg, SendMMsg);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

TEST(sys_socket, sendmmsg_error) {
#if defined(SENDMMSG_SUPPORTED)
  ASSERT_EQ(-1, sendmmsg(-1, NULL, 0, 0));
  ASSERT_EQ(EBADF, errno);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}
