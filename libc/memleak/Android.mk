LOCAL_PATH := $(call my-dir)

memleak_srcs := \
   Allocator.cpp \
   HeapWalker.cpp \
   LeakPipe.cpp \
   LineBuffer.cpp \
   Memleak.cpp \
   ProcessMappings.cpp \
   PtracerThread.cpp \
   ThreadCapture.cpp \

memleak_test_srcs := \
   tests/Allocator_test.cpp \
   tests/HeapWalker_test.cpp \
   tests/Memleak_test.cpp \
   tests/ThreadCapture_test.cpp \

include $(CLEAR_VARS)

LOCAL_MODULE := libmemleak
LOCAL_SRC_FILES := $(memleak_srcs)
LOCAL_CFLAGS := -std=c++14 -Wall -Wextra -Werror
LOCAL_C_INCLUDES := bionic/libc external/jemalloc/include
LOCAL_CLANG := true

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := memleak_test
LOCAL_SRC_FILES := $(memleak_test_srcs)
LOCAL_CFLAGS := -std=c++14 -Wall -Wextra -Werror
LOCAL_CLANG := true
LOCAL_STATIC_LIBRARIES := libmemleak
LOCAL_C_INCLUDES := bionic/libc external/jemalloc/include
LOCAL_FORCE_STATIC_EXECUTABLE := true

include $(BUILD_NATIVE_TEST)
