#!/bin/bash
#
# Copyright (C) 2019 The Android Open Source Project
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

# Builds targets for linux_bionic (e.g. host Bionic). Pass a list of ninja targets to build. By
# default, the script builds the "linux_bionic_test" Hello World program.
#
# It isn't necessary to lunch a build target ahead of time. This script will set TARGET_PRODUCT
# and TARGET_BUILD_VARIANT appropriately.
#
# If $OUT_DIR isn't set, this script sets $OUT_DIR to out-linux-bionic to avoid interfering with
# typical "out" directory. Any time the mk/bp files change, the $OUT_DIR must be deleted. However,
# to enable incremental builds, this script does not delete $OUT_DIR itself, and it skips Soong+Kati
# if $OUT_DIR already exists.

cd "$(dirname "$0")/../.."
targets=( "$@" )

if [ "${#targets[@]}" -eq 0 ]; then
  targets=( "${OUT_DIR}/soong/host/linux_bionic-x86/bin/linux_bionic_test" )
fi

# By default, use a special output directory.
if [ -z "$OUT_DIR" ]; then
  export OUT_DIR=out-linux-bionic
fi

# Set the target.
export TARGET_PRODUCT=aosp_x86_64
export TARGET_BUILD_VARIANT=userdebug

# Set up the build.
if [ ! -d "${OUT_DIR}" ]; then
  # Invoke Soong+Kati to set up default linux_glibc variables.
  build/soong/soong_ui.bash --make-mode nothing

  # Reconfigure Soong variables to target linux_bionic.
  python3 <<END - "${OUT_DIR}/soong/soong.variables"
import json
import sys
with open(sys.argv[1]) as f:
  x = json.load(f)
x['Allow_missing_dependencies'] = True
x['HostArch'] = 'x86_64'
x['CrossHost'] = 'linux_bionic'
x['CrossHostArch'] = 'x86_64'
if 'CrossHostSecondaryArch' in x:
  del x['CrossHostSecondaryArch']
if 'DexpreoptGlobalConfig' in x:
  del x['DexpreoptGlobalConfig']
json.dump(x, open(sys.argv[1], mode='w'))
END

  tmp_build_number=$(cat "${OUT_DIR}/build_number.txt")
  echo ${tmp_build_number}_SOONG_ONLY_BUILD > "${OUT_DIR}/build_number.txt"

  build/soong/soong_ui.bash --make-mode --skip-make nothing
fi

printf "Incrementally building targets (to rerun Soong/Kati, delete $(realpath ${OUT_DIR})):\n"
printf ' - %s\n' "${targets[@]}"
prebuilts/build-tools/linux-x86/bin/ninja -f "$OUT_DIR/combined-aosp_x86_64.ninja" "${targets[@]}"
