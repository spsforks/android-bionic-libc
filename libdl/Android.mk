LOCAL_PATH:= $(call my-dir)

include bionic/bionic-host.mk

#
# libdl
#

# NOTE: --exclude-libs=libgcc.a makes sure that any symbols libdl.so pulls from
# libgcc.a are made static to libdl.so.  This in turn ensures that libraries that
# a) pull symbols from libgcc.a and b) depend on libdl.so will not rely on libdl.so
# to provide those symbols, but will instead pull them from libgcc.a.  Specifically,
# we use this property to make sure libc.so has its own copy of the code from
# libgcc.a it uses.
libdl_ldflags := -Wl,--exclude-libs=libgcc.a
ifeq ($(TARGET_ARCH),x86)
  # For x86, exclude libgcc_eh.a for the same reasons.
  libdl_ldflags += -Wl,--exclude-libs=libgcc_eh.a
endif

# NOTE: libdl needs __aeabi_unwind_cpp_pr0 from libgcc.a but libgcc.a needs a
# few symbols from libc. Using --no-undefined here results in having to link
# against libc creating a circular dependency which is removed and we end up
# with missing symbols. Since this library is just a bunch of stubs, we set
# LOCAL_ALLOW_UNDEFINED_SYMBOLS to remove --no-undefined from the linker flags.
libdl_allow_undefined_symbols := true
libdl_system_shared_libraries :=

#
# libdl.so for target.
#
include $(CLEAR_VARS)
LOCAL_LDFLAGS := $(local_ldflags)
LOCAL_SRC_FILES:= libdl.c
LOCAL_MODULE:= libdl
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_ALLOW_UNDEFINED_SYMBOLS := $(libdl_allow_undefined_symbols)
LOCAL_SYSTEM_SHARED_LIBRARIES := $(libdl_system_shared_libraries)
include $(BUILD_SHARED_LIBRARY)

#
# libdl.so for host bionic.
#
include $(CLEAR_VARS)
LOCAL_LDFLAGS := $(local_ldflags)
LOCAL_SRC_FILES:= libdl.c
LOCAL_MODULE:= libdl
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_ALLOW_UNDEFINED_SYMBOLS := $(libdl_allow_undefined_symbols)
LOCAL_SYSTEM_SHARED_LIBRARIES := $(libdl_system_shared_libraries)
include $(BUILD_BIONIC_HOST_SHARED_LIBRARY)

#
# dltest
#
BUILD_DLTEST:=0
ifeq ($(BUILD_DLTEST),1)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= dltest.c
LOCAL_MODULE:= dltest
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SHARED_LIBRARIES := libdl
include $(BUILD_EXECUTABLE)
endif
