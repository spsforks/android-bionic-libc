# Generated by gensyscalls.py. Do not edit.
syscall_src :=
syscall_src += arch-arm/syscalls/__brk.S
syscall_src += arch-arm/syscalls/__epoll_pwait.S
syscall_src += arch-arm/syscalls/__exit.S
syscall_src += arch-arm/syscalls/__fcntl64.S
syscall_src += arch-arm/syscalls/__fstatfs64.S
syscall_src += arch-arm/syscalls/__getcpu.S
syscall_src += arch-arm/syscalls/__getcwd.S
syscall_src += arch-arm/syscalls/__getpriority.S
syscall_src += arch-arm/syscalls/__ioctl.S
syscall_src += arch-arm/syscalls/__llseek.S
syscall_src += arch-arm/syscalls/__mmap2.S
syscall_src += arch-arm/syscalls/__openat.S
syscall_src += arch-arm/syscalls/__ppoll.S
syscall_src += arch-arm/syscalls/__pselect6.S
syscall_src += arch-arm/syscalls/__ptrace.S
syscall_src += arch-arm/syscalls/__reboot.S
syscall_src += arch-arm/syscalls/__rt_sigaction.S
syscall_src += arch-arm/syscalls/__rt_sigpending.S
syscall_src += arch-arm/syscalls/__rt_sigprocmask.S
syscall_src += arch-arm/syscalls/__rt_sigsuspend.S
syscall_src += arch-arm/syscalls/__rt_sigtimedwait.S
syscall_src += arch-arm/syscalls/__sched_getaffinity.S
syscall_src += arch-arm/syscalls/__set_tid_address.S
syscall_src += arch-arm/syscalls/__set_tls.S
syscall_src += arch-arm/syscalls/__sigaction.S
syscall_src += arch-arm/syscalls/__statfs64.S
syscall_src += arch-arm/syscalls/__syslog.S
syscall_src += arch-arm/syscalls/__timer_create.S
syscall_src += arch-arm/syscalls/__timer_delete.S
syscall_src += arch-arm/syscalls/__timer_getoverrun.S
syscall_src += arch-arm/syscalls/__timer_gettime.S
syscall_src += arch-arm/syscalls/__timer_settime.S
syscall_src += arch-arm/syscalls/__waitid.S
syscall_src += arch-arm/syscalls/_exit.S
syscall_src += arch-arm/syscalls/accept.S
syscall_src += arch-arm/syscalls/acct.S
syscall_src += arch-arm/syscalls/bind.S
syscall_src += arch-arm/syscalls/cacheflush.S
syscall_src += arch-arm/syscalls/capget.S
syscall_src += arch-arm/syscalls/capset.S
syscall_src += arch-arm/syscalls/chdir.S
syscall_src += arch-arm/syscalls/chroot.S
syscall_src += arch-arm/syscalls/clock_getres.S
syscall_src += arch-arm/syscalls/clock_gettime.S
syscall_src += arch-arm/syscalls/clock_nanosleep.S
syscall_src += arch-arm/syscalls/clock_settime.S
syscall_src += arch-arm/syscalls/close.S
syscall_src += arch-arm/syscalls/connect.S
syscall_src += arch-arm/syscalls/delete_module.S
syscall_src += arch-arm/syscalls/dup.S
syscall_src += arch-arm/syscalls/dup3.S
syscall_src += arch-arm/syscalls/epoll_create1.S
syscall_src += arch-arm/syscalls/epoll_ctl.S
syscall_src += arch-arm/syscalls/eventfd.S
syscall_src += arch-arm/syscalls/execve.S
syscall_src += arch-arm/syscalls/faccessat.S
syscall_src += arch-arm/syscalls/fchdir.S
syscall_src += arch-arm/syscalls/fchmod.S
syscall_src += arch-arm/syscalls/fchmodat.S
syscall_src += arch-arm/syscalls/fchown.S
syscall_src += arch-arm/syscalls/fchownat.S
syscall_src += arch-arm/syscalls/fdatasync.S
syscall_src += arch-arm/syscalls/fgetxattr.S
syscall_src += arch-arm/syscalls/flistxattr.S
syscall_src += arch-arm/syscalls/flock.S
syscall_src += arch-arm/syscalls/fremovexattr.S
syscall_src += arch-arm/syscalls/fsetxattr.S
syscall_src += arch-arm/syscalls/fstat.S
syscall_src += arch-arm/syscalls/fstatat.S
syscall_src += arch-arm/syscalls/fsync.S
syscall_src += arch-arm/syscalls/ftruncate.S
syscall_src += arch-arm/syscalls/ftruncate64.S
syscall_src += arch-arm/syscalls/futex.S
syscall_src += arch-arm/syscalls/getdents.S
syscall_src += arch-arm/syscalls/getegid.S
syscall_src += arch-arm/syscalls/geteuid.S
syscall_src += arch-arm/syscalls/getgid.S
syscall_src += arch-arm/syscalls/getgroups.S
syscall_src += arch-arm/syscalls/getitimer.S
syscall_src += arch-arm/syscalls/getpeername.S
syscall_src += arch-arm/syscalls/getpgid.S
syscall_src += arch-arm/syscalls/getpid.S
syscall_src += arch-arm/syscalls/getppid.S
syscall_src += arch-arm/syscalls/getresgid.S
syscall_src += arch-arm/syscalls/getresuid.S
syscall_src += arch-arm/syscalls/getrlimit.S
syscall_src += arch-arm/syscalls/getrusage.S
syscall_src += arch-arm/syscalls/getsid.S
syscall_src += arch-arm/syscalls/getsockname.S
syscall_src += arch-arm/syscalls/getsockopt.S
syscall_src += arch-arm/syscalls/gettid.S
syscall_src += arch-arm/syscalls/gettimeofday.S
syscall_src += arch-arm/syscalls/getuid.S
syscall_src += arch-arm/syscalls/getxattr.S
syscall_src += arch-arm/syscalls/init_module.S
syscall_src += arch-arm/syscalls/inotify_add_watch.S
syscall_src += arch-arm/syscalls/inotify_init1.S
syscall_src += arch-arm/syscalls/inotify_rm_watch.S
syscall_src += arch-arm/syscalls/ioprio_get.S
syscall_src += arch-arm/syscalls/ioprio_set.S
syscall_src += arch-arm/syscalls/kill.S
syscall_src += arch-arm/syscalls/klogctl.S
syscall_src += arch-arm/syscalls/lgetxattr.S
syscall_src += arch-arm/syscalls/linkat.S
syscall_src += arch-arm/syscalls/listen.S
syscall_src += arch-arm/syscalls/listxattr.S
syscall_src += arch-arm/syscalls/llistxattr.S
syscall_src += arch-arm/syscalls/lremovexattr.S
syscall_src += arch-arm/syscalls/lseek.S
syscall_src += arch-arm/syscalls/lsetxattr.S
syscall_src += arch-arm/syscalls/madvise.S
syscall_src += arch-arm/syscalls/mincore.S
syscall_src += arch-arm/syscalls/mkdirat.S
syscall_src += arch-arm/syscalls/mknodat.S
syscall_src += arch-arm/syscalls/mlock.S
syscall_src += arch-arm/syscalls/mlockall.S
syscall_src += arch-arm/syscalls/mount.S
syscall_src += arch-arm/syscalls/mprotect.S
syscall_src += arch-arm/syscalls/mremap.S
syscall_src += arch-arm/syscalls/msync.S
syscall_src += arch-arm/syscalls/munlock.S
syscall_src += arch-arm/syscalls/munlockall.S
syscall_src += arch-arm/syscalls/munmap.S
syscall_src += arch-arm/syscalls/nanosleep.S
syscall_src += arch-arm/syscalls/perf_event_open.S
syscall_src += arch-arm/syscalls/personality.S
syscall_src += arch-arm/syscalls/pipe2.S
syscall_src += arch-arm/syscalls/prctl.S
syscall_src += arch-arm/syscalls/pread64.S
syscall_src += arch-arm/syscalls/pwrite64.S
syscall_src += arch-arm/syscalls/read.S
syscall_src += arch-arm/syscalls/readahead.S
syscall_src += arch-arm/syscalls/readlinkat.S
syscall_src += arch-arm/syscalls/readv.S
syscall_src += arch-arm/syscalls/recvfrom.S
syscall_src += arch-arm/syscalls/recvmsg.S
syscall_src += arch-arm/syscalls/removexattr.S
syscall_src += arch-arm/syscalls/renameat.S
syscall_src += arch-arm/syscalls/sched_get_priority_max.S
syscall_src += arch-arm/syscalls/sched_get_priority_min.S
syscall_src += arch-arm/syscalls/sched_getparam.S
syscall_src += arch-arm/syscalls/sched_getscheduler.S
syscall_src += arch-arm/syscalls/sched_rr_get_interval.S
syscall_src += arch-arm/syscalls/sched_setaffinity.S
syscall_src += arch-arm/syscalls/sched_setparam.S
syscall_src += arch-arm/syscalls/sched_setscheduler.S
syscall_src += arch-arm/syscalls/sched_yield.S
syscall_src += arch-arm/syscalls/sendfile.S
syscall_src += arch-arm/syscalls/sendfile64.S
syscall_src += arch-arm/syscalls/sendmsg.S
syscall_src += arch-arm/syscalls/sendto.S
syscall_src += arch-arm/syscalls/setgid.S
syscall_src += arch-arm/syscalls/setgroups.S
syscall_src += arch-arm/syscalls/setitimer.S
syscall_src += arch-arm/syscalls/setpgid.S
syscall_src += arch-arm/syscalls/setpriority.S
syscall_src += arch-arm/syscalls/setregid.S
syscall_src += arch-arm/syscalls/setresgid.S
syscall_src += arch-arm/syscalls/setresuid.S
syscall_src += arch-arm/syscalls/setreuid.S
syscall_src += arch-arm/syscalls/setrlimit.S
syscall_src += arch-arm/syscalls/setsid.S
syscall_src += arch-arm/syscalls/setsockopt.S
syscall_src += arch-arm/syscalls/settimeofday.S
syscall_src += arch-arm/syscalls/setuid.S
syscall_src += arch-arm/syscalls/setxattr.S
syscall_src += arch-arm/syscalls/shutdown.S
syscall_src += arch-arm/syscalls/sigaltstack.S
syscall_src += arch-arm/syscalls/signalfd4.S
syscall_src += arch-arm/syscalls/socket.S
syscall_src += arch-arm/syscalls/socketpair.S
syscall_src += arch-arm/syscalls/swapoff.S
syscall_src += arch-arm/syscalls/swapon.S
syscall_src += arch-arm/syscalls/symlinkat.S
syscall_src += arch-arm/syscalls/sync.S
syscall_src += arch-arm/syscalls/sysinfo.S
syscall_src += arch-arm/syscalls/tgkill.S
syscall_src += arch-arm/syscalls/timerfd_create.S
syscall_src += arch-arm/syscalls/timerfd_gettime.S
syscall_src += arch-arm/syscalls/timerfd_settime.S
syscall_src += arch-arm/syscalls/times.S
syscall_src += arch-arm/syscalls/tkill.S
syscall_src += arch-arm/syscalls/truncate.S
syscall_src += arch-arm/syscalls/truncate64.S
syscall_src += arch-arm/syscalls/umask.S
syscall_src += arch-arm/syscalls/umount2.S
syscall_src += arch-arm/syscalls/uname.S
syscall_src += arch-arm/syscalls/unlinkat.S
syscall_src += arch-arm/syscalls/unshare.S
syscall_src += arch-arm/syscalls/utimensat.S
syscall_src += arch-arm/syscalls/vfork.S
syscall_src += arch-arm/syscalls/wait4.S
syscall_src += arch-arm/syscalls/write.S
syscall_src += arch-arm/syscalls/writev.S
