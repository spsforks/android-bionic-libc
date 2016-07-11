#!/bin/bash -e

. $(dirname $0)/../build/run-on-host.sh

BITS=$1
shift

if [ "${BITS}" != 32 -a "${BITS}" != 64 ]; then
    echo "Must pass either 32 or 64 on command line"
    exit 1
fi

if [ ${HOST_OS}-${HOST_ARCH} = linux-x86 -o ${HOST_OS}-${HOST_ARCH} = linux-x86_64 ]; then

    prepare bionic-benchmarks

    if [ ${TARGET_ARCH} = x86 -o ${TARGET_ARCH} = x86_64 ]; then
	if [ ${BITS} = 32 ]; then
	    (
		cd ${ANDROID_BUILD_TOP}
		ANDROID_DATA=${TARGET_OUT_DATA} ANDROID_ROOT=${TARGET_OUT} ${TARGET_OUT_DATA}/nativetest/bionic-benchmarks/bionic-benchmarks32 $@
	    )
	elif [ ${BITS} = 64 ]; then
	    (
		cd ${ANDROID_BUILD_TOP}
		ANDROID_DATA=${TARGET_OUT_DATA} ANDROID_ROOT=${TARGET_OUT} ${TARGET_OUT_DATA}/nativetest64/bionic-benchmarks/bionic-benchmarks64 $@
	    )
	fi
    fi
fi
