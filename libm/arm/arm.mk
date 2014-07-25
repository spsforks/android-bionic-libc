# ARMv7 processor with VFPv3 and NEON support
ifneq (generic, $(TARGET_$(my_2nd_arch_prefix)CPU_VARIANT))
libm_arch_src_files_arm := \
    arm/s_floor.S \
    arm/e_sqrt.S \
    arm/e_sqrtf.S\
    upstream-freebsd/lib/msun/src/s_ceil.c \
    upstream-freebsd/lib/msun/src/s_ceilf.c \
    upstream-freebsd/lib/msun/src/s_floorf.c
else
libm_arch_src_files_arm := $(libm_arch_src_files_default)
endif
