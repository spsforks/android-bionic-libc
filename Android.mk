LOCAL_PATH := $(call my-dir)

# $(1): bionic module name (e.g. libc)
# $(2): source to the mountpoint file
# $(3): lib or bin
define install-bionic-mountpoint
include $$(CLEAR_VARS)
LOCAL_MODULE := $(1).mountpoint
LOCAL_SRC_FILES_32 := $(2)
LOCAL_SRC_FILES_64 := $(2)
LOCAL_USE_CLANG_LLD := false
ifeq ($(3),lib)
  LOCAL_MODULE_CLASS := SHARED_LIBRARIES
  LOCAL_MODULE_PATH_32 := $$(TARGET_ROOT_OUT)/bionic/lib
  LOCAL_MODULE_PATH_64 := $$(TARGET_ROOT_OUT)/bionic/lib64
  LOCAL_MODULE_STEM := $(1).so
  LOCAL_POST_INSTALL_CMD_32 := \
    $$(hide) mkdir -p $$(TARGET_OUT)/lib \
    && ln -sf /bionic/lib/$(1) $$(TARGET_OUT)/lib/$(1).so
  LOCAL_POST_INSTALL_CMD_64 := \
    $$(hide) mkdir -p $$(TARGET_OUT)/lib64 \
    && ln -sf /bionic/lib64/$(1) $$(TARGET_OUT)/lib64/$(1).so
else ifeq ($(3),bin)
  LOCAL_MODULE_CLASS := EXECUTABLES
  LOCAL_MODULE_PATH := $$(TARGET_ROOT_OUT)/bionic/bin
  LOCAL_MODULE_STEM_32 := $(1)
  LOCAL_MODULE_STEM_64 := $(1)64
  LOCAL_POST_INSTALL_CMD_32 := \
    $$(hide) mkdir -p $$(TARGET_OUT)/bin \
    && ln -sf /bionic/bin/$(1) $(TARGET_OUT)/bin/$(1)
  LOCAL_POST_INSTALL_CMD_64 := \
    $$(hide) mkdir -p $$(TARGET_OUT)/bin \
    && ln -sf /bionic/bin/$(1)64 $$(TARGET_OUT)/bin/$(1)64
endif
ifeq ($$(TARGET_TRANSLATE_2ND_ARCH),true)
LOCAL_MULTILIB := first
else
LOCAL_MULTILIB := both
endif
include $$(BUILD_PREBUILT)
endef

$(eval $(call install-bionic-mountpoint,libc,dummy_mountpoint,lib))
$(eval $(call install-bionic-mountpoint,libm,dummy_mountpoint,lib))
$(eval $(call install-bionic-mountpoint,libdl,dummy_mountpoint,lib))
$(eval $(call install-bionic-mountpoint,linker,dummy_mountpoint,bin))

include $(call all-makefiles-under,$(LOCAL_PATH))

