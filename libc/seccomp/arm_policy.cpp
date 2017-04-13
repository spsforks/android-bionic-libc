// Autogenerated file - edit at your peril!!

#include <linux/filter.h>
#include <errno.h>

#include "seccomp_bpfs.h"
const sock_filter arm_filter[] = {
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 0, 0, 126),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 143, 63, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 74, 31, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 41, 15, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 24, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 10, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 8, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 7, 119, 118), //restart_syscall|exit|fork|read|write|open|close
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 9, 118, 117), //creat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 19, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 13, 116, 115), //unlink|execve|chdir
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 22, 115, 114), //lseek|getpid|mount
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 33, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 26, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 25, 112, 111), //getuid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 27, 111, 110), //ptrace
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 36, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 34, 109, 108), //access
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 39, 108, 107), //sync|kill|rename
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 57, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 51, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 45, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 44, 104, 103), //dup|pipe|times
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 46, 103, 102), //brk
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 54, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 53, 101, 100), //acct|umount2
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 56, 100, 99), //ioctl|fcntl
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 64, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 60, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 58, 97, 96), //setpgid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 62, 96, 95), //umask|chroot
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 66, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 65, 94, 93), //getppid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 68, 93, 92), //setsid|sigaction
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 114, 15, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 91, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 85, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 77, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 76, 88, 87), //sethostname|setrlimit
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 80, 87, 86), //getrusage|gettimeofday|settimeofday
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 88, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 86, 85, 84), //readlink
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 89, 84, 83), //reboot
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 96, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 94, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 93, 81, 80), //munmap|truncate
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 95, 80, 79), //fchmod
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 103, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 98, 78, 77), //getpriority|setpriority
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 106, 77, 76), //syslog|setitimer|getitimer
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 128, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 118, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 116, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 115, 73, 72), //wait4
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 117, 72, 71), //sysinfo
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 124, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 123, 70, 69), //fsync|sigreturn|clone|setdomainname|uname
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 126, 69, 68), //adjtimex|mprotect
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 136, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 131, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 130, 66, 65), //init_module|delete_module
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 134, 65, 64), //quotactl|getpgid|fchdir
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 138, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 137, 63, 62), //personality
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 142, 62, 61), //setfsuid|setfsgid|_llseek|getdents
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 286, 31, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 217, 15, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 183, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 168, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 150, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 149, 56, 55), //flock|msync|readv|writev|getsid|fdatasync
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 164, 55, 54), //mlock|munlock|mlockall|munlockall|sched_setparam|sched_getparam|sched_setscheduler|sched_getscheduler|sched_yield|sched_get_priority_max|sched_get_priority_min|sched_rr_get_interval|nanosleep|mremap
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 172, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 169, 53, 52), //poll
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 182, 52, 51), //prctl|rt_sigreturn|rt_sigaction|rt_sigprocmask|rt_sigpending|rt_sigtimedwait|rt_sigqueueinfo|rt_sigsuspend|pread64|pwrite64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 199, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 190, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 188, 49, 48), //getcwd|capget|capset|sigaltstack|sendfile
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 198, 48, 47), //vfork|ugetrlimit|mmap2|truncate64|ftruncate64|stat64|lstat64|fstat64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 213, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 212, 46, 45), //getuid32|getgid32|geteuid32|getegid32|setreuid32|setregid32|getgroups32|setgroups32|fchown32|setresuid32|getresuid32|setresgid32|getresgid32
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 215, 45, 44), //setuid32|setgid32
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 250, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 224, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 219, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 218, 41, 40), //getdents64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 222, 40, 39), //mincore|madvise|fcntl64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 248, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 243, 38, 37), //gettid|readahead|setxattr|lsetxattr|fsetxattr|getxattr|lgetxattr|fgetxattr|listxattr|llistxattr|flistxattr|removexattr|lremovexattr|fremovexattr|tkill|sendfile64|futex|sched_setaffinity|sched_getaffinity
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 249, 37, 36), //exit_group
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 270, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 256, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 254, 34, 33), //epoll_create|epoll_ctl|epoll_wait|remap_file_pages
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 269, 33, 32), //set_tid_address|timer_create|timer_settime|timer_gettime|timer_getoverrun|timer_delete|clock_settime|clock_gettime|clock_getres|clock_nanosleep|statfs64|fstatfs64|tgkill
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 280, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 271, 31, 30), //arm_fadvise64_64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 285, 30, 29), //waitid|socket|bind|connect|listen
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 348, 15, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 322, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 292, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 290, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 289, 25, 24), //getsockname|getpeername|socketpair
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 291, 24, 23), //sendto
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 316, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 298, 22, 21), //recvfrom|shutdown|setsockopt|getsockopt|sendmsg|recvmsg
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 319, 21, 20), //inotify_init|inotify_add_watch|inotify_rm_watch
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 340, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 327, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 326, 18, 17), //openat|mkdirat|mknodat|fchownat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 338, 17, 16), //fstatat64|unlinkat|renameat|linkat|symlinkat|readlinkat|fchmodat|faccessat|pselect6|ppoll|unshare
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 345, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 344, 15, 14), //splice|sync_file_range2|tee|vmsplice
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 347, 14, 13), //getcpu|epoll_pwait
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 383, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 369, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 350, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 349, 10, 9), //utimensat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 367, 9, 8), //timerfd_create|eventfd|fallocate|timerfd_settime|timerfd_gettime|signalfd4|eventfd2|epoll_create1|dup3|pipe2|inotify_init1|preadv|pwritev|rt_tgsigqueueinfo|perf_event_open|recvmmsg|accept4
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 372, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 370, 7, 6), //prlimit64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 378, 6, 5), //clock_adjtime|syncfs|sendmmsg|setns|process_vm_readv|process_vm_writev
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 983045, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 983042, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 386, 3, 2), //seccomp|getrandom|memfd_create
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 983043, 2, 1), //__ARM_NR_cacheflush
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 983046, 1, 0), //__ARM_NR_set_tls
BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW),
};

const size_t arm_filter_size = sizeof(arm_filter) / sizeof(struct sock_filter);
