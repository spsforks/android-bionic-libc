#!/bin/bash
OUR_DIRS="libc/bionic libc/private libdl libstdc++ linker tests"
for dir in $OUR_DIRS; do
  find $dir \( -name '*.h' -o -name '*.cpp' \) -print -exec \
    clang-format -i {} \;
done
