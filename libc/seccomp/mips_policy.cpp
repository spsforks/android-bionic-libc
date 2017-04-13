// Autogenerated file - edit at your peril!!

#include <linux/filter.h>
#include <errno.h>

#include "seccomp_bpfs.h"
const sock_filter mips_filter[] = {
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4001, 0, 110),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4131, 55, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4064, 27, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4036, 13, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4023, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4010, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4008, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4007, 103, 102), //exit|fork|read|write|open|close
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4009, 102, 101), //creat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4019, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4013, 100, 99), //unlink|execve|chdir
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4022, 99, 98), //lseek|getpid|mount
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4033, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4026, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4025, 96, 95), //setuid|getuid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4027, 95, 94), //ptrace
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4034, 94, 93), //access
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4054, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4045, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4041, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4039, 90, 89), //sync|kill|rename
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4044, 89, 88), //dup|pipe|times
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4049, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4048, 87, 86), //brk|setgid|getgid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4053, 86, 85), //geteuid|getegid|acct|umount2
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4060, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4057, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4056, 83, 82), //ioctl|fcntl
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4058, 82, 81), //setpgid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4062, 81, 80), //umask|chroot
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4094, 13, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4085, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4070, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4066, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4065, 76, 75), //getppid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4068, 75, 74), //setsid|sigaction
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4074, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4072, 73, 72), //setreuid|setregid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4082, 72, 71), //sethostname|setrlimit|getrlimit|getrusage|gettimeofday|settimeofday|getgroups|setgroups
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4090, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4088, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4086, 69, 68), //readlink
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4089, 68, 67), //reboot
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4093, 67, 66), //mmap|munmap|truncate
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4118, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4114, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4103, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4098, 63, 62), //fchmod|fchown|getpriority|setpriority
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4106, 62, 61), //syslog|setitimer|getitimer
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4116, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4115, 60, 59), //wait4
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4117, 59, 58), //sysinfo
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4128, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4124, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4123, 56, 55), //fsync|sigreturn|clone|setdomainname|uname
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4126, 55, 54), //adjtimex|mprotect
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4130, 54, 53), //init_module|delete_module
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4222, 27, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4176, 13, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4151, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4138, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4136, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4134, 48, 47), //quotactl|getpgid|fchdir
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4137, 47, 46), //personality
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4143, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4142, 45, 44), //setfsuid|setfsgid|_llseek|getdents
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4148, 44, 43), //flock|msync|readv|writev|cacheflush
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4169, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4154, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4153, 41, 40), //getsid|fdatasync
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4168, 40, 39), //mlock|munlock|mlockall|munlockall|sched_setparam|sched_getparam|sched_setscheduler|sched_getscheduler|sched_yield|sched_get_priority_max|sched_get_priority_min|sched_rr_get_interval|nanosleep|mremap
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4175, 39, 38), //bind|connect|getpeername|getsockname|getsockopt|listen
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4203, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4188, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4179, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4178, 35, 34), //recvfrom|recvmsg
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4187, 34, 33), //sendmsg|sendto|setsockopt|shutdown|socket|socketpair|setresuid|getresuid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4190, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4189, 32, 31), //poll
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4202, 31, 30), //setresgid|getresgid|prctl|rt_sigreturn|rt_sigaction|rt_sigprocmask|rt_sigpending|rt_sigtimedwait|rt_sigqueueinfo|rt_sigsuspend|pread64|pwrite64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4217, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4210, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4208, 28, 27), //getcwd|capget|capset|sigaltstack|sendfile
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4216, 27, 26), //mmap2|truncate64|ftruncate64|stat64|lstat64|fstat64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4221, 26, 25), //mincore|madvise|getdents64|fcntl64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4312, 13, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4283, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4248, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4246, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4241, 21, 20), //gettid|readahead|setxattr|lsetxattr|fsetxattr|getxattr|lgetxattr|fgetxattr|listxattr|llistxattr|flistxattr|removexattr|lremovexattr|fremovexattr|tkill|sendfile64|futex|sched_setaffinity|sched_getaffinity
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4247, 20, 19), //exit_group
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4278, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4267, 18, 17), //epoll_create|epoll_ctl|epoll_wait|remap_file_pages|set_tid_address|restart_syscall|fadvise64|statfs64|fstatfs64|timer_create|timer_settime|timer_gettime|timer_getoverrun|timer_delete|clock_settime|clock_gettime|clock_getres|clock_nanosleep|tgkill
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4279, 17, 16), //waitid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4293, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4288, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4287, 14, 13), //set_thread_area|inotify_init|inotify_add_watch|inotify_rm_watch
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4292, 13, 12), //openat|mkdirat|mknodat|fchownat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4308, 12, 11), //fstatat64|unlinkat|renameat|linkat|symlinkat|readlinkat|fchmodat|faccessat|pselect6|ppoll|unshare|splice|sync_file_range|tee|vmsplice
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4338, 5, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4319, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4316, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4314, 8, 7), //getcpu|epoll_pwait
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4317, 7, 6), //utimensat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4336, 6, 5), //eventfd|fallocate|timerfd_create|timerfd_gettime|timerfd_settime|signalfd4|eventfd2|epoll_create1|dup3|pipe2|inotify_init1|preadv|pwritev|rt_tgsigqueueinfo|perf_event_open|accept4|recvmmsg
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4352, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4341, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4339, 3, 2), //prlimit64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4347, 2, 1), //clock_adjtime|syncfs|sendmmsg|setns|process_vm_readv|process_vm_writev
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4355, 1, 0), //seccomp|getrandom|memfd_create
BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW),
};

const size_t mips_filter_size = sizeof(mips_filter) / sizeof(struct sock_filter);
