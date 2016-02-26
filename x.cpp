#include "./libc/bionic/ifaddrs.cpp"

       #include <arpa/inet.h>
       #include <sys/socket.h>
       #include <net/if.h>
       #include <netdb.h>
       #include <ifaddrs.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h>

       int
       main(int argc, char *argv[])
       {
           struct ifaddrs *ifaddr, *ifa;
           int family, s;
           char host[NI_MAXHOST];

           if (getifaddrs(&ifaddr) == -1) {
               perror("getifaddrs");
               exit(EXIT_FAILURE);
           }

           /* Walk through linked list, maintaining head pointer so we
              can free list later */

           for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
               if (ifa->ifa_addr == NULL)
                   continue;

               family = ifa->ifa_addr->sa_family;

               /* Display interface name and family (including symbolic
                  form of the latter for the common families) */

               printf("%-5s  address family: %d%s %x\n",
                       ifa->ifa_name, family,
                       (family == AF_PACKET) ? " (AF_PACKET)" :
                       (family == AF_INET) ?   " (AF_INET)" :
                       (family == AF_INET6) ?  " (AF_INET6)" : "",
                       ifa->ifa_flags);

               /* For an AF_INET* interface address, display the address */

               if (family == AF_INET || family == AF_INET6) {
                   s = getnameinfo(ifa->ifa_addr,
                           (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                 sizeof(struct sockaddr_in6),
                           host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                   if (s != 0) {
                       printf("%d getnameinfo() failed: %s\n", family, gai_strerror(s));
                       exit(EXIT_FAILURE);
                   }
                   printf("\taddress: <%s>\n", host);
                   
                   if (ifa->ifa_broadaddr && (ifa->ifa_flags & (IFF_BROADCAST | IFF_POINTOPOINT)) != 0) {
               family = ifa->ifa_broadaddr->sa_family;
                     s = getnameinfo(ifa->ifa_broadaddr,
                             (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                   sizeof(struct sockaddr_in6),
                             host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                     if (s != 0) {
                         printf("%d getnameinfo() failed: %s\n", family, gai_strerror(s));
                     } else {
                         printf("\tbroadcast: <%s>\n", host);
                     }
                   }
               }
           }

           freeifaddrs(ifaddr);
           exit(EXIT_SUCCESS);
       }


