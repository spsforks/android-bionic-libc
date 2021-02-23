// File autogenerated by genseccomp.py - edit at your peril!!

#include <errno.h>
#include <linux/filter.h>

#include "seccomp/seccomp_bpfs.h"
const sock_filter x86_app_zygote_filter[] = {
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 0, 0, 134),
    BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 240, 132, 0),  // futex
    BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 54, 131, 0),   // ioctl
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 172, 65, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 85, 33, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 45, 17, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 26, 9, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 19, 5, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 10, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 8, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 7, 124,
             123),  // restart_syscall|exit|fork|read|write|open|close
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 9, 123, 122),   // creat
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 13, 122, 121),  // unlink|execve|chdir
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 24, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 21, 120, 119),  // lseek|getpid
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 25, 119, 118),  // getuid
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 36, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 33, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 27, 116, 115),  // ptrace
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 34, 115, 114),  // access
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 41, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 40, 113, 112),  // sync|kill|rename|mkdir
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 44, 112, 111),  // dup|pipe|times
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 63, 7, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 57, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 55, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 46, 108, 107),  // brk
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 56, 107, 106),  // fcntl
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 60, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 58, 105, 104),  // setpgid
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 61, 104, 103),  // umask
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 75, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 66, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 65, 101, 100),  // dup2|getppid
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 68, 100, 99),   // setsid|sigaction
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 77, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 76, 98, 97),  // setrlimit
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 79, 97, 96),  // getrusage|gettimeofday
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 118, 15, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 102, 7, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 94, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 90, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 86, 92, 91),  // readlink
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 93, 91, 90),  // mmap|munmap|truncate
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 96, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 95, 89, 88),  // fchmod
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 98, 88, 87),  // getpriority|setpriority
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 114, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 104, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 103, 85, 84),  // socketcall
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 106, 84, 83),  // setitimer|getitimer
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 116, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 115, 82, 81),  // wait4
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 117, 81, 80),  // sysinfo
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 136, 7, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 125, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 122, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 121, 77, 76),  // fsync|sigreturn|clone
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 123, 76, 75),  // uname
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 131, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 126, 74, 73),  // mprotect
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 134, 73, 72),  // quotactl|getpgid|fchdir
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 150, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 140, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 137, 70, 69),  // personality
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 149, 69,
             68),  //_llseek|getdents|_newselect|flock|msync|readv|writev|getsid|fdatasync
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 168, 1, 0),
    BPF_JUMP(
        BPF_JMP | BPF_JGE | BPF_K, 164, 67,
        66),  // mlock|munlock|mlockall|munlockall|sched_setparam|sched_getparam|sched_setscheduler|sched_getscheduler|sched_yield|sched_get_priority_max|sched_get_priority_min|sched_rr_get_interval|nanosleep|mremap
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 169, 66, 65),  // poll
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 300, 33, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 245, 17, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 207, 9, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 199, 5, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 190, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 183, 1, 0),
    BPF_JUMP(
        BPF_JMP | BPF_JGE | BPF_K, 182, 59,
        58),  // prctl|rt_sigreturn|rt_sigaction|rt_sigprocmask|rt_sigpending|rt_sigtimedwait|rt_sigqueueinfo|rt_sigsuspend|pread64|pwrite64
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 188, 58, 57),  // getcwd|capget|capset|sigaltstack|sendfile
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 198, 57,
             56),  // vfork|ugetrlimit|mmap2|truncate64|ftruncate64|stat64|lstat64|fstat64
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 205, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 203, 55, 54),  // getuid32|getgid32|geteuid32|getegid32
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 206, 54, 53),  // getgroups32
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 224, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 218, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 212, 51,
             50),  // fchown32|setresuid32|getresuid32|setresgid32|getresgid32
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 222, 50, 49),  // mincore|madvise|getdents64|fcntl64
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 241, 1, 0),
    BPF_JUMP(
        BPF_JMP | BPF_JGE | BPF_K, 240, 48,
        47),  // gettid|readahead|setxattr|lsetxattr|fsetxattr|getxattr|lgetxattr|fgetxattr|listxattr|llistxattr|flistxattr|removexattr|lremovexattr|fremovexattr|tkill|sendfile64
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 244, 47,
             46),  // sched_setaffinity|sched_getaffinity|set_thread_area
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 272, 7, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 254, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 252, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 250, 43,
             42),  // io_setup|io_destroy|io_getevents|io_submit|io_cancel
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 253, 42, 41),  // exit_group
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 265, 1, 0),
    BPF_JUMP(
        BPF_JMP | BPF_JGE | BPF_K, 264, 40,
        39),  // epoll_create|epoll_ctl|epoll_wait|remap_file_pages|set_tid_address|timer_create|timer_settime|timer_gettime|timer_getoverrun|timer_delete
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 271, 39,
             38),  // clock_gettime|clock_getres|clock_nanosleep|statfs64|fstatfs64|tgkill
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 291, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 284, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 273, 36, 35),  // fadvise64_64
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 285, 35, 34),  // waitid
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 295, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 294, 33,
             32),  // inotify_init|inotify_add_watch|inotify_rm_watch
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 299, 32, 31),  // openat|mkdirat|mknodat|fchownat
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 358, 15, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 340, 7, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 318, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 313, 1, 0),
    BPF_JUMP(
        BPF_JMP | BPF_JGE | BPF_K, 311, 27,
        26),  // fstatat64|unlinkat|renameat|linkat|symlinkat|readlinkat|fchmodat|faccessat|pselect6|ppoll|unshare
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 317, 26, 25),  // splice|sync_file_range|tee|vmsplice
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 322, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 321, 24, 23),  // getcpu|epoll_pwait|utimensat
    BPF_JUMP(
        BPF_JMP | BPF_JGE | BPF_K, 337, 23,
        22),  // timerfd_create|eventfd|fallocate|timerfd_settime|timerfd_gettime|signalfd4|eventfd2|epoll_create1|dup3|pipe2|inotify_init1|preadv|pwritev|rt_tgsigqueueinfo|perf_event_open
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 346, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 344, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 341, 20, 19),  // prlimit64
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 345, 19, 18),  // syncfs
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 351, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 349, 17, 16),  // setns|process_vm_readv|process_vm_writev
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 357, 16,
             15),  // sched_setattr|sched_getattr|renameat2|seccomp|getrandom|memfd_create
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 417, 7, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 383, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 375, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 359, 12, 11),  // execveat
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 380, 11,
             10),  // membarrier|mlock2|copy_file_range|preadv2|pwritev2
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 403, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 384, 9, 8),  // statx
    BPF_JUMP(
        BPF_JMP | BPF_JGE | BPF_K, 415, 8,
        7),  // clock_gettime64|clock_settime64|clock_adjtime64|clock_getres_time64|clock_nanosleep_time64|timer_gettime64|timer_settime64|timerfd_gettime64|timerfd_settime64|utimensat_time64|pselect6_time64|ppoll_time64
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 434, 3, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 420, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 418, 5, 4),  // recvmmsg_time64
    BPF_JUMP(
        BPF_JMP | BPF_JGE | BPF_K, 424, 4,
        3),  // semtimedop_time64|rt_sigtimedwait_time64|futex_time64|sched_rr_get_interval_time64
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 440, 1, 0),
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 435, 2, 1),  // pidfd_open
    BPF_JUMP(BPF_JMP | BPF_JGE | BPF_K, 441, 1, 0),  // process_madvise
    BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),
};

const size_t x86_app_zygote_filter_size =
    sizeof(x86_app_zygote_filter) / sizeof(struct sock_filter);
