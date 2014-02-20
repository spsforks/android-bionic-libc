#!/bin/sh
source $ANDROID_BUILD_TOP/build/envsetup.sh
mm -j20 -B TARGET_LIBGCC="" > $1 2>&1

