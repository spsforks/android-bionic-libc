#!/bin/bash

. $(dirname $0)/../build/run-on-host.sh

if [ "$1" = glibc ]; then
    m -j bionic-unit-tests-glibc
    (
        cd ${ANDROID_BUILD_TOP}
        export ANDROID_DATA=${TARGET_OUT_DATA}
        export ANDROID_ROOT=${TARGET_OUT}
        ${HOST_OUT}/nativetest64/bionic-unit-tests-glibc/bionic-unit-tests-glibc $@
    )
    exit 0
fi

if [ ${HOST_OS}-${HOST_ARCH} = linux-x86 -o ${HOST_OS}-${HOST_ARCH} = linux-x86_64 ]; then

    prepare $1 bionic-unit-tests

    if [ ${TARGET_ARCH} = x86 -o ${TARGET_ARCH} = x86_64 ]; then
        (
            cd ${ANDROID_BUILD_TOP}
            export ANDROID_DATA=${TARGET_OUT_DATA}
            export ANDROID_DNS_MODE=local
            export ANDROID_ROOT=${TARGET_OUT}
            ${NATIVETEST}/bionic-unit-tests/bionic-unit-tests $@
        )
    fi
fi
