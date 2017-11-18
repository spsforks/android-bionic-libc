// Autogenerated file - edit at your peril!!

#include <linux/filter.h>
#include <errno.h>

#include "seccomp_bpfs.h"
const sock_filter arm64_filter[] = {
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 0, 0, 30),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 220, 15, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 101, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 43, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 19, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 18, 25, 24), //io_setup|io_destroy|io_submit|io_cancel|io_getevents|setxattr|lsetxattr|fsetxattr|getxattr|lgetxattr|fgetxattr|listxattr|llistxattr|flistxattr|removexattr|lremovexattr|fremovexattr|getcwd
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 42, 24, 23), //eventfd2|epoll_create1|epoll_ctl|epoll_pwait|dup|dup3|fcntl|inotify_init1|inotify_add_watch|inotify_rm_watch|ioctl|ioprio_set|ioprio_get|flock|mknodat|mkdirat|unlinkat|symlinkat|linkat|renameat|umount2|mount|pivot_root
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 59, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 58, 22, 21), //statfs|fstatfs|truncate|ftruncate|fallocate|faccessat|chdir|fchdir|chroot|fchmod|fchmodat|fchownat|fchown|openat|close
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 99, 21, 20), //pipe2|quotactl|getdents64|lseek|read|write|readv|writev|pread64|pwrite64|preadv|pwritev|sendfile|pselect6|ppoll|signalfd4|vmsplice|splice|tee|readlinkat|newfstatat|fstat|sync|fsync|fdatasync|sync_file_range|timerfd_create|timerfd_settime|timerfd_gettime|utimensat|acct|capget|capset|personality|exit|exit_group|waitid|set_tid_address|unshare|futex
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 198, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 105, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 104, 18, 17), //nanosleep|getitimer|setitimer
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 180, 17, 16), //init_module|delete_module|timer_create|timer_gettime|timer_getoverrun|timer_settime|timer_delete|clock_settime|clock_gettime|clock_getres|clock_nanosleep|syslog|ptrace|sched_setparam|sched_setscheduler|sched_getscheduler|sched_getparam|sched_setaffinity|sched_getaffinity|sched_yield|sched_get_priority_max|sched_get_priority_min|sched_rr_get_interval|restart_syscall|kill|tkill|tgkill|sigaltstack|rt_sigsuspend|rt_sigaction|rt_sigprocmask|rt_sigpending|rt_sigtimedwait|rt_sigqueueinfo|rt_sigreturn|setpriority|getpriority|reboot|setregid|setgid|setreuid|setuid|setresuid|getresuid|setresgid|getresgid|setfsuid|setfsgid|times|setpgid|getpgid|getsid|setsid|getgroups|setgroups|uname|sethostname|setdomainname|getrlimit|setrlimit|getrusage|umask|prctl|getcpu|gettimeofday|settimeofday|adjtimex|getpid|getppid|getuid|geteuid|getgid|getegid|gettid|sysinfo
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 203, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 202, 15, 14), //socket|socketpair|bind|listen
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 217, 14, 13), //connect|getsockname|getpeername|sendto|recvfrom|setsockopt|getsockopt|shutdown|sendmsg|recvmsg|readahead|brk|munmap|mremap
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 266, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 240, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 226, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 224, 10, 9), //clone|execve|mmap|fadvise64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 234, 9, 8), //mprotect|msync|mlock|munlock|mlockall|munlockall|mincore|madvise
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 260, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 244, 7, 6), //rt_tgsigqueueinfo|perf_event_open|accept4|recvmmsg
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 262, 6, 5), //wait4|prlimit64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 284, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 274, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 272, 3, 2), //clock_adjtime|syncfs|setns|sendmmsg|process_vm_readv|process_vm_writev
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 282, 2, 1), //sched_setattr|sched_getattr|renameat2|seccomp|getrandom|memfd_create|bpf|execveat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 288, 1, 0), //mlock2|copy_file_range|preadv2|pwritev2
BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW),
};

const size_t arm64_filter_size = sizeof(arm64_filter) / sizeof(struct sock_filter);
