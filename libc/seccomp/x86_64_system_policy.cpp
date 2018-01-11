// Autogenerated file - edit at your peril!!

#include <linux/filter.h>
#include <errno.h>

#include "seccomp_bpfs.h"
const sock_filter x86_64_system_filter[] = {
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 0, 0, 88),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 175, 43, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 79, 21, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 35, 11, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 8, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 5, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 3, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 2, 81, 80), //read|write
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4, 80, 79), //close
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 6, 79, 78), //fstat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 32, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 24, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 21, 76, 75), //lseek|mmap|mprotect|munmap|brk|rt_sigaction|rt_sigprocmask|rt_sigreturn|ioctl|pread64|pwrite64|readv|writev
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 29, 75, 74), //sched_yield|mremap|msync|mincore|madvise
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 33, 74, 73), //dup
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 58, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 44, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 38, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 37, 70, 69), //nanosleep|getitimer
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 43, 69, 68), //setitimer|getpid|sendfile|socket|connect
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 57, 68, 67), //sendto|recvfrom|sendmsg|recvmsg|shutdown|bind|listen|getsockname|getpeername|socketpair|setsockopt|getsockopt|clone
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 72, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 64, 66, 65), //vfork|execve|exit|wait4|kill|uname
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 78, 65, 64), //fcntl|flock|fsync|fdatasync|truncate|ftruncate
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 137, 11, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 95, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 93, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 91, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 82, 60, 59), //getcwd|chdir|fchdir
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 92, 59, 58), //fchmod
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 94, 58, 57), //fchown
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 135, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 112, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 111, 55, 54), //umask|gettimeofday|getrlimit|getrusage|sysinfo|times|ptrace|getuid|syslog|getgid|setuid|setgid|geteuid|getegid|setpgid|getppid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 132, 54, 53), //setsid|setreuid|setregid|getgroups|setgroups|setresuid|getresuid|setresgid|getresgid|getpgid|setfsuid|setfsgid|getsid|capget|capset|rt_sigpending|rt_sigtimedwait|rt_sigqueueinfo|rt_sigsuspend|sigaltstack
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 136, 53, 52), //personality
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 157, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 155, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 140, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 139, 49, 48), //statfs|fstatfs
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 153, 48, 47), //getpriority|setpriority|sched_setparam|sched_getparam|sched_setscheduler|sched_getscheduler|sched_get_priority_max|sched_get_priority_min|sched_rr_get_interval|mlock|munlock|mlockall|munlockall
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 156, 47, 46), //pivot_root
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 169, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 167, 45, 44), //prctl|arch_prctl|adjtimex|setrlimit|chroot|sync|acct|settimeofday|mount|umount2
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 172, 44, 43), //reboot|sethostname|setdomainname
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 257, 21, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 221, 11, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 202, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 186, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 179, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 177, 38, 37), //init_module|delete_module
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 180, 37, 36), //quotactl
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 201, 36, 35), //gettid|readahead|setxattr|lsetxattr|fsetxattr|getxattr|lgetxattr|fgetxattr|listxattr|llistxattr|flistxattr|removexattr|lremovexattr|fremovexattr|tkill
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 217, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 206, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 205, 33, 32), //futex|sched_setaffinity|sched_getaffinity
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 211, 32, 31), //io_setup|io_destroy|io_getevents|io_submit|io_cancel
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 220, 31, 30), //getdents64|set_tid_address|restart_syscall
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 251, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 247, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 233, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 232, 27, 26), //fadvise64|timer_create|timer_settime|timer_gettime|timer_getoverrun|timer_delete|clock_settime|clock_gettime|clock_getres|clock_nanosleep|exit_group
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 235, 26, 25), //epoll_ctl|tgkill
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 248, 25, 24), //waitid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 254, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 253, 23, 22), //ioprio_set|ioprio_get
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 256, 22, 21), //inotify_add_watch|inotify_rm_watch
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 302, 11, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 280, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 275, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 262, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 261, 17, 16), //openat|mkdirat|mknodat|fchownat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 273, 16, 15), //newfstatat|unlinkat|renameat|linkat|symlinkat|readlinkat|fchmodat|faccessat|pselect6|ppoll|unshare
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 279, 15, 14), //splice|tee|sync_file_range|vmsplice
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 285, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 283, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 282, 12, 11), //utimensat|epoll_pwait
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 284, 11, 10), //timerfd_create
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 300, 10, 9), //fallocate|timerfd_settime|timerfd_gettime|accept4|signalfd4|eventfd2|epoll_create1|dup3|pipe2|inotify_init1|preadv|pwritev|rt_tgsigqueueinfo|perf_event_open|recvmmsg
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 322, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 314, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 305, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 303, 6, 5), //prlimit64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 312, 5, 4), //clock_adjtime|syncfs|sendmmsg|setns|getcpu|process_vm_readv|process_vm_writev
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 320, 4, 3), //sched_setattr|sched_getattr|renameat2|seccomp|getrandom|memfd_create
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 325, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 323, 2, 1), //execveat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 329, 1, 0), //mlock2|copy_file_range|preadv2|pwritev2
BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW),
};

const size_t x86_64_system_filter_size = sizeof(x86_64_system_filter) / sizeof(struct sock_filter);
