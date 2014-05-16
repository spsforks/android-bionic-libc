#!/usr/bin/python

import glob
import os
import re
import string
import subprocess
import sys

toolchain = os.environ['ANDROID_TOOLCHAIN']
arch = re.sub(r'.*/linux-x86/([^/]+)/.*', r'\1', toolchain)

def GetSymbolsFromSo(so_file):

  # Example readelf output:
  #   264: 0001623c     4 FUNC    GLOBAL DEFAULT    8 cabsf
  #   266: 00016244     4 FUNC    GLOBAL DEFAULT    8 dremf
  #   267: 00019018     4 OBJECT  GLOBAL DEFAULT   11 __fe_dfl_env
  #   268: 00000000     0 FUNC    GLOBAL DEFAULT  UND __aeabi_dcmplt

  r = re.compile(r' +\d+: [0-9a-f]+ +\d+ (I?FUNC|OBJECT) +\S+ +\S+ +\d+ (\S+)')

  symbols = set()

  for line in subprocess.check_output(['readelf', '--dyn-syms', '-W', so_file]).split('\n'):
     if ' HIDDEN ' in line or ' UND ' in line:
       continue
     m = r.match(line)
     if m:
       symbol = m.group(2)
       symbol = re.sub('@.*', '', symbol)
       symbols.add(symbol)

  return symbols

def GetSymbolsFromAndroidSo(*files):
  symbols = set()
  for f in files:
    symbols = symbols | GetSymbolsFromSo('%s/system/lib64/%s' % (os.environ['ANDROID_PRODUCT_OUT'], f))
  return symbols

def GetSymbolsFromSystemSo(*files):
  symbols = set()
  for f in files:
    symbols = symbols | GetSymbolsFromSo('/lib/x86_64-linux-gnu/%s' % f)
  return symbols

glibc = GetSymbolsFromSystemSo('libc.so.6', 'librt-2.15.so', 'libpthread-2.15.so', 'libresolv-2.15.so', 'libm.so.6')
bionic = GetSymbolsFromAndroidSo('libc.so', 'libm.so')

print 'glibc:'
for symbol in sorted(glibc):
  print symbol

print
print 'bionic:'
for symbol in sorted(bionic):
  print symbol

print
print 'in bionic but not glibc:'
for symbol in sorted(bionic.difference(glibc)):
  print symbol

sys.exit(0)
