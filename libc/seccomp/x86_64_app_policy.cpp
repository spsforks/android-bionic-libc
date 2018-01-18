// Autogenerated file - edit at your peril!!

#include <linux/filter.h>
#include <errno.h>

#include "seccomp_bpfs.h"
const sock_filter x86_64_app_filter[] = {
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 0, 0, 102),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 157, 51, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 93, 25, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 38, 13, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 24, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 5, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 3, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 2, 95, 94), //read|write
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4, 94, 93), //close
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 8, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 6, 92, 91), //fstat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 21, 91, 90), //lseek|mmap|mprotect|munmap|brk|rt_sigaction|rt_sigprocmask|rt_sigreturn|ioctl|pread64|pwrite64|readv|writev
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 35, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 32, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 29, 88, 87), //sched_yield|mremap|msync|mincore|madvise
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 33, 87, 86), //dup
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 37, 86, 85), //nanosleep|getitimer
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 72, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 58, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 44, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 43, 82, 81), //setitimer|getpid|sendfile|socket|connect
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 57, 81, 80), //sendto|recvfrom|sendmsg|recvmsg|shutdown|bind|listen|getsockname|getpeername|socketpair|setsockopt|getsockopt|clone
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 64, 80, 79), //vfork|execve|exit|wait4|kill|uname
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 91, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 79, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 78, 77, 76), //fcntl|flock|fsync|fdatasync|truncate|ftruncate
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 82, 76, 75), //getcwd|chdir|fchdir
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 92, 75, 74), //fchmod
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 124, 13, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 114, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 104, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 95, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 94, 70, 69), //fchown
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 103, 69, 68), //umask|gettimeofday|getrlimit|getrusage|sysinfo|times|ptrace|getuid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 107, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 105, 67, 66), //getgid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 111, 66, 65), //geteuid|getegid|setpgid|getppid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 120, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 118, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 116, 63, 62), //setregid|getgroups
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 119, 62, 61), //getresuid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 122, 61, 60), //getresgid|getpgid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 137, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 135, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 127, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 126, 57, 56), //getsid|capget
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 132, 56, 55), //rt_sigpending|rt_sigtimedwait|rt_sigqueueinfo|rt_sigsuspend|sigaltstack
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 136, 55, 54), //personality
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 155, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 140, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 139, 52, 51), //statfs|fstatfs
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 153, 51, 50), //getpriority|setpriority|sched_setparam|sched_getparam|sched_setscheduler|sched_getscheduler|sched_get_priority_max|sched_get_priority_min|sched_rr_get_interval|mlock|munlock|mlockall|munlockall
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 156, 50, 49), //pivot_root
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 254, 25, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 217, 13, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 186, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 162, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 160, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 159, 44, 43), //prctl|arch_prctl
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 161, 43, 42), //setrlimit
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 179, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 163, 41, 40), //sync
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 180, 40, 39), //quotactl
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 206, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 202, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 201, 37, 36), //gettid|readahead|setxattr|lsetxattr|fsetxattr|getxattr|lgetxattr|fgetxattr|listxattr|llistxattr|flistxattr|removexattr|lremovexattr|fremovexattr|tkill
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 205, 36, 35), //futex|sched_setaffinity|sched_getaffinity
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 211, 35, 34), //io_setup|io_destroy|io_getevents|io_submit|io_cancel
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 233, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 228, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 221, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 220, 31, 30), //getdents64|set_tid_address|restart_syscall
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 227, 30, 29), //fadvise64|timer_create|timer_settime|timer_gettime|timer_getoverrun|timer_delete
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 232, 29, 28), //clock_gettime|clock_getres|clock_nanosleep|exit_group
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 251, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 247, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 235, 26, 25), //epoll_ctl|tgkill
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 248, 25, 24), //waitid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 253, 24, 23), //ioprio_set|ioprio_get
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 285, 11, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 275, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 262, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 257, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 256, 19, 18), //inotify_add_watch|inotify_rm_watch
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 261, 18, 17), //openat|mkdirat|mknodat|fchownat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 273, 17, 16), //newfstatat|unlinkat|renameat|linkat|symlinkat|readlinkat|fchmodat|faccessat|pselect6|ppoll|unshare
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 283, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 280, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 279, 14, 13), //splice|tee|sync_file_range|vmsplice
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 282, 13, 12), //utimensat|epoll_pwait
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 284, 12, 11), //timerfd_create
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 314, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 306, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 302, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 300, 8, 7), //fallocate|timerfd_settime|timerfd_gettime|accept4|signalfd4|eventfd2|epoll_create1|dup3|pipe2|inotify_init1|preadv|pwritev|rt_tgsigqueueinfo|perf_event_open|recvmmsg
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 303, 7, 6), //prlimit64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 312, 6, 5), //syncfs|sendmmsg|setns|getcpu|process_vm_readv|process_vm_writev
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 325, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 322, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 320, 3, 2), //sched_setattr|sched_getattr|renameat2|seccomp|getrandom|memfd_create
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 323, 2, 1), //execveat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 329, 1, 0), //mlock2|copy_file_range|preadv2|pwritev2
BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW),
};

const size_t x86_64_app_filter_size = sizeof(x86_64_app_filter) / sizeof(struct sock_filter);
