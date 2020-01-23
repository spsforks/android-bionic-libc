#!/bin/bash -e

cd "$(dirname "${BASH_SOURCE[0]}")"/../..
source build/envsetup.sh

TEST_DIR=bionic/fortify_test
RESULTS=bionic/fortify_test/results
RESULT_FILE=$RESULTS/table
mkdir -p $RESULTS
rm -f $RESULT_FILE

if [ -z "$NOBUILD" ]; then
  lunch aosp_x86_64-eng
  mmm $TEST_DIR
  lunch aosp_arm64-eng
  mmm $TEST_DIR
fi

TEST_OUT_DIR=$(get_abs_build_var SOONG_OUT_DIR)/.intermediates/$TEST_DIR
OBJDUMP="prebuilts/clang/host/linux-x86/llvm-binutils-stable/llvm-objdump --line-numbers"
NM="prebuilts/clang/host/linux-x86/llvm-binutils-stable/llvm-nm --demangle --defined-only --extern-only"

function dump() {
  local objfile=$TEST_OUT_DIR/inline_test_$1/$2_static/obj/$TEST_DIR/test.o
  local results=$RESULTS/$1.$3

  $NM $objfile >$results 2>&1

  if grep -q "basic_string" $results; then
    echo "$1 $3 not_inline" >>$RESULT_FILE
  else
    echo "$1 $3 inline" >>$RESULT_FILE
  fi

  echo >>$results
  $OBJDUMP $objfile >>$results 2>&1
}

dump    fortify android_x86_64        android_x86_64
dump    fortify android_x86_x86_64    android_x86
dump    fortify android_arm64_armv8-a android_arm64
dump    fortify android_arm_armv8-a   android_arm
dump    fortify linux_glibc_x86_64    linux_x86_64
dump    fortify linux_glibc_x86       linux_x86
dump    fortify windows_x86_64        windows_x86_64
dump    fortify windows_x86           windows_x86

dump no_fortify android_x86_64        android_x86_64
dump no_fortify android_x86_x86_64    android_x86
dump no_fortify android_arm64_armv8-a android_arm64
dump no_fortify android_arm_armv8-a   android_arm
dump no_fortify linux_glibc_x86_64    linux_x86_64
dump no_fortify linux_glibc_x86       linux_x86
dump no_fortify windows_x86_64        windows_x86_64
dump no_fortify windows_x86           windows_x86

if column -t $RESULT_FILE >$RESULT_FILE.tmp; then
  mv $RESULT_FILE.tmp $RESULT_FILE
fi
cat $RESULT_FILE
