/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef NETROM_KERNEL_H
#define NETROM_KERNEL_H
#include <linux/ax25.h>
#define NETROM_MTU 236
#define NETROM_T1 1
#define NETROM_T2 2
#define NETROM_N2 3
#define NETROM_T4 6
#define NETROM_IDLE 7
#define SIOCNRDECOBS (SIOCPROTOPRIVATE + 2)
struct nr_route_struct {
#define NETROM_NEIGH 0
#define NETROM_NODE 1
  int type;
  ax25_address callsign;
  char device[16];
  unsigned int quality;
  char mnemonic[7];
  ax25_address neighbour;
  unsigned int obs_count;
  unsigned int ndigis;
  ax25_address digipeaters[AX25_MAX_DIGIS];
};
#endif
