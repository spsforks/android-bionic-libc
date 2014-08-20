#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern "C" unsigned __mallinfo_narenas();
extern "C" unsigned __mallinfo_nbins();
extern "C" struct mallinfo __mallinfo_arena_info(unsigned);
extern "C" struct mallinfo __mallinfo_bin_info(unsigned, unsigned);

int main() {
  // Dump all of the large allocations in the arenas.
  for (unsigned i = 0; i < __mallinfo_narenas(); i++) {
    struct mallinfo mi = __mallinfo_arena_info(i);
    if (mi.hblkhd != 0) {
      printf("arena %d\n", i);
      printf("  mapped %zu\n", mi.hblkhd);
      printf("  allocated_large %zu\n", mi.ordblks);
      printf("  allocated_huge %zu\n", mi.uordblks);
      printf("  bins.allocated %zu\n", mi.fsmblks);
    }
  }
  printf("\n");

  // Dump all of the bins in all arenas.
  size_t total = 0;
  for (unsigned i = 0; i < __mallinfo_narenas(); i++) {
    for (unsigned j = 0; j < __mallinfo_nbins(); j++) {
      struct mallinfo mi = __mallinfo_bin_info(i, j);
      if (mi.ordblks != 0) {
        printf("arena %d bin %d\n", i, j);
        printf("  bins.allocated %zu\n", mi.ordblks);
        total += mi.ordblks;
        printf("  bins.nmalloc %zu\n", mi.uordblks);
        printf("  bins.ndalloc %zu\n", mi.fordblks);
      }
    }
  }
  printf("  Total in bins %zu\n", total);
}
