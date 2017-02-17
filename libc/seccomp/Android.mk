LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MODULE := libseccomp_policy
LOCAL_CLANG := true
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
LOCAL_SHARED_LIBRARIES := libbase
LOCAL_SRC_FILES := seccomp_policy.cpp \
		   arm_policy.cpp \
		   arm64_policy.cpp \
		   mips_policy.cpp \
		   mips64_policy.cpp \
		   x86_policy.cpp \
		   x86_64_policy.cpp \

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
LOCAL_CFLAGS := -DTARGET_ARCH_$(TARGET_ARCH) -DTARGET_2ND_ARCH_$(TARGET_2ND_ARCH)

include $(BUILD_STATIC_LIBRARY)

