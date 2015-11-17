LOCAL_PATH := $(call my-dir)

#bionic_coverage ?= false

libc_malloc_debug_src_files := \
    backtrace.cpp \
	malloc_debug.cpp \
	MallocDebugConfig.cpp \
	mapinfo.cpp \

# ==============================================================
# libc_malloc_debug.so
# ==============================================================
include $(CLEAR_VARS)

LOCAL_MODULE := libc_malloc_debug

LOCAL_SRC_FILES := $(libc_malloc_debug_src_files)
LOCAL_CXX_STL := none

# Only need this for arm since libc++ uses its own unwind code that
# doesn't mix with the other default unwind code.
LOCAL_STATIC_LIBRARIES_arm := libunwind_llvm
LOCAL_STATIC_LIBRARIES += libc++abi libc++_static libc_logging
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_C_INCLUDES += bionic/libc

LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

LOCAL_CFLAGS := \
	-Wall \
	-Werror \
	-O0 -g

ifeq ($(TARGET_BUILD_VARIANT),user)
LOCAL_CFLAGS += -DUSER_BUILD
endif

include $(BUILD_SHARED_LIBRARY)

libc_malloc_debug_test_src_files := \
    tests/backtrace_fake.cpp \
    tests/log_fake.cpp \
	tests/libc_fake.cpp \
    tests/property_fake.cpp \
    tests/malloc_debug_config_tests.cpp \
	tests/malloc_debug_tests.cpp \
	malloc_debug.cpp \
	MallocDebugConfig.cpp \
	mapinfo.cpp \

# ==============================================================
# ==============================================================
include $(CLEAR_VARS)

LOCAL_MODULE := malloc_debug_tests
LOCAL_MODULE_STEM_32 := $(LOCAL_MODULE)32
LOCAL_MODULE_STEM_64 := $(LOCAL_MODULE)64

LOCAL_SRC_FILES := $(libc_malloc_debug_test_src_files)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/tests
LOCAL_C_INCLUDES += bionic/libc

LOCAL_SHARED_LIBRARIES := libbase

LOCAL_CFLAGS := \
	-Wall \
	-Werror \
	-O0 -g

include $(BUILD_NATIVE_TEST)

# ==============================================================
# ==============================================================
include $(CLEAR_VARS)

LOCAL_MODULE := malloc_debug_tests
LOCAL_MODULE_STEM_32 := $(LOCAL_MODULE)32
LOCAL_MODULE_STEM_64 := $(LOCAL_MODULE)64

LOCAL_SRC_FILES := $(libc_malloc_debug_test_src_files)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/tests
LOCAL_C_INCLUDES += bionic/libc

LOCAL_STATIC_LIBRARIES := libc_logging libbase
#LOCAL_SHARED_LIBRARIES := libbase
LOCAL_CFLAGS := -Wall -Werror -O0 -g

#include $(BUILD_HOST_NATIVE_TEST)
