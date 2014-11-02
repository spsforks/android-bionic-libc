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

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#if defined(__BIONIC__)
  #define RECVMMSG_SUPPORTED 1
  #define SENDMMSG_SUPPORTED 1
#elif defined(__GLIBC_PREREQ)
  #if __GLIBC_PREREQ(2, 12)
    #define RECVMMSG_SUPPORTED 1
  #endif
  #if __GLIBC_PREREQ(2, 14)
    #define SENDMMSG_SUPPORTED 1
  #endif
#endif

#define SOCK_PATH "test"

static void* ConnectFn(void* data) {
  bool (*callback_fn)(int) = reinterpret_cast<bool (*)(int)>(data);
  void* return_value = NULL;

  int fd = socket(PF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC | SOCK_NONBLOCK, IPPROTO_IP);
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
  int fd = socket(PF_UNIX, SOCK_SEQPACKET, IPPROTO_IP);
  ASSERT_NE(fd, -1) << strerror(errno);

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  addr.sun_path[0] = '\0';
  strcpy(addr.sun_path + 1, SOCK_PATH);

  ASSERT_NE(-1, bind(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr))) << strerror(errno);

  ASSERT_NE(-1, listen(fd, 1)) << strerror(errno);

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, ConnectFn, reinterpret_cast<void*>(callback_fn)));

  fd_set read_set;
  FD_ZERO(&read_set);
  FD_SET(fd, &read_set);
  timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;
  ASSERT_LT(0, select(fd+1, &read_set, NULL, NULL, &tv));

  test_fn(&addr, fd);

  void* ret_val;
  ASSERT_EQ(0, pthread_join(thread, &ret_val));
  ASSERT_EQ(NULL, ret_val);

  close(fd);
}

TEST(sys_socket, accept4_error) {
  ASSERT_EQ(-1, accept4(-1, NULL, NULL, 0));
  ASSERT_EQ(EBADF, errno);
}

static void TestAccept4(struct sockaddr_un* addr, int fd) {
  socklen_t len = sizeof(*addr);
  int fd_acc = accept4(fd, reinterpret_cast<struct sockaddr*>(addr), &len, SOCK_CLOEXEC);
  ASSERT_NE(fd_acc, -1) << strerror(errno);

  // Check that the flag was set properly.
  ASSERT_EQ(FD_CLOEXEC, fcntl(fd_acc, F_GETFD) & FD_CLOEXEC);

  close(fd_acc);
}

TEST(sys_socket, accept4_smoke) {
  RunTest(TestAccept4, NULL);
}

#if defined(RECVMMSG_SUPPORTED)
const char* g_RecvMsgs[] = {
  "RECVMMSG_ONE",
  "RECVMMSG_TWO",
  "RECVMMSG_THREE",
};
#define NUM_RECV_MSGS (sizeof(g_RecvMsgs)/sizeof(const char*))

static bool SendMultiple(int fd) {
  for (size_t i = 0; i < NUM_RECV_MSGS; i++) {
    if (send(fd, g_RecvMsgs[i], strlen(g_RecvMsgs[i]) + 1, 0) < 0) {
      GTEST_LOG_(ERROR) << "send call failed: " << strerror(errno);
      return false;
    }
  }

  return true;
}

static void TestRecvMMsg(struct sockaddr_un *addr, int fd) {
  socklen_t len = sizeof(*addr);
  int fd_acc = accept(fd, reinterpret_cast<struct sockaddr*>(addr), &len);
  ASSERT_NE(fd_acc, -1) << strerror(errno);

  struct mmsghdr msgs[NUM_RECV_MSGS];
  memset(msgs, 0, sizeof(struct mmsghdr)*NUM_RECV_MSGS);

  struct iovec io[NUM_RECV_MSGS];
  char bufs[NUM_RECV_MSGS][100];
  for (size_t i = 0; i < NUM_RECV_MSGS; i++) {
    io[i].iov_base = reinterpret_cast<void*>(bufs[i]);
    io[i].iov_len = strlen(g_RecvMsgs[i]) + 1;

    msgs[i].msg_hdr.msg_iov = &io[i];
    msgs[i].msg_hdr.msg_iovlen = 1;
    msgs[i].msg_len = sizeof(struct msghdr);
  }

  struct timespec ts;
  memset(&ts, 0, sizeof(ts));
  ts.tv_sec = 5;
  ts.tv_nsec = 0;
  ASSERT_EQ(NUM_RECV_MSGS,
            static_cast<size_t>(recvmmsg(fd_acc, msgs, NUM_RECV_MSGS, 0, &ts)))
           << strerror(errno);
  for (size_t i = 0; i < NUM_RECV_MSGS; i++) {
    ASSERT_STREQ(g_RecvMsgs[i], bufs[i]);
  }

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
const char* g_SendMsgs[] = {
  "MSG_ONE",
  "MSG_TWO",
  "MSG_THREE"
};
#define NUM_SEND_MSGS (sizeof(g_SendMsgs)/sizeof(const char*))

static bool SendMMsg(int fd) {
  struct mmsghdr msgs[NUM_SEND_MSGS];
  memset(msgs, 0, sizeof(struct mmsghdr)*NUM_SEND_MSGS);
  struct iovec io[NUM_SEND_MSGS];
  for (size_t i = 0; i < NUM_SEND_MSGS; i++) {
    io[i].iov_base = reinterpret_cast<void*>(const_cast<char*>(g_SendMsgs[i]));
    io[i].iov_len = strlen(g_SendMsgs[i]) + 1;
    msgs[i].msg_hdr.msg_iov = &io[i];
    msgs[i].msg_hdr.msg_iovlen = 1;
    msgs[i].msg_len = sizeof(struct msghdr);
  }

  if (sendmmsg(fd, msgs, NUM_SEND_MSGS, 0) < 0) {
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

  for (size_t i = 0; i < NUM_SEND_MSGS; i++) {
    timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    ASSERT_LT(0, select(fd_acc+1, &read_set, NULL, NULL, &tv));
    char buffer[100];
    ASSERT_EQ(strlen(g_SendMsgs[i]) + 1,
              static_cast<size_t>(recv(fd_acc, buffer, sizeof(buffer), 0)));
    ASSERT_STREQ(g_SendMsgs[i], buffer);
  }

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

#define NPACK  10
struct SocketTestWrapper {
  int socketId;
  sockaddr_in sendtoSi;
};

static void* TestRecvFrom(void* data) {
  SocketTestWrapper* dataWrapperP = reinterpret_cast<SocketTestWrapper*>(data);
  void* return_value = NULL;
  int slen = sizeof(sockaddr_in);
  char expected[BUFSIZ];
  char actual[BUFSIZ];
  int ret;
  for (int i=0; i<NPACK; i++) {
    memset(actual, 0, sizeof(actual));
    ret = recvfrom(dataWrapperP->socketId, actual, BUFSIZ, 0, (sockaddr*)&(dataWrapperP->sendtoSi), (socklen_t*)&slen);
    if (ret == -1 ){
      GTEST_LOG_(INFO) << "recvfrom return -1 for i=" << i << ": " << strerror(errno);
      return_value = reinterpret_cast<void*>(-1);
      return return_value;
    }

    bzero(expected, sizeof(expected));
    sprintf(expected, "This is packet %d\n", i);
    if(strcmp(expected, actual) != 0){
      GTEST_LOG_(INFO) << "Received string is not expect for i=" << i << "\n";
      GTEST_LOG_(INFO) << "Excepted" << expected;
      GTEST_LOG_(INFO) << "Actual:" << actual;
      return_value = reinterpret_cast<void*>(-2);
      return return_value;
    }
  }
  return_value = reinterpret_cast<void*>(0);
  return return_value;
}

TEST(sys_socket, sendto_DGRAM_UDP) {
  SocketTestWrapper dataWrapper;
  ASSERT_NE(0, inet_aton("127.0.0.1", &(dataWrapper.sendtoSi.sin_addr))) << "Failed to initialise socked address: " << strerror(errno);
  int address = dataWrapper.sendtoSi.sin_addr.s_addr;

  memset(&(dataWrapper.sendtoSi), 0, sizeof(sockaddr_in));
  dataWrapper.sendtoSi.sin_family = AF_INET;
  dataWrapper.sendtoSi.sin_port = htons(7000);
  dataWrapper.sendtoSi.sin_addr.s_addr = address;

  dataWrapper.socketId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  ASSERT_NE(-1, dataWrapper.socketId) << "Failed to create the socket:" << strerror(errno);

  ASSERT_EQ(0, bind(dataWrapper.socketId, (sockaddr*)&(dataWrapper.sendtoSi), sizeof(sockaddr_in))) << "Failed to bind:" << strerror(errno);

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, TestRecvFrom, &dataWrapper));

  //sleep 1 second to make the recv block first
  usleep(1000000);
  char buf[BUFSIZ];
  for (int i=0; i<NPACK; i++) {
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "This is packet %d\n", i);
    int ret = sendto(dataWrapper.socketId, buf, BUFSIZ, 0, (struct sockaddr*)&(dataWrapper.sendtoSi), sizeof(sockaddr_in));
    ASSERT_NE(-1, ret) << "Failed to call sendto for i=" << i << ": " << strerror(errno);
  }

  void* ret_val;
  ASSERT_EQ(0, pthread_join(thread, &ret_val));
  ASSERT_EQ(NULL, ret_val);

  close(dataWrapper.socketId);
}
