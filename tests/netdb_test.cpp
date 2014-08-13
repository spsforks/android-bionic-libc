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

#include <gtest/gtest.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

bool find_socket_type(int socket_type, addrinfo* ai_link){
  addrinfo* rp = NULL;
  for (rp = ai_link; rp != NULL; rp = rp->ai_next) {
    if(socket_type == rp->ai_socktype){
        return true;
    }
  }
  return false;
}

TEST(netdb, getaddrinfo_NULL_hints) {
  addrinfo* res = NULL;
  ASSERT_EQ(0, getaddrinfo("localhost", "9999", NULL, &res));

  //assume SOCK_STREAM and SOCK_DGRAM are supported
  ASSERT_NE((addrinfo *)NULL, res->ai_next);
  ASSERT_TRUE(find_socket_type(SOCK_STREAM, res));
  ASSERT_TRUE(find_socket_type(SOCK_DGRAM, res));

  freeaddrinfo(res);
}

TEST(netdb, getaddrinfo_hints) {
  addrinfo* res = NULL;
  struct addrinfo  hints;

  /* now try with the hints */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  ASSERT_EQ(0, getaddrinfo( "localhost", "9999", &hints, &res));
  //the return information should have the same socket type
  //and protocol as specified in hints
  ASSERT_EQ(SOCK_STREAM, res->ai_socktype);
  ASSERT_EQ(IPPROTO_TCP, res->ai_protocol);
  ASSERT_EQ((addrinfo *)NULL, res->ai_next);
  freeaddrinfo(res);
}

TEST(netdb, getnameinfo_salen) {
  sockaddr_storage ss;
  memset(&ss, 0, sizeof(ss));
  sockaddr* sa = reinterpret_cast<sockaddr*>(&ss);
  char tmp[16];

  ss.ss_family = AF_INET;
  socklen_t too_much = sizeof(ss);
  socklen_t just_right = sizeof(sockaddr_in);
  socklen_t too_little = sizeof(sockaddr_in) - 1;

  ASSERT_EQ(0, getnameinfo(sa, too_much, tmp, sizeof(tmp), NULL, 0, NI_NUMERICHOST));
  ASSERT_STREQ("0.0.0.0", tmp);
  ASSERT_EQ(0, getnameinfo(sa, just_right, tmp, sizeof(tmp), NULL, 0, NI_NUMERICHOST));
  ASSERT_STREQ("0.0.0.0", tmp);
  ASSERT_EQ(EAI_FAMILY, getnameinfo(sa, too_little, tmp, sizeof(tmp), NULL, 0, NI_NUMERICHOST));

  ss.ss_family = AF_INET6;
  just_right = sizeof(sockaddr_in6);
  too_little = sizeof(sockaddr_in6) - 1;
  too_much = just_right + 1;

  ASSERT_EQ(0, getnameinfo(sa, too_much, tmp, sizeof(tmp), NULL, 0, NI_NUMERICHOST));
  ASSERT_STREQ("::", tmp);
  ASSERT_EQ(0, getnameinfo(sa, just_right, tmp, sizeof(tmp), NULL, 0, NI_NUMERICHOST));
  ASSERT_STREQ("::", tmp);
  ASSERT_EQ(EAI_FAMILY, getnameinfo(sa, too_little, tmp, sizeof(tmp), NULL, 0, NI_NUMERICHOST));
}
