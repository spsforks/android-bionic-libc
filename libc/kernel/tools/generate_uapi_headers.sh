#!/bin/bash

# Script

set -eE

TMPDIR=""
ANDROID_DIR=""
KERNEL_VERSION="android-3.10"
KERNEL_DIR=""
KERNEL_DOWNLOAD=0
ARCH_LIST=("arm" "arm64" "mips" "x86")
ANDROID_KERNEL_DIR="external/kernel-headers/original"
SKIP_ARCH_GENERATION=0

function at_exit () {
  if [[ -d "${TMPDIR}" ]]; then
    rm -rf ${TMPDIR}
  fi
}

#trap at_exit EXIT HUP INT TERM TSTP

while [ $# -gt 0 ]; do
  case "$1" in
    "--skip-arch-generation")
      SKIP_ARCH_GENERATION=1
      ;;
    "--download-kernel")
      KERNEL_DOWNLOAD=1
      ;;
    "--use-kernel-dir")
      if [[ $# -lt 2 ]]; then
        echo "--use-kernel-dir requires an argument."
        exit 1
      fi
      shift
      KERNEL_DIR="$1"
      KERNEL_DOWNLOAD=0
      ;;
    "--android-dir")
      if [[ $# -lt 2 ]]; then
        echo "--android-dr requires an argument."
        exit 1
      fi
      shift
      ANDROID_DIR="$1"
      ;;
    "-"*)
      echo "Error: Unrecognized option $1"
      exit 1
      ;;
    *)
      echo "Error: Extra arguments on the command-line."
      exit 1
      ;;
  esac
  shift
done

ANDROID_KERNEL_DIR="${ANDROID_DIR}/${ANDROID_KERNEL_DIR}"
if [[ "${ANDROID_DIR}" == "" ]]; then
  echo "Must specify destination android directory with --android-dir."
  exit 1
elif [[ ! -d "${ANDROID_KERNEL_DIR}" ]]; then
  echo "${ANDROID_DIR} doesn't appear to be the root of an android tree."
  echo "  ${ANDROID_KERNEL_DIR} is not a directory."
  exit 1
fi

if [[ ${KERNEL_DOWNLOAD} -eq 1 ]]; then
  TMPDIR=$(mktemp -d /tmp/android_kernelXXXXXXXX)
  cd "${TMPDIR}"
  echo "Fetching android kernel source ${KERNEL_VERSION}"
  git clone https://android.googlesource.com/kernel/common.git
  cd common
  git checkout "${KERNEL_VERSION}"
  KERNEL_DIR="${TMPDIR}"
elif [[ "${KERNEL_DIR}" == "" ]]; then
  echo "Must specify one of --use-kernel-dir or --download-kernel."
  exit 1
elif [[ ! -d "${KERNEL_DIR}" ]] || [[ ! -d "${KERNEL_DIR}/common" ]]; then
  echo "The kernel directory $KERNEL_DIR or $KERNEL_DIR/common does not exist."
  exit 1
else
  cd "${KERNEL_DIR}/common"
fi

if [[ ${SKIP_ARCH_GENERATION} -eq 0 ]]; then
  # Build all of the generated headers.
  for arch in "${ARCH_LIST[@]}"; do
    echo "  Generating headers for arch ${arch}"
    make ARCH=${arch} headers_install
  done
fi

function copy_hdrs () {
  local src_dir=$1
  local tgt_dir=$2
  local dont_copy_dirs=$3

  mkdir -p ${tgt_dir}

  local search_dirs=()

  # This only works if none of the filenames have spaces.
  for file in $(ls -d ${src_dir}/* 2> /dev/null); do
    if [[ -d "${file}" ]]; then
      search_dirs+=("${file}")
    elif [[ -f  "${file}" ]] && [[ "${file}" =~ .h$ ]]; then
      cp ${file} ${tgt_dir}
    fi
  done

  if [[ "${dont_copy_dirs}" == "" ]]; then
    for dir in "${search_dirs[@]}"; do
      copy_hdrs "${dir}" ${tgt_dir}/$(basename ${dir})
    done
  fi
}

# Copy all of the include/uapi files to the kernel headers uapi directory.
copy_hdrs "${KERNEL_DIR}/common/include/uapi" "${ANDROID_KERNEL_DIR}/uapi"

copy_hdrs "${KERNEL_DIR}/common/drivers/staging/android/uapi" \
          "${ANDROID_KERNEL_DIR}/uapi/linux" "no-copy-dirs"

copy_hdrs "${KERNEL_DIR}/common/include/generated/uapi" \
          "${ANDROID_KERNEL_DIR}/uapi"

for arch in "${ARCH_LIST[@]}"; do
  if [[ "$arch" == "arm64" ]]; then
    # The Android headers use aarch64 as the name of the 64 bit arm headers.
    tgt_arch="asm-aarch64"
  else
    tgt_arch="asm-${arch}"
  fi
  # Copy arch headers first.
  copy_hdrs "${KERNEL_DIR}/common/arch/${arch}/include/uapi" \
            "${ANDROID_KERNEL_DIR}/uapi/${tgt_arch}"
  # Copy the generated arch header files.
  copy_hdrs "${KERNEL_DIR}/common/arch/${arch}/include/generated/uapi" \
            "${ANDROID_KERNEL_DIR}/uapi/${tgt_arch}"
done
