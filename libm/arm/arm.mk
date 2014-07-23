ifneq (generic, $(TARGET_$(my_2nd_arch_prefix)_CPU_VARIANT))
libm_common_src_files_arm := \
    arm/s_floor.S \
    arm/e_sqrt.S \
    arm/e_sqrtf.S
else
libm_common_src_files_arm := libm_default_src_files
endif
