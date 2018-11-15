// Autogenerated file - edit at your peril!!

#include <linux/filter.h>
#include <errno.h>

#include "seccomp_bpfs.h"
const sock_filter x86_64_system_filter[] = {
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 0, 0, 84),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 179, 41, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 93, 21, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 38, 11, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 24, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 8, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 5, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4, 77, 76), //read|write|open|close
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 6, 76, 75), //fstat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 21, 75, 74), //lseek|mmap|mprotect|munmap|brk|rt_sigaction|rt_sigprocmask|rt_sigreturn|ioctl|pread64|pwrite64|readv|writev
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 35, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 32, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 29, 72, 71), //sched_yield|mremap|msync|mincore|madvise
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 33, 71, 70), //dup
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 37, 70, 69), //nanosleep|getitimer
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 72, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 58, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 44, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 43, 66, 65), //setitimer|getpid|sendfile|socket|connect
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 57, 65, 64), //sendto|recvfrom|sendmsg|recvmsg|shutdown|bind|listen|getsockname|getpeername|socketpair|setsockopt|getsockopt|clone
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 64, 64, 63), //vfork|execve|exit|wait4|kill|uname
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 91, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 82, 62, 61), //fcntl|flock|fsync|fdatasync|truncate|ftruncate|getdents|getcwd|chdir|fchdir
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 92, 61, 60), //fchmod
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 140, 9, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 135, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 112, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 95, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 94, 56, 55), //fchown
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 111, 55, 54), //umask|gettimeofday|getrlimit|getrusage|sysinfo|times|ptrace|getuid|syslog|getgid|setuid|setgid|geteuid|getegid|setpgid|getppid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 132, 54, 53), //setsid|setreuid|setregid|getgroups|setgroups|setresuid|getresuid|setresgid|getresgid|getpgid|setfsuid|setfsgid|getsid|capget|capset|rt_sigpending|rt_sigtimedwait|rt_sigqueueinfo|rt_sigsuspend|sigaltstack
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 137, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 136, 52, 51), //personality
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 139, 51, 50), //statfs|fstatfs
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 169, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 157, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 155, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 153, 47, 46), //getpriority|setpriority|sched_setparam|sched_getparam|sched_setscheduler|sched_getscheduler|sched_get_priority_max|sched_get_priority_min|sched_rr_get_interval|mlock|munlock|mlockall|munlockall
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 156, 46, 45), //pivot_root
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 167, 45, 44), //prctl|arch_prctl|adjtimex|setrlimit|chroot|sync|acct|settimeofday|mount|umount2
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 175, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 172, 43, 42), //reboot|sethostname|setdomainname
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 177, 42, 41), //init_module|delete_module
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 262, 21, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 233, 11, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 206, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 202, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 186, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 180, 36, 35), //quotactl
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 201, 35, 34), //gettid|readahead|setxattr|lsetxattr|fsetxattr|getxattr|lgetxattr|fgetxattr|listxattr|llistxattr|flistxattr|removexattr|lremovexattr|fremovexattr|tkill
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 205, 34, 33), //futex|sched_setaffinity|sched_getaffinity
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 221, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 217, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 211, 31, 30), //io_setup|io_destroy|io_getevents|io_submit|io_cancel
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 220, 30, 29), //getdents64|set_tid_address|restart_syscall
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 232, 29, 28), //fadvise64|timer_create|timer_settime|timer_gettime|timer_getoverrun|timer_delete|clock_settime|clock_gettime|clock_getres|clock_nanosleep|exit_group
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 254, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 251, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 247, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 235, 25, 24), //epoll_ctl|tgkill
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 248, 24, 23), //waitid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 253, 23, 22), //ioprio_set|ioprio_get
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 257, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 256, 21, 20), //inotify_add_watch|inotify_rm_watch
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 261, 20, 19), //openat|mkdirat|mknodat|fchownat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 302, 9, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 283, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 280, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 275, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 273, 15, 14), //newfstatat|unlinkat|renameat|linkat|symlinkat|readlinkat|fchmodat|faccessat|pselect6|ppoll|unshare
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 279, 14, 13), //splice|tee|sync_file_range|vmsplice
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 282, 13, 12), //utimensat|epoll_pwait
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 285, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 284, 11, 10), //timerfd_create
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 300, 10, 9), //fallocate|timerfd_settime|timerfd_gettime|accept4|signalfd4|eventfd2|epoll_create1|dup3|pipe2|inotify_init1|preadv|pwritev|rt_tgsigqueueinfo|perf_event_open|recvmmsg
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 321, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 314, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 305, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 303, 6, 5), //prlimit64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 312, 5, 4), //clock_adjtime|syncfs|sendmmsg|setns|getcpu|process_vm_readv|process_vm_writev
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 320, 4, 3), //sched_setattr|sched_getattr|renameat2|seccomp|getrandom|memfd_create
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 325, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 323, 2, 1), //bpf|execveat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 329, 1, 0), //mlock2|copy_file_range|preadv2|pwritev2
BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW),
};

const size_t x86_64_system_filter_size = sizeof(x86_64_system_filter) / sizeof(struct sock_filter);
