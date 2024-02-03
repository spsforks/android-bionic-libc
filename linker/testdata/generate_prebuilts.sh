#!/bin/bash

# Copyright (C) 2024 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Generate bionic linker test ELFs
#
# This script generates the prebuilt ELFs in the directory
#     bionic/linker/tests/data/${arch}/
# used in bionic linker tests.

if [[ -z "$ANDROID_BUILD_TOP" ]]; then
  echo "Error: ANDROID_BUILD_TOP not set"
  echo "       Please run lunch before using this script"
  exit 1
fi

set -e

cd $ANDROID_BUILD_TOP

source build/envsetup.sh
ARCHS="x86,arm,arm64,x86_64,riscv64"
ELFS="crt_pad_segment_enabled,crt_pad_segment_disabled,no_crt_pad_segment"

for arch in ${ARCHS//,/ }
do
  lunch "aosp_${arch}-trunk-userdebug"

  LIB=lib
  if [[ $arch =~ "64" ]]; then
    LIB=lib64
  fi

  for elf in ${ELFS//,/ }
  do
    TMPFILE=$(mktemp)
    build/soong/soong_ui.bash --make-mode ${elf} 2>&1 | tee ${TMPFILE}
    TARGETDIR=bionic/linker/testdata/${arch}/${elf}.so
    mkdir -p ${TARGETDIR%/*}
    cp $OUT/system/${LIB}/${elf}.so ${TARGETDIR}
  done
done
