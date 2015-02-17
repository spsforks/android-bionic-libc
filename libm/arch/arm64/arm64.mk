arm_exclude_files := \

arm64_exclude_files := \
    upstream-freebsd/lib/msun/src/s_llrint.c \
    upstream-freebsd/lib/msun/src/s_llrintf.c \
    upstream-freebsd/lib/msun/src/s_lrint.c \
    upstream-freebsd/lib/msun/src/s_lrintf.c \
    upstream-freebsd/lib/msun/src/s_rint.c \
    upstream-freebsd/lib/msun/src/s_rintf.c \
    upstream-freebsd/lib/msun/src/e_sqrt.c \
    upstream-freebsd/lib/msun/src/e_sqrtf.c \
    upstream-freebsd/lib/msun/src/s_fma.c \
    upstream-freebsd/lib/msun/src/s_fmaf.c \
    upstream-freebsd/lib/msun/src/s_ceil.c \
    upstream-freebsd/lib/msun/src/s_ceilf.c \
    upstream-freebsd/lib/msun/src/s_floor.c \
    upstream-freebsd/lib/msun/src/s_floorf.c \
    upstream-freebsd/lib/msun/src/s_trunc.c \
    upstream-freebsd/lib/msun/src/s_truncf.c \

LOCAL_SRC_FILES_32 := \
    $(libm_common_src_files) \
    $(libm_32_src_files)

LOCAL_SRC_FILES_64 := \
    $(filter-out $(arm64_exclude_files),$(libm_common_src_files) $(libm_64_src_files)) \
    arch/arm64/round.c \
    arch/arm64/sqrt.c \
    arch/arm64/fma.c \


