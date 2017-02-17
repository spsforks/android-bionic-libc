// Autogenerated file - edit at your peril!!

#include <linux/filter.h>
#include <errno.h>

#include "seccomp_bpfs.h"
const sock_filter mips_filter[] = {
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4001, 0, 113),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4138, 57, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4066, 29, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4041, 15, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4023, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4010, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4008, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4007, 107, 106), //exit|fork|read|write|open|close
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4009, 106, 105), //creat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4019, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4013, 104, 103), //unlink|execve|chdir
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4022, 103, 102), //lseek|getpid|mount
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4033, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4026, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4025, 100, 99), //setuid|getuid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4027, 99, 98), //ptrace
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4036, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4034, 97, 96), //access
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4038, 96, 95), //sync|kill
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4057, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4049, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4045, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4044, 92, 91), //dup|pipe|times
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4048, 91, 90), //brk|setgid|getgid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4054, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4053, 89, 88), //geteuid|getegid|acct|umount2
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4056, 88, 87), //ioctl|fcntl
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4064, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4060, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4058, 85, 84), //setpgid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4062, 84, 83), //umask|chroot
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4065, 83, 82), //getppid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4103, 13, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4087, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4074, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4070, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4068, 78, 77), //setsid|sigaction
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4072, 77, 76), //setreuid|setregid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4085, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4082, 75, 74), //sethostname|setrlimit|getrlimit|getrusage|gettimeofday|settimeofday|getgroups|setgroups
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4086, 74, 73), //readlink
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4094, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4091, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4089, 71, 70), //swapon|reboot
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4093, 70, 69), //munmap|truncate
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4098, 69, 68), //fchmod|fchown|getpriority|setpriority
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4128, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4118, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4114, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4106, 65, 64), //syslog|setitimer|getitimer
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4117, 64, 63), //wait4|swapoff|sysinfo
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4124, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4123, 62, 61), //fsync|sigreturn|clone|setdomainname|uname
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4126, 61, 60), //adjtimex|mprotect
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4136, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4131, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4130, 58, 57), //init_module|delete_module
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4134, 57, 56), //quotactl|getpgid|fchdir
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4137, 56, 55), //personality
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4248, 27, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4188, 13, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4169, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4151, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4143, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4142, 50, 49), //setfsuid|setfsgid|_llseek|getdents
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4148, 49, 48), //flock|msync|readv|writev|cacheflush
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4154, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4153, 47, 46), //getsid|fdatasync
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4168, 46, 45), //mlock|munlock|mlockall|munlockall|sched_setparam|sched_getparam|sched_setscheduler|sched_getscheduler|sched_yield|sched_get_priority_max|sched_get_priority_min|sched_rr_get_interval|nanosleep|mremap
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4179, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4176, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4175, 43, 42), //bind|connect|getpeername|getsockname|getsockopt|listen
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4178, 42, 41), //recvfrom|recvmsg
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4187, 41, 40), //sendmsg|sendto|setsockopt|shutdown|socket|socketpair|setresuid|getresuid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4217, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4203, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4190, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4189, 37, 36), //poll
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4202, 36, 35), //setresgid|getresgid|prctl|rt_sigreturn|rt_sigaction|rt_sigprocmask|rt_sigpending|rt_sigtimedwait|rt_sigqueueinfo|rt_sigsuspend|pread64|pwrite64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4210, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4208, 34, 33), //getcwd|capget|capset|sigaltstack|sendfile
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4216, 33, 32), //mmap2|truncate64|ftruncate64|stat64|lstat64|fstat64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4246, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4222, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4221, 30, 29), //mincore|madvise|getdents64|fcntl64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4241, 29, 28), //gettid|readahead|setxattr|lsetxattr|fsetxattr|getxattr|lgetxattr|fgetxattr|listxattr|llistxattr|flistxattr|removexattr|lremovexattr|fremovexattr|tkill|sendfile64|futex|sched_setaffinity|sched_getaffinity
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4247, 28, 27), //exit_group
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4312, 13, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4285, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4278, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4252, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4251, 23, 22), //epoll_create|epoll_ctl|epoll_wait
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4267, 22, 21), //set_tid_address|restart_syscall|fadvise64|statfs64|fstatfs64|timer_create|timer_settime|timer_gettime|timer_getoverrun|timer_delete|clock_settime|clock_gettime|clock_getres|clock_nanosleep|tgkill
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4283, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4279, 20, 19), //waitid
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4284, 19, 18), //set_thread_area
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4293, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4288, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4287, 16, 15), //inotify_add_watch|inotify_rm_watch
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4292, 15, 14), //openat|mkdirat|mknodat|fchownat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4308, 14, 13), //fstatat64|unlinkat|renameat|linkat|symlinkat|readlinkat|fchmodat|faccessat|pselect6|ppoll|unshare|splice|sync_file_range|tee|vmsplice
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4341, 7, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4319, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4316, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4314, 10, 9), //getcpu|epoll_pwait
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4317, 9, 8), //utimensat
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4338, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4336, 7, 6), //eventfd|fallocate|timerfd_create|timerfd_gettime|timerfd_settime|signalfd4|eventfd2|epoll_create1|dup3|pipe2|inotify_init1|preadv|pwritev|rt_tgsigqueueinfo|perf_event_open|accept4|recvmmsg
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4339, 6, 5), //prlimit64
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4352, 3, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4343, 1, 0),
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4342, 3, 2), //clock_adjtime
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4347, 2, 1), //sendmmsg|setns|process_vm_readv|process_vm_writev
BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4354, 1, 0), //seccomp|getrandom
BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW),
};

const size_t mips_filter_size = sizeof(mips_filter) / sizeof(struct sock_filter);
