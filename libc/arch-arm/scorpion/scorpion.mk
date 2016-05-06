# Use krait versions of memset/strcmp/memmove
libc_bionic_src_files_arm += \
    arch-arm/krait/bionic/memset.S \
    arch-arm/krait/bionic/strcmp.S \
    arch-arm/scorpion/bionic/memmove.S

# Use cortex-a15 versions of memcpy/strcat/strcpy/strlen
libc_bionic_src_files_arm += \
    arch-arm/scorpion/bionic/memchr.S \
    arch-arm/cortex-a15/bionic/memcpy.S \
    arch-arm/cortex-a15/bionic/stpcpy.S \
    arch-arm/cortex-a15/bionic/strcat.S \
    arch-arm/cortex-a15/bionic/strcpy.S \
    arch-arm/cortex-a15/bionic/strlen.S \
    arch-arm/cortex-a15/bionic/__strcat_chk.S \
    arch-arm/cortex-a15/bionic/__strcpy_chk.S \
	
bionic_filter_files := bionic/memchr.c
libc_bionic_src_files_arm := $(filter-out $(bionic_filter_files), $(libc_bionic_src_files_arm))
openbsd_filter_files := upstream-openbsd/lib/libc/string/bcopy.c
libc_openbsd_src_files_arm := $(filter-out $(openbsd_filter_files), $(libc_openbsd_src_files_arm))