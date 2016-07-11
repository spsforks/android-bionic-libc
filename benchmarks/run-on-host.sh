#!/bin/bash

. $(dirname $0)/../build/run-on-host.sh

if [ ${HOST_OS}-${HOST_ARCH} = linux-x86 -o ${HOST_OS}-${HOST_ARCH} = linux-x86_64 ]; then

    prepare bionic-benchmarks

    if [ ${TARGET_ARCH} = x86 -o ${TARGET_ARCH} = x86_64 ]; then
        (
            cd ${ANDROID_BUILD_TOP}
            export ANDROID_DATA=${TARGET_OUT_DATA}
            export ANDROID_ROOT=${TARGET_OUT}
            ${NATIVETEST}/bionic-benchmarks/bionic-benchmarks32 $@
        )
    fi
fi
