ifneq (generic, $(TARGET_$(my_2nd_arch_prefix)_CPU_VARIANT))
libm_arch_src_files_arm := arm/s_floor.S
else
libm_arch_src_files_arm := libm_default_src_files
endif
