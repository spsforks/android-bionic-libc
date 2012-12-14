LOCAL_PATH:= $(call my-dir)

include bionic/bionic-host.mk

# Common C++ flags to build this library.
# Note that we need to access private Bionic headers
# and define ANDROID_SMP accordingly.
libstdc++_cflags := -Ibionic/libc/
ifeq ($(TARGET_CPU_SMP),false)
  libstdc++_cflags += -DANDROID_SMP=0
else
  libstdc++_cflags += -DANDROID_SMP=1
endif

libstdc++_src_files := \
    src/one_time_construction.cpp \
    src/new.cpp \
    src/pure_virtual.cpp \
    src/typeinfo.cpp

#
# libstdc++.a for target.
#
include $(CLEAR_VARS)
LOCAL_MODULE:= libstdc++
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_CFLAGS := $(libstdc++_cflags)
LOCAL_SRC_FILES := $(libstdc++_src_files)
LOCAL_SYSTEM_SHARED_LIBRARIES := libc
include $(BUILD_STATIC_LIBRARY)

#
# libstdc++.so for target.
#
include $(CLEAR_VARS)
LOCAL_MODULE:= libstdc++
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SYSTEM_SHARED_LIBRARIES := libc
LOCAL_WHOLE_STATIC_LIBRARIES := libstdc++
include $(BUILD_SHARED_LIBRARY)

#
# libstdc++.so for host.
#
include $(CLEAR_VARS)
LOCAL_MODULE:= libstdc++
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_CFLAGS := $(libstdc++_cflags)
LOCAL_SRC_FILES := $(libstdc++_src_files)
LOCAL_SYSTEM_SHARED_LIBRARIES := libc
include $(BUILD_BIONIC_HOST_SHARED_LIBRARY)
