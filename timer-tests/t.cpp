       #include <arpa/inet.h>
       #include <sys/socket.h>
       #include <net/if.h>
       #include <netdb.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <string.h>
       #include <unistd.h>
       #include <sys/socket.h>
       #include <linux/if_packet.h>

int main() {
  fprintf(stdout, "hello%c", ' ');
  fflush(stdout);
  fprintf(stderr, "world%c", '\n');
  fflush(stderr);
  return 0;
}
