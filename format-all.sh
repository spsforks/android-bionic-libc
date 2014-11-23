#!/bin/bash
for dir in libc/bionic libc/private tests; do
  find $dir \( -name '*.h' -o -name '*.cpp' \) -print -exec \
    clang-format -i {} \;
done
