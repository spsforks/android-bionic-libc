LOCAL_PATH:= $(call my-dir)

# Define ANDROID_SMP appropriately.
ifeq ($(TARGET_CPU_SMP),true)
    libstdcxx_common_cflags := -DANDROID_SMP=1
else
    libstdcxx_common_cflags := -DANDROID_SMP=0
endif

# So we can access <private/libc_logging.h>
libstdcxx_common_c_includes := $(LOCAL_PATH)/../libc/

libstdcxx_common_src_files := \
    ../libc/bionic/__cxa_guard.cpp \
    ../libc/bionic/__cxa_pure_virtual.cpp \
    ../libc/bionic/new.cpp \
    ../libc/bionic/libc_logging.cpp \

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(libstdcxx_common_c_includes)
LOCAL_CFLAGS := $(libstdcxx_common_cflags)
LOCAL_SRC_FILES := $(libstdcxx_common_src_files)
LOCAL_MODULE:= libstdc++
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SYSTEM_SHARED_LIBRARIES := libc
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(libstdcxx_common_c_includes)
LOCAL_CFLAGS := $(libstdcxx_common_cflags)
LOCAL_SRC_FILES := $(libstdcxx_common_src_files)
LOCAL_MODULE:= libstdc++
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SYSTEM_SHARED_LIBRARIES := libc
include $(BUILD_STATIC_LIBRARY)
