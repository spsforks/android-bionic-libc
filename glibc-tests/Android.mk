LOCAL_PATH:= $(call my-dir)

#	$(eval LOCAL_SDK_VERSION := 23) \

$(foreach file,$(filter-out test-skeleton.c,$(call all-c-files-under,.)), \
	$(eval include $(CLEAR_VARS)) \
	$(eval LOCAL_CFLAGS := -std=gnu11) \
	$(eval LOCAL_SRC_FILES := $(file)) \
	$(eval LOCAL_MODULE := $(notdir $(file:%.c=%))) \
	$(eval include $(BUILD_NATIVE_TEST)) \
 )
