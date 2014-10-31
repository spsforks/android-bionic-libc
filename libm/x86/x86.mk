x86_exclude_files := \
	upstream-freebsd/lib/msun/src/e_acos.c \
	upstream-freebsd/lib/msun/src/e_asin.c \
	upstream-freebsd/lib/msun/src/e_atan2.c \
	upstream-freebsd/lib/msun/src/e_cosh.c \
	upstream-freebsd/lib/msun/src/e_exp.c \
	upstream-freebsd/lib/msun/src/e_hypot.c \
	upstream-freebsd/lib/msun/src/e_log.c \
	upstream-freebsd/lib/msun/src/e_log10.c \
	upstream-freebsd/lib/msun/src/e_pow.c \
	upstream-freebsd/lib/msun/src/e_sinh.c \
	upstream-freebsd/lib/msun/src/s_atan.c \
	upstream-freebsd/lib/msun/src/s_cbrt.c \
	upstream-freebsd/lib/msun/src/s_cos.c \
	upstream-freebsd/lib/msun/src/s_expm1.c \
	upstream-freebsd/lib/msun/src/s_log1p.c \
	upstream-freebsd/lib/msun/src/s_sin.c \
	upstream-freebsd/lib/msun/src/s_tan.c \
	upstream-freebsd/lib/msun/src/s_tanh.c \

x86_replace_files := \
	x86/e_acos.S \
	x86/e_asin.S \
	x86/e_atan2.S \
	x86/e_cosh.S \
	x86/e_exp.S \
	x86/e_hypot.S \
	x86/e_log10.S \
	x86/e_log.S \
	x86/e_pow.S \
	x86/e_sinh.S \
	x86/libm_reduce_pi04l.S \
	x86/libm_sincos_huge.S \
	x86/libm_tancot_huge.S \
	x86/s_atan.S \
	x86/s_cbrt.S \
	x86/s_cos.S \
	x86/s_expm1.S \
	x86/s_log1p.S \
	x86/s_sin.S \
	x86/s_tanh.S \
	x86/s_tan.S \

LOCAL_SRC_FILES := $(filter-out $(x86_exclude_files),$(LOCAL_SRC_FILES))
LOCAL_C_INCLUDES_x86 += $(LOCAL_PATH)/../libc
LOCAL_SRC_FILES_x86 += $(x86_replace_files)
