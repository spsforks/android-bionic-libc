LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MODULE := libseccomp_policy
LOCAL_CLANG := true
LOCAL_SRC_FILES := seccomp_policy.cpp \
		   arm_policy.c \
		   arm64_policy.c \
		   mips_policy.c \
		   mips64_policy.c \
		   x86_policy.c \
		   x86_64_policy.c \

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
LOCAL_CFLAGS := -DTARGET_ARCH_$(TARGET_ARCH) -DTARGET_2ND_ARCH_$(TARGET_2ND_ARCH)

include $(BUILD_STATIC_LIBRARY)

