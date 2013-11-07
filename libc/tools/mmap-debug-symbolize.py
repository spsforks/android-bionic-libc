#!/usr/bin/python

""" Script for symbolizing the mmap backtraces.

Usage:
. build/envsetup.sh
lunch occam-userdebug (or whatever lunch combo)
adb shell setprop libc.debug.mmap 1
adb shell stop && adb shell start
adb shell showmap -a 9733| python mmap-debug.symbolize.py

To symbolize non system libs, set the SYM_PATH env var to the directory
containing symbol files.
"""

import bisect
import commands
import operator
import os
import posixpath
import re
import subprocess
import sys

a2ls = {} # '/path/lib.so' -> popen obj
sym_cache = {} # 'lib.so+0x1234' -> 'symbol::name()'
sym_dirs = []

def main(argv):
  global sym_dirs

  MAPS_LINE_RE = r'^([a-f0-9]+)\s+([a-f0-9]+)(?:\s+\d+){3}\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(.*)$'
  maps = {} # start_address (int) -> dictionary{len, file, off}
  backtraces = {} # "1aba 2eee" -> (pr_dirty, pr_clean, sh_dirty, sh_clean)

  if 'ANDROID_PRODUCT_OUT' not in os.environ:
    print 'Failed to detect Android SDK environment vars. Run envsetup.sh and lunch.'
  else:
    sym_dirs += [os.path.join(os.environ['ANDROID_PRODUCT_OUT'],
                    'symbols', 'system', 'lib')]
  if 'SYM_PATH' in os.environ:
    sym_dirs += os.environ['SYM_PATH']
  sym_dirs += [os.getcwd()]

  # Build mapppings table and filter anon mmap backtraces.
  for line in sys.stdin:
    line = line.rstrip('\r\n')
    m = re.match(MAPS_LINE_RE, line)
    if not m:
      continue
    map_start = int(m.group(1), 16)
    map_end = int(m.group(2), 16)
    mem_sh_clean = int(m.group(3))
    mem_sh_dirty = int(m.group(4))
    mem_pr_clean = int(m.group(5))
    mem_pr_dirty = int(m.group(6))
    # This script relies on the fact the the executable section of a .so is
    # always mapped at offset 0 (which seems to be true at the moment).
    # Unfortunately, showmap does not print out the mapping offset.
    map_offset = 0
    map_file = m.group(7)
    map_length = map_end - map_start + 1
    m = re.match(r'\[anon:([a-f0-9 ]+)\]?', map_file)
    if m:
      bt_str = m.group(1)
      if bt_str not in backtraces:
        backtraces[bt_str] = (0, 0, 0, 0)
      mem_counters = (mem_pr_dirty, mem_sh_dirty, mem_pr_clean, mem_sh_clean)
      backtraces[bt_str] = tuple(map(operator.add,
                                     backtraces[bt_str],
                                     mem_counters))
    else:
      maps[map_start] = {'len' : map_length,
                         'file': map_file,
                         'off': map_offset}

  maps_keys = sorted(maps.keys())
  symbolized_bt_by_total = [] # (mem_counters, bakctrace_str)
  bt_idx = 0
  for bt_str, mem_counters in backtraces.iteritems():
    bt_idx += 1
    print '\rSymbolizing %d/%d' % (bt_idx, len(backtraces)),
    bt_frames = [int(x, 16) for x in bt_str.split(' ')]
    symbolized_bt = ''
    for addr in bt_frames:
      symbolized_bt += '\n  '
      idx = bisect.bisect_right(maps_keys, addr) - 1
      if (idx < 0):
        continue # Skip invalid addr (probably truncated by showmap).
      map_addr = maps_keys[idx]
      mapping = maps[map_addr]
      symbolized_bt += '0x%08x : ' % addr
      offset_in_mapping = addr - map_addr
      if offset_in_mapping >= mapping['len']:
        symbolized_bt += '??'
        continue
      offset_in_file = offset_in_mapping + mapping['off']
      lib_file_name = posixpath.basename(mapping['file'])
      symbolized_bt += symbolize(lib_file_name, offset_in_file)

    symbolized_bt = symbolized_bt[1:]
    symbolized_bt_by_total += [(mem_counters, symbolized_bt)]

  print
  for mem, symbolized_bt in sorted(symbolized_bt_by_total, key=lambda x: x[0]):
    print 'Dirty (PR: %d Kb SH: %d Kb)   Clean (PR: %d Kb SH: %d Kb)' % mem
    print symbolized_bt
    print
  return 0

def symbolize(file_name, addr):
  sym_cache_key = '%s+0x%08x ' % (file_name, addr)
  if sym_cache_key in sym_cache:
    return sym_cache[sym_cache_key]

  filepath = None
  for sym_dir in sym_dirs:
    search_path = os.path.join(sym_dir, file_name)
    if os.path.exists(search_path):
      filepath = search_path
      break;

  if not filepath:
    unknown_sym_str = '%s+0x%08x ' % (file_name, addr)
    sym_cache[sym_cache_key] = sym_cache_key
    return sym_cache_key

  if filepath not in a2ls:
    cmd = 'addr2line -fC -e "%s"' % filepath
    a2ls[filepath] = subprocess.Popen(
        cmd, shell=True, stdout=subprocess.PIPE, stdin=subprocess.PIPE,
        stderr=sys.stderr, close_fds=True)
  a2l = a2ls[filepath]
  print >>a2l.stdin, hex(addr)
  sym_name = a2l.stdout.readline().rstrip()
  assert(sym_name)
  sym_path = a2l.stdout.readline().rstrip()
  assert(sym_path)
  sym_cache[sym_cache_key] = sym_name
  return sym_name

if __name__ == "__main__":
  sys.exit(main(sys.argv))
