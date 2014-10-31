include $(LOCAL_PATH)/x86/x86.mk

x86_64_replace_files := \
	x86_64/e_acos.S \
	x86_64/e_asin.S \
	x86_64/e_atan2.S \
	x86_64/e_cosh.S \
	x86_64/e_exp.S \
	x86_64/e_hypot.S \
	x86_64/e_log10.S \
	x86_64/e_log.S \
	x86_64/e_pow.S \
	x86_64/e_sinh.S \
	x86_64/s_atan.S \
	x86_64/s_cbrt.S \
	x86_64/s_cos.S \
	x86_64/s_expm1.S \
	x86_64/s_log1p.S \
	x86_64/s_sin.S \
	x86_64/s_tanh.S \
	x86_64/s_tan.S \

LOCAL_SRC_FILES := $(filter-out $(x86_exclude_files),$(LOCAL_SRC_FILES))
LOCAL_C_INCLUDES_x86_64 += $(LOCAL_PATH)/../libc
LOCAL_SRC_FILES_x86_64 += $(x86_64_replace_files)
