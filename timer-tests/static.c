#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <unistd.h>
int main() {
  printf("AT_SECURE=%lu\n", getauxval(AT_SECURE));
  
  setgid(0);
  setuid(0);
  printf("LD_PRELOAD=%s\n", getenv("LD_PRELOAD"));
  printf("X=%s\n", getenv("X"));
  system("id");
  return 0;
}
