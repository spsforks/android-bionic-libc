/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef MAP_TO_14SEGMENT_H
#define MAP_TO_14SEGMENT_H
#include <linux/errno.h>
#include <linux/types.h>
#include <asm/byteorder.h>
#define BIT_SEG14_A 0
#define BIT_SEG14_B 1
#define BIT_SEG14_C 2
#define BIT_SEG14_D 3
#define BIT_SEG14_E 4
#define BIT_SEG14_F 5
#define BIT_SEG14_G1 6
#define BIT_SEG14_G2 7
#define BIT_SEG14_H 8
#define BIT_SEG14_I 9
#define BIT_SEG14_J 10
#define BIT_SEG14_K 11
#define BIT_SEG14_L 12
#define BIT_SEG14_M 13
#define BIT_SEG14_RESERVED1 14
#define BIT_SEG14_RESERVED2 15
struct seg14_conversion_map {
  __be16 table[128];
};
static __inline__ int map_to_seg14(struct seg14_conversion_map * map, int c) {
  if(c < 0 || c >= sizeof(map->table) / sizeof(map->table[0])) return - EINVAL;
  return __be16_to_cpu(map->table[c]);
}
#define SEG14_CONVERSION_MAP(_name,_map) struct seg14_conversion_map _name = {.table = { _map } }
#define MAP_TO_SEG14_SYSFS_FILE "map_seg14"
#define _SEG14(sym,a,b,c,d,e,f,g1,g2,h,j,k,l,m,n) __cpu_to_be16(a << BIT_SEG14_A | b << BIT_SEG14_B | c << BIT_SEG14_C | d << BIT_SEG14_D | e << BIT_SEG14_E | f << BIT_SEG14_F | g1 << BIT_SEG14_G1 | g2 << BIT_SEG14_G2 | h << BIT_SEG14_H | j << BIT_SEG14_I | k << BIT_SEG14_J | l << BIT_SEG14_K | m << BIT_SEG14_L | n << BIT_SEG14_M)
#define _MAP_0_32_ASCII_SEG14_NON_PRINTABLE 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#define _MAP_33_47_ASCII_SEG14_SYMBOL _SEG14('!', 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), _SEG14('"', 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0), _SEG14('#', 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0), _SEG14('$', 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0), _SEG14('%', 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0), _SEG14('&', 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1), _SEG14('\'', 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0), _SEG14('(', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1), _SEG14(')', 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0), _SEG14('*', 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1), _SEG14('+', 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0), _SEG14(',', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0), _SEG14('-', 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0), _SEG14('.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1), _SEG14('/', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0),
#define _MAP_48_57_ASCII_SEG14_NUMERIC _SEG14('0', 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0), _SEG14('1', 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0), _SEG14('2', 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0), _SEG14('3', 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0), _SEG14('4', 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0), _SEG14('5', 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1), _SEG14('6', 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0), _SEG14('7', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0), _SEG14('8', 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0), _SEG14('9', 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0),
#define _MAP_58_64_ASCII_SEG14_SYMBOL _SEG14(':', 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0), _SEG14(';', 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0), _SEG14('<', 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1), _SEG14('=', 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0), _SEG14('>', 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0), _SEG14('?', 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0), _SEG14('@', 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0),
#define _MAP_65_90_ASCII_SEG14_ALPHA_UPPER _SEG14('A', 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0), _SEG14('B', 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0), _SEG14('C', 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0), _SEG14('D', 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0), _SEG14('E', 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0), _SEG14('F', 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0), _SEG14('G', 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0), _SEG14('H', 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0), _SEG14('I', 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0), _SEG14('J', 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), _SEG14('K', 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1), _SEG14('L', 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0), _SEG14('M', 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0), _SEG14('N', 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1), _SEG14('O', 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0), _SEG14('P', 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0), _SEG14('Q', 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1), _SEG14('R', 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1), _SEG14('S', 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0), _SEG14('T', 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0), _SEG14('U', 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0), _SEG14('V', 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0), _SEG14('W', 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1), _SEG14('X', 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1), _SEG14('Y', 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0), _SEG14('Z', 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0),
#define _MAP_91_96_ASCII_SEG14_SYMBOL _SEG14('[', 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0), _SEG14('\\', 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), _SEG14(']', 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), _SEG14('^', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1), _SEG14('_', 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), _SEG14('`', 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0),
#define _MAP_97_122_ASCII_SEG14_ALPHA_LOWER _SEG14('a', 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0), _SEG14('b', 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1), _SEG14('c', 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0), _SEG14('d', 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0), _SEG14('e', 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0), _SEG14('f', 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0), _SEG14('g', 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0), _SEG14('h', 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0), _SEG14('i', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0), _SEG14('j', 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0), _SEG14('k', 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1), _SEG14('l', 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0), _SEG14('m', 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0), _SEG14('n', 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0), _SEG14('o', 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0), _SEG14('p', 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0), _SEG14('q', 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0), _SEG14('r', 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0), _SEG14('s', 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1), _SEG14('t', 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0), _SEG14('u', 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0), _SEG14('v', 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0), _SEG14('w', 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1), _SEG14('x', 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1), _SEG14('y', 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0), _SEG14('z', 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0),
#define _MAP_123_126_ASCII_SEG14_SYMBOL _SEG14('{', 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0), _SEG14('|', 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0), _SEG14('}', 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1), _SEG14('~', 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0),
#define MAP_ASCII14SEG_ALPHANUM _MAP_0_32_ASCII_SEG14_NON_PRINTABLE _MAP_33_47_ASCII_SEG14_SYMBOL _MAP_48_57_ASCII_SEG14_NUMERIC _MAP_58_64_ASCII_SEG14_SYMBOL _MAP_65_90_ASCII_SEG14_ALPHA_UPPER _MAP_91_96_ASCII_SEG14_SYMBOL _MAP_97_122_ASCII_SEG14_ALPHA_LOWER _MAP_123_126_ASCII_SEG14_SYMBOL
#define SEG14_DEFAULT_MAP(_name) SEG14_CONVERSION_MAP(_name, MAP_ASCII14SEG_ALPHANUM)
#endif
