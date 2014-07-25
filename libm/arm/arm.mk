# ARMv7 processor with VFPv3 and NEON support
ifneq (generic, $(TARGET_$(my_2nd_arch_prefix)_CPU_VARIANT))
libm_common_src_files_arm := \
    upstream-freebsd/lib/msun/src/s_ceil.c \
    upstream-freebsd/lib/msun/src/s_ceilf.c \
    arm/s_floor.S \
    upstream-freebsd/lib/msun/src/s_floorf.c \
    arm/e_sqrt.S \
    arm/e_sqrtf.S
else
libm_common_src_files_arm := libm_default_src_files
endif
