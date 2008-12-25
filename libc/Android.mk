LOCAL_PATH:= $(call my-dir)

include $(LOCAL_PATH)/arch-$(TARGET_ARCH)/syscalls.mk
libc_common_src_files := \
	$(syscall_src) \
	unistd/abort.c \
	unistd/alarm.c \
	unistd/brk.c \
	unistd/creat.c \
	unistd/daemon.c \
	unistd/exec.c \
	unistd/fcntl.c \
	unistd/fnmatch.c \
	unistd/ftime.c \
	unistd/ftok.c \
	unistd/getcwd.c \
	unistd/getdtablesize.c \
	unistd/gethostname.c \
	unistd/getopt_long.c \
	unistd/getpgrp.c \
	unistd/getpriority.c \
	unistd/getpt.c \
	unistd/initgroups.c \
	unistd/isatty.c \
	unistd/issetugid.c \
	unistd/lseek64.c \
	unistd/mmap.c \
	unistd/nice.c \
	unistd/open.c \
	unistd/openat.c \
	unistd/opendir.c \
	unistd/pathconf.c \
	unistd/perror.c \
	unistd/popen.c \
	unistd/pread.c \
	unistd/pselect.c \
	unistd/ptsname.c \
	unistd/ptsname_r.c \
	unistd/pwrite.c \
	unistd/raise.c \
	unistd/reboot.c \
	unistd/recv.c \
	unistd/sbrk.c \
	unistd/send.c \
	unistd/setegid.c \
	unistd/seteuid.c \
	unistd/setpgrp.c \
	unistd/sigblock.c \
	unistd/siginterrupt.c \
	unistd/siglist.c \
	unistd/signal.c \
	unistd/sigsetmask.c \
	unistd/sigsuspend.c \
	unistd/sigwait.c \
	unistd/sleep.c \
	unistd/statfs.c \
	unistd/strsignal.c \
	unistd/sysconf.c \
	unistd/syslog.c \
	unistd/system.c \
	unistd/tcgetpgrp.c \
	unistd/tcsetpgrp.c \
	unistd/time.c \
	unistd/umount.c \
	unistd/unlockpt.c \
	unistd/usleep.c \
	unistd/wait.c \
	stdio/asprintf.c \
	stdio/fclose.c \
	stdio/fdopen.c \
	stdio/feof.c \
	stdio/ferror.c \
	stdio/fflush.c \
	stdio/fgetc.c \
	stdio/fgetln.c \
	stdio/fgetpos.c \
	stdio/fgets.c \
	stdio/fileno.c \
	stdio/findfp.c \
	stdio/flags.c \
	stdio/flockfile.c \
	stdio/fopen.c \
	stdio/fprintf.c \
	stdio/fpurge.c \
	stdio/fputc.c \
	stdio/fputs.c \
	stdio/fread.c \
	stdio/freopen.c \
	stdio/fscanf.c \
	stdio/fseek.c \
	stdio/fsetpos.c \
	stdio/ftell.c \
	stdio/funopen.c \
	stdio/fvwrite.c \
	stdio/fwalk.c \
	stdio/fwrite.c \
	stdio/getc.c \
	stdio/getchar.c \
	stdio/gets.c \
	stdio/makebuf.c \
	stdio/mktemp.c \
	stdio/printf.c \
	stdio/putc.c \
	stdio/putchar.c \
	stdio/puts.c \
	stdio/putw.c \
	stdio/refill.c \
	stdio/remove.c \
	stdio/rewind.c \
	stdio/rget.c \
	stdio/scanf.c \
	stdio/setbuf.c \
	stdio/setbuffer.c \
	stdio/setvbuf.c \
	stdio/snprintf.c\
	stdio/sprintf.c \
	stdio/sscanf.c \
	stdio/stdio.c \
	stdio/tempnam.c \
	stdio/tmpfile.c \
	stdio/tmpnam.c \
	stdio/ungetc.c \
	stdio/vasprintf.c \
	stdio/vfprintf.c \
	stdio/vfscanf.c \
	stdio/vprintf.c \
	stdio/vsnprintf.c \
	stdio/vsprintf.c \
	stdio/vscanf.c \
	stdio/vsscanf.c \
	stdio/wbuf.c \
	stdio/wsetup.c \
	stdlib/_rand48.c \
	stdlib/assert.c \
	stdlib/atexit.c \
	stdlib/atoi.c \
	stdlib/atol.c \
	stdlib/atoll.c \
	stdlib/bsearch.c \
	stdlib/ctype_.c \
	stdlib/div.c \
	stdlib/exit.c \
	stdlib/getenv.c \
	stdlib/jrand48.c \
	stdlib/ldiv.c \
	stdlib/lldiv.c \
	stdlib/locale.c \
	stdlib/lrand48.c \
	stdlib/mrand48.c \
	stdlib/nrand48.c \
	stdlib/putenv.c \
	stdlib/qsort.c \
	stdlib/seed48.c \
	stdlib/setenv.c \
	stdlib/setjmperr.c \
	stdlib/srand48.c \
	stdlib/strntoimax.c \
	stdlib/strntoumax.c \
	stdlib/strtod.c \
	stdlib/strtoimax.c \
	stdlib/strtol.c \
	stdlib/strtoll.c \
	stdlib/strtoul.c \
	stdlib/strtoull.c \
	stdlib/strtoumax.c \
	stdlib/tolower_.c \
	stdlib/toupper_.c \
	stdlib/wchar.c \
	string/bcopy.c \
	string/index.c \
	string/memccpy.c \
	string/memchr.c \
	string/memmem.c \
	string/memmove.c.arm \
	string/memrchr.c \
	string/memswap.c \
	string/strcasecmp.c \
	string/strcasestr.c \
	string/strcat.c \
	string/strchr.c \
	string/strcmp.c \
	string/strcpy.c \
	string/strcspn.c \
	string/strdup.c \
	string/strerror.c \
	string/strerror_r.c \
	string/strlcat.c \
	string/strlcpy.c \
	string/strncat.c \
	string/strncmp.c \
	string/strncpy.c \
	string/strndup.c \
	string/strnlen.c \
	string/strpbrk.c \
	string/strrchr.c \
	string/strsep.c \
	string/strspn.c \
	string/strstr.c \
	string/strtok.c \
	string/strtotimeval.c \
	inet/bindresvport.c \
	inet/inet_addr.c \
	inet/inet_aton.c \
	inet/inet_ntoa.c \
	inet/inet_ntop.c \
	inet/inet_pton.c \
	tzcode/asctime.c \
	tzcode/difftime.c \
	tzcode/localtime.c \
	tzcode/strftime.c \
	tzcode/strptime.c \
	bionic/__errno.c \
	bionic/__set_errno.c \
	bionic/_rand48.c \
	bionic/arc4random.c \
	bionic/basename.c \
	bionic/basename_r.c \
	bionic/dirname.c \
	bionic/dirname_r.c \
	bionic/drand48.c \
	bionic/erand48.c \
	bionic/fork.c \
	bionic/if_nametoindex.c \
	bionic/if_indextoname.c \
	bionic/ioctl.c \
	bionic/ldexp.c \
	bionic/libc_init_common.c \
	bionic/logd_write.c \
	bionic/md5.c \
	bionic/pututline.c \
	bionic/realpath.c \
	bionic/semaphore.c \
	bionic/sha1.c \
	bionic/ssp.c \
	bionic/stubs.c \
	bionic/system_properties.c \
	bionic/thread_atexit.c \
	bionic/utime.c \
	bionic/utmp.c \
	netbsd/gethnamaddr.c \
	netbsd/isc/ev_timers.c \
	netbsd/isc/ev_streams.c \
	netbsd/inet/nsap_addr.c \
	netbsd/resolv/__dn_comp.c \
	netbsd/resolv/__res_close.c \
	netbsd/resolv/__res_send.c \
	netbsd/resolv/herror.c \
	netbsd/resolv/res_comp.c \
	netbsd/resolv/res_data.c \
	netbsd/resolv/res_debug.c \
	netbsd/resolv/res_init.c \
	netbsd/resolv/res_mkquery.c \
	netbsd/resolv/res_query.c \
	netbsd/resolv/res_send.c \
	netbsd/resolv/res_state.c.arm \
	netbsd/resolv/res_cache.c \
	netbsd/net/nsdispatch.c \
	netbsd/net/getaddrinfo.c \
	netbsd/net/getnameinfo.c \
	netbsd/net/getservbyname.c \
	netbsd/net/getservent.c \
	netbsd/net/base64.c \
	netbsd/net/getservbyport.c \
	netbsd/nameser/ns_name.c \
	netbsd/nameser/ns_parse.c \
	netbsd/nameser/ns_ttl.c \
	netbsd/nameser/ns_netint.c \
	netbsd/nameser/ns_print.c \
	netbsd/nameser/ns_samedomain.c

ifeq ($(TARGET_ARCH),arm)
libc_common_src_files += \
	bionic/eabi.c \
	arch-arm/bionic/__get_pc.S \
	arch-arm/bionic/__get_sp.S \
	arch-arm/bionic/_exit_with_stack_teardown.S \
	arch-arm/bionic/_setjmp.S \
	arch-arm/bionic/atomics_arm.S \
	arch-arm/bionic/clone.S \
	arch-arm/bionic/kill.S \
	arch-arm/bionic/tkill.S \
	arch-arm/bionic/memcmp.S \
	arch-arm/bionic/memcmp16.S \
	arch-arm/bionic/memcpy.S \
	arch-arm/bionic/memset.S \
	arch-arm/bionic/setjmp.S \
	arch-arm/bionic/sigsetjmp.S \
	arch-arm/bionic/strlen.c.arm \
	arch-arm/bionic/syscall.S \
	unistd/socketcalls.c

# These files need to be arm so that gdbserver
# can set breakpoints in them without messing
# up any thumb code.
libc_common_src_files += \
	bionic/pthread.c.arm \
	bionic/pthread-timers.c.arm \
	bionic/ptrace.c.arm
else # !arm

ifeq ($(TARGET_ARCH),x86)
libc_common_src_files += \
	arch-x86/bionic/__get_sp.S \
	arch-x86/bionic/__get_tls.c \
	arch-x86/bionic/__set_tls.c \
	arch-x86/bionic/ipc_sysv.c \
	arch-x86/bionic/atomics_x86.S \
	arch-x86/bionic/clone.S \
	arch-x86/bionic/_exit_with_stack_teardown.S \
	arch-x86/bionic/setjmp.S \
	arch-x86/bionic/_setjmp.S \
	arch-x86/bionic/vfork.S \
	arch-x86/string/bzero.S \
	arch-x86/string/memset.S \
	arch-x86/string/memcmp.S \
	arch-x86/string/memcpy.S \
	arch-x86/string/strlen.S \
	bionic/pthread.c \
	bionic/pthread-timers.c \
	bionic/ptrace.c
endif # x86

endif # !arm

libc_common_cflags := \
		-DWITH_ERRLIST			\
		-DANDROID_CHANGES		\
		-DUSE_LOCKS 			\
		-DREALLOC_ZERO_BYTES_FREES 	\
		-D_LIBC=1 			\
		-DSOFTFLOAT                     \
		-DFLOATING_POINT		\
		-DNEED_PSELECT=1		\
		-DINET6

ifeq ($(TARGET_BUILD_TYPE),debug)
  libc_common_cflags += -DDEBUG
endif

ifeq ($(TARGET_ARCH),arm)
  libc_common_cflags += -fstrict-aliasing
endif

libc_common_c_includes := \
		$(LOCAL_PATH)/stdlib  \
		$(LOCAL_PATH)/string  \
		$(LOCAL_PATH)/stdio

# libc_common.a
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_common_src_files)
LOCAL_CFLAGS := $(libc_common_cflags) -DUSE_DL_PREFIX
LOCAL_C_INCLUDES := $(libc_common_c_includes)

ifneq ($(TARGET_SIMULATOR),true)
  ifeq ($(TARGET_ARCH),arm)
    crtend_target_cflags := -mthumb-interwork
  else
    ifeq ($(TARGET_ARCH),x86)
      crtend_target_cflags := -m32
    endif
  endif
# We rename crtend.o to crtend_android.o to avoid a
# name clash between gcc and bionic.
GEN := $(TARGET_OUT_STATIC_LIBRARIES)/crtend_android.o
$(GEN): $(LOCAL_PATH)/arch-$(TARGET_ARCH)/bionic/crtend.S
	@mkdir -p $(dir $@)
	$(TARGET_CC) $(crtend_target_cflags) -o $@ -c $<
ALL_GENERATED_SOURCES += $(GEN)
endif


# crtbegin_so.o/crtend_so.o
# These are needed for building the shared libs.
ifneq ($(TARGET_SIMULATOR),true)

ifeq ($(TARGET_ARCH),x86)

crt_begin_end_so_target_cflags := -m32

GEN := $(TARGET_OUT_STATIC_LIBRARIES)/crtbegin_so.o
$(GEN): $(LOCAL_PATH)/arch-$(TARGET_ARCH)/bionic/crtbegin_so.S
	@mkdir -p $(dir $@)
	$(TARGET_CC) $(crt_begin_end_so_target_cflags) -o $@ -c $<
ALL_GENERATED_SOURCES += $(GEN)

GEN := $(TARGET_OUT_STATIC_LIBRARIES)/crtend_so.o
$(GEN): $(LOCAL_PATH)/arch-$(TARGET_ARCH)/bionic/crtend_so.S
	@mkdir -p $(dir $@)
	$(TARGET_CC) $(crt_begin_end_so_target_cflags) -o $@ -c $<
ALL_GENERATED_SOURCES += $(GEN)

endif # TARGET_ARCH == x86

endif # !TARGET_SIMULATOR


LOCAL_MODULE := libc_common
LOCAL_SYSTEM_SHARED_LIBRARIES :=

include $(BUILD_STATIC_LIBRARY)


# libc.a
# ========================================================

include $(CLEAR_VARS)

include $(LOCAL_PATH)/arch-$(TARGET_ARCH)/syscalls.mk

# To enable malloc leak check for statically linked programs, add
# "WITH_MALLOC_CHECK_LIBC_A := true" to device/buildspec.mk
WITH_MALLOC_CHECK_LIBC_A := $(strip $(WITH_MALLOC_CHECK_LIBC_A))

LOCAL_SRC_FILES := \
	$(libc_common_src_files) \
	bionic/dlmalloc.c \
	bionic/libc_init_static.c

ifeq ($(WITH_MALLOC_CHECK_LIBC_A),true)
  LOCAL_SRC_FILES += bionic/malloc_leak.c.arm
endif

ifeq ($(TARGET_ARCH),arm)
LOCAL_SRC_FILES += \
	arch-arm/bionic/exidx_static.c

else # TARGET_ARCH != arm

ifeq ($(TARGET_ARCH),x86)
LOCAL_SRC_FILES += \
	arch-x86/bionic/dl_iterate_phdr_static.c
endif

endif

ifneq ($(TARGET_SIMULATOR),true)
  ifeq ($(TARGET_ARCH),arm)
    crtbegin_static_target_cflags := -mthumb-interwork
  else
    ifeq ($(TARGET_ARCH),x86)
      crtbegin_static_target_cflags := -m32
    endif
  endif
GEN := $(TARGET_OUT_STATIC_LIBRARIES)/crtbegin_static.o
$(GEN): $(LOCAL_PATH)/arch-$(TARGET_ARCH)/bionic/crtbegin_static.S
	@mkdir -p $(dir $@)
	$(TARGET_CC) $(crtbegin_static_target_cflags) -o $@ -c $<
ALL_GENERATED_SOURCES += $(GEN)
endif

LOCAL_CFLAGS := $(libc_common_cflags)

LOCAL_C_INCLUDES := $(libc_common_c_includes)

ifeq ($(WITH_MALLOC_CHECK_LIBC_A),true)
  LOCAL_CFLAGS += -DUSE_DL_PREFIX -DMALLOC_LEAK_CHECK
endif

LOCAL_WHOLE_STATIC_LIBRARIES := libc_common
LOCAL_MODULE:= libc
LOCAL_SYSTEM_SHARED_LIBRARIES :=

include $(BUILD_STATIC_LIBRARY)


# libc.so
# ========================================================

include $(CLEAR_VARS)

LOCAL_CFLAGS := $(libc_common_cflags)

LOCAL_CFLAGS += -DUSE_DL_PREFIX

LOCAL_C_INCLUDES := $(libc_common_c_includes)

LOCAL_SRC_FILES := \
	bionic/dlmalloc.c \
	bionic/malloc_leak.c.arm \
	bionic/libc_init_dynamic.c

ifeq ($(TARGET_ARCH),arm)
LOCAL_SRC_FILES += \
	arch-arm/bionic/exidx_dynamic.c
endif

LOCAL_MODULE:= libc

# WARNING: The only library libc.so should depend on is libdl.so!  If you add other libraries,
# make sure to add -Wl,--exclude-libs=libgcc.a to the LOCAL_LDFLAGS for those libraries.  This
# ensures that symbols that are pulled into those new libraries from libgcc.a are not declared
# external; if that were the case, then libc would not pull those symbols from libgcc.a as it
# should, instead relying on the external symbols from the dependent libraries.  That would
# create an "cloaked" dependency on libgcc.a in libc though the libraries, which is not what
# you wanted!

LOCAL_SHARED_LIBRARIES := libdl
LOCAL_WHOLE_STATIC_LIBRARIES := libc_common
LOCAL_SYSTEM_SHARED_LIBRARIES :=

ifneq ($(TARGET_SIMULATOR),true)
  ifeq ($(TARGET_ARCH),arm)
    crtbegin_dynamic_target_cflags := -mthumb-interwork
  else
    ifeq ($(TARGET_ARCH),x86)
      crtbegin_dynamic_target_cflags := -m32
    endif
  endif
GEN := $(TARGET_OUT_STATIC_LIBRARIES)/crtbegin_dynamic.o
$(GEN): $(LOCAL_PATH)/arch-$(TARGET_ARCH)/bionic/crtbegin_dynamic.S
	@mkdir -p $(dir $@)
	$(TARGET_CC) $(crtbegin_dynamic_target_cflags) -o $@ -c $<
ALL_GENERATED_SOURCES += $(GEN)
endif

include $(BUILD_SHARED_LIBRARY)


# libc_debug.so
# ========================================================

include $(CLEAR_VARS)

LOCAL_CFLAGS := $(libc_common_cflags)

LOCAL_CFLAGS += -DUSE_DL_PREFIX -DMALLOC_LEAK_CHECK

LOCAL_C_INCLUDES := $(libc_common_c_includes)

LOCAL_SRC_FILES := \
	bionic/dlmalloc.c \
	bionic/malloc_leak.c.arm \
	bionic/libc_init_dynamic.c

ifeq ($(TARGET_ARCH),arm)
LOCAL_SRC_FILES += \
	arch-arm/bionic/exidx_dynamic.c
endif

LOCAL_MODULE:= libc_debug

# WARNING: The only library libc.so should depend on is libdl.so!  If you add other libraries,
# make sure to add -Wl,--exclude-libs=libgcc.a to the LOCAL_LDFLAGS for those libraries.  This
# ensures that symbols that are pulled into those new libraries from libgcc.a are not declared
# external; if that were the case, then libc would not pull those symbols from libgcc.a as it
# should, instead relying on the external symbols from the dependent libraries.  That would
# create an "cloaked" dependency on libgcc.a in libc though the libraries, which is not what
# you wanted!

LOCAL_SHARED_LIBRARIES := libdl
LOCAL_WHOLE_STATIC_LIBRARIES := libc_common
LOCAL_SYSTEM_SHARED_LIBRARIES :=
# Don't prelink
LOCAL_PRELINK_MODULE := false
# Don't install on release build
LOCAL_MODULE_TAGS := eng

GEN := $(TARGET_OUT_STATIC_LIBRARIES)/crtbegin_dynamic.o
ALL_GENERATED_SOURCES += $(GEN)

include $(BUILD_SHARED_LIBRARY)

# ========================================================
include $(call all-makefiles-under,$(LOCAL_PATH))
