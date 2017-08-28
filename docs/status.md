Android bionic status
=====================

libc
----

Current libc symbols: https://android.googlesource.com/platform/bionic/+/master/libc/libc.map.txt

New libc functions in P:
  * `__freading`/`__fwriting` (completing <stdio_ext.h>)
  * `getlogin_r`
  * `iconv`/`iconv_close`/`iconv_open` (adding <iconv.h>)
  * `syncfs`

New libc functions in O:
  * `sendto` FORTIFY support
  * `__system_property_read_callback`/`__system_property_wait`
  * legacy `bsd_signal`
  * `catclose`/`catgets`/`catopen` (adding <nl_types.h>)
  * `ctermid`
  * all 6 <grp.h>/<pwd.h> (get|set|end)(gr|pw)ent functions
  * `futimes`/`futimesat`/`lutimes`
  * `getdomainname`/`setdomainname`
  * `getsubopt`
  * `hasmntopt`
  * `mallopt`
  * `mblen`
  * 4 <sys/msg.h> `msg*` functions
  * <langinfo.h> `nl_langinfo`/`nl_langinfo_l`
  * `pthread_getname_np`
  * 2 new Linux system calls `quotactl` and `sync_file_range`
  * 4 <sys/sem.h> `sem*` functions
  * 4 <sys/shm.h> `shm*` functions
  * 5 legacy <signal.h> functions: `sighold`/`sigignore`/`sigpause`/`sigrelse`/`sigset`
  * `strtod_l`/`strtof_l`/`strtol_l`/`strtoul_l`
  * <wctype.h> `towctrans`/`towctrans_l`/`wctrans`/`wctrans_l`

New libc functions in N:
  * more FORTIFY support functions (`fread`/`fwrite`/`getcwd`/`pwrite`/`write`)
  * all remaining `_FILE_OFFSET_BITS=64` functions, completing `_FILE_OFFSET_BITS=64` support in bionic (8)
  * all 7 `pthread_barrier*` functions
  * all 5 `pthread_spin*` functions
  * `lockf`/`preadv`/`pwritev`/`scandirat` and `off64_t` variants
  * `adjtimex`/`clock_adjtime`
  * `getifaddrs`/`freeifaddrs`/`if_freenameindex`/`if_nameindex`
  * `getgrgid_r`/`getgrnam_r`
  * GNU extensions `fileno_unlocked`/`strchrnul`
  * 32-bit `prlimit`

libc function count over time:
  G 801
  H 823
  I 824
  J 844
  K 899
  L 1118
  M 1183
  N 1228
  O 1280

Run `./libc/tools/check-symbols-glibc.py` in bionic/ for the current
list of POSIX functions implemented by glibc but not by bionic. Currently
(2017-09):
```
aio_cancel
aio_error
aio_fsync
aio_read
aio_return
aio_suspend
aio_write
endhostent
endnetent
endprotoent
endutxent
fexecve
fmtmsg
getdate
getdate_err
getnetent
getprotoent
glob
globfree
hcreate
hdestroy
hsearch
lio_listio
posix_spawn
posix_spawn_file_actions_addclose
posix_spawn_file_actions_adddup2
posix_spawn_file_actions_addopen
posix_spawn_file_actions_destroy
posix_spawn_file_actions_init
posix_spawnattr_destroy
posix_spawnattr_getflags
posix_spawnattr_getpgroup
posix_spawnattr_getschedparam
posix_spawnattr_getschedpolicy
posix_spawnattr_getsigdefault
posix_spawnattr_getsigmask
posix_spawnattr_init
posix_spawnattr_setflags
posix_spawnattr_setpgroup
posix_spawnattr_setschedparam
posix_spawnattr_setschedpolicy
posix_spawnattr_setsigdefault
posix_spawnattr_setsigmask
posix_spawnp
pthread_attr_getinheritsched
pthread_attr_setinheritsched
pthread_cancel
pthread_getconcurrency
pthread_mutex_consistent
pthread_mutex_getprioceiling
pthread_mutex_setprioceiling
pthread_mutexattr_getprioceiling
pthread_mutexattr_getprotocol
pthread_mutexattr_getrobust
pthread_mutexattr_setprioceiling
pthread_mutexattr_setprotocol
pthread_mutexattr_setrobust
pthread_setcancelstate
pthread_setcanceltype
pthread_setconcurrency
pthread_setschedprio
pthread_testcancel
sethostent
setnetent
setprotoent
sockatmark
swab
wordexp
wordfree
```

libm
----

Current libm symbols: https://android.googlesource.com/platform/bionic/+/master/libm/libm.map.txt

0 remaining missing POSIX libm functions.

19 new libm functions in O: complex trig/exp/log functions.

libm function count over time:
  G 154
  H 158
  J 164
  L 220
  M 265
  N 265
  O 284


Availability
------------

| Function | API level |
| -------- | --------- |
| _Exit | 21 |
| _exit | all |
| _longjmp | all |
| _setjmp | all |
| _tolower | 21 |
| _toupper | 21 |
| abort | all |
| abs | 19 |
| accept | all |
| access | all |
| acos | 18 |
| acosf | 18 |
| acosh | 18 |
| acoshf | 18 |
| acoshl | 21 |
| acosl | 21 |
| aio_cancel | N/A |
| aio_error | N/A |
| aio_fsync | N/A |
| aio_read | N/A |
| aio_return | N/A |
| aio_suspend | N/A |
| aio_write | N/A |
| alarm | all |
| alphasort | all |
| asctime | all |
| asctime_r | all |
| asin | 18 |
| asinf | 18 |
| asinh | 18 |
| asinhf | 18 |
| asinhl | 21 |
| asinl | 21 |
| atan | 18 |
| atan2 | 18 |
| atan2f | 18 |
| atan2l | 21 |
| atanf | 18 |
| atanh | 18 |
| atanhf | 18 |
| atanhl | 21 |
| atanl | 21 |
| atexit | all |
| atof | 21 |
| atoi | all |
| atol | all |
| atoll | all |
| basename | all |
| bind | all |
| bsearch | all |
| btowc | all |
| cabs | 23 |
| cabsf | 23 |
| cabsl | 21 |
| cacos | 23 |
| cacosf | 23 |
| cacosh | 23 |
| cacoshf | 23 |
| cacoshl | 26 |
| cacosl | 26 |
| calloc | all |
| carg | 23 |
| cargf | 23 |
| cargl | 23 |
| casin | 23 |
| casinf | 23 |
| casinh | 23 |
| casinhf | 23 |
| casinhl | 26 |
| casinl | 26 |
| catan | 23 |
| catanf | 23 |
| catanh | 23 |
| catanhf | 23 |
| catanhl | 26 |
| catanl | 26 |
| catclose | 26 |
| catgets | 26 |
| catopen | 26 |
| cbrt | 18 |
| cbrtf | 18 |
| cbrtl | 21 |
| ccos | 23 |
| ccosf | 23 |
| ccosh | 23 |
| ccoshf | 23 |
| ccoshl | 26 |
| ccosl | 26 |
| ceil | 18 |
| ceilf | 18 |
| ceill | 18 |
| cexp | 23 |
| cexpf | 23 |
| cexpl | 26 |
| cfgetispeed | 21 |
| cfgetospeed | 21 |
| cfsetispeed | 21 |
| cfsetospeed | 21 |
| chdir | all |
| chmod | all |
| chown | all |
| cimag | 23 |
| cimagf | 23 |
| cimagl | 23 |
| clearerr | all |
| clock | all |
| clock_getcpuclockid | 23 |
| clock_getres | all |
| clock_gettime | all |
| clock_nanosleep | all |
| clock_settime | all |
| clog | 26 |
| clogf | 26 |
| clogl | 26 |
| close | all |
| closedir | all |
| closelog | all |
| confstr | N/A |
| conj | 23 |
| conjf | 23 |
| conjl | 23 |
| connect | all |
| copysign | 18 |
| copysignf | 18 |
| copysignl | 18 |
| cos | 18 |
| cosf | 18 |
| cosh | 18 |
| coshf | 18 |
| coshl | 21 |
| cosl | 21 |
| cpow | 26 |
| cpowf | 26 |
| cpowl | 26 |
| cproj | 23 |
| cprojf | 23 |
| cprojl | 21 |
| creal | 23 |
| crealf | 23 |
| creall | 23 |
| creat | all |
| crypt | N/A |
| csin | 23 |
| csinf | 23 |
| csinh | 23 |
| csinhf | 23 |
| csinhl | 26 |
| csinl | 26 |
| csqrt | 23 |
| csqrtf | 23 |
| csqrtl | 21 |
| ctan | 23 |
| ctanf | 23 |
| ctanh | 23 |
| ctanhf | 23 |
| ctanhl | 26 |
| ctanl | 26 |
| ctermid | 26 |
| ctime | all |
| ctime_r | all |
| daylight | all |
| difftime | all |
| dirfd | all |
| dirname | all |
| div | all |
| dlclose | 21 |
| dlerror | 21 |
| dlopen | 21 |
| dlsym | 21 |
| dprintf | 21 |
| drand48 | all |
| dup | all |
| dup2 | all |
| duplocale | 21 |
| encrypt | N/A |
| endgrent | N/A |
| endhostent | N/A |
| endnetent | N/A |
| endprotoent | N/A |
| endpwent | N/A |
| endservent | all |
| endutxent | N/A |
| environ | all |
| erand48 | all |
| erf | 18 |
| erfc | 18 |
| erfcf | 18 |
| erfcl | 21 |
| erff | 18 |
| erfl | 21 |
| execl | all |
| execle | all |
| execlp | all |
| execv | all |
| execve | all |
| execvp | all |
| exit | all |
| exp | 18 |
| exp2 | 18 |
| exp2f | 18 |
| exp2l | 21 |
| expf | 18 |
| expl | 21 |
| expm1 | 18 |
| expm1f | 18 |
| expm1l | 21 |
| fabs | 18 |
| fabsf | 18 |
| fabsl | 18 |
| faccessat | 16 |
| fattach | N/A |
| fchdir | all |
| fchmod | all |
| fchmodat | all |
| fchown | all |
| fchownat | all |
| fclose | all |
| fcntl | all |
| fdatasync | all |
| fdetach | N/A |
| fdim | 18 |
| fdimf | 18 |
| fdiml | 18 |
| fdopen | all |
| fdopendir | all |
| feclearexcept | 21 |
| fegetenv | 21 |
| fegetexceptflag | 21 |
| fegetround | 21 |
| feholdexcept | 21 |
| feof | all |
| feraiseexcept | 21 |
| ferror | all |
| fesetenv | 21 |
| fesetexceptflag | 21 |
| fesetround | 21 |
| fetestexcept | 21 |
| feupdateenv | 21 |
| fexecve | N/A |
| fflush | all |
| ffs | all |
| fgetc | all |
| fgetpos | all |
| fgets | all |
| fgetwc | all |
| fgetws | all |
| fileno | all |
| flockfile | all |
| floor | 18 |
| floorf | 18 |
| floorl | 18 |
| fma | 18 |
| fmaf | 18 |
| fmal | 21 |
| fmax | 18 |
| fmaxf | 18 |
| fmaxl | 18 |
| fmemopen | 23 |
| fmin | 18 |
| fminf | 18 |
| fminl | 18 |
| fmod | 18 |
| fmodf | 18 |
| fmodl | 21 |
| fmtmsg | N/A |
| fnmatch | all |
| fopen | all |
| fork | all |
| fpathconf | all |
| fpclassify | all (compiler builtin) |
| fprintf | all |
| fputc | all |
| fputs | all |
| fputwc | all |
| fputws | all |
| fread | all |
| free | all |
| freeaddrinfo | all |
| freelocale | 21 |
| freopen | all |
| frexp | 18 |
| frexpf | 18 |
| frexpl | 21 |
| fscanf | all |
| fseek | all |
| fseeko | all |
| fsetpos | all |
| fstat | all |
| fstatat | all |
| fstatvfs | 19 |
| fsync | all |
| ftell | all |
| ftello | all |
| ftok | all |
| ftruncate | all |
| ftrylockfile | all |
| ftw | 17 |
| funlockfile | all |
| futimens | 19 |
| fwide | all |
| fwprintf | all |
| fwrite | all |
| fwscanf | all |
| gai_strerror | all |
| getaddrinfo | all |
| getc | all |
| getc_unlocked | all |
| getchar | all |
| getchar_unlocked | all |
| getcwd | all |
| getdate | N/A |
| getdate_err | N/A |
| getdelim | 18 |
| getegid | all |
| getenv | all |
| geteuid | all |
| getgid | all |
| getgrent | N/A |
| getgrgid | all |
| getgrgid_r | 24 |
| getgrnam | all |
| getgrnam_r | 24 |
| getgroups | all |
| gethostent | all |
| gethostid | N/A |
| gethostname | all |
| getitimer | all |
| getline | 18 |
| getlogin | all |
| getlogin_r | N/A |
| getmsg | N/A |
| getnameinfo | all |
| getnetbyaddr | all |
| getnetbyname | all |
| getnetent | N/A |
| getopt | all |
| getpeername | all |
| getpgid | all |
| getpgrp | all |
| getpid | all |
| getpmsg | N/A |
| getppid | all |
| getpriority | all |
| getprotobyname | all |
| getprotobynumber | all |
| getprotoent | N/A |
| getpwent | N/A |
| getpwnam | all |
| getpwnam_r | all |
| getpwuid | all |
| getpwuid_r | all |
| getrlimit | all |
| getrusage | all |
| gets | all |
| getservbyname | all |
| getservbyport | all |
| getservent | all |
| getsid | 17 |
| getsockname | all |
| getsockopt | all |
| getsubopt | 26 |
| gettimeofday | all |
| getuid | all |
| getutxent | N/A |
| getutxid | N/A |
| getutxline | N/A |
| getwc | all |
| getwchar | all |
| glob | N/A |
| globfree | N/A |
| gmtime | all |
| gmtime_r | all |
| grantpt | 21 |
| hcreate | N/A |
| hdestroy | N/A |
| hsearch | N/A |
| htonl | 21 |
| htons | 21 |
| hypot | 18 |
| hypotf | 18 |
| hypotl | 21 |
| iconv | N/A |
| iconv_close | N/A |
| iconv_open | N/A |
| if_freenameindex | 24 |
| if_indextoname | all |
| if_nameindex | 24 |
| if_nametoindex | all |
| ilogb | 18 |
| ilogbf | 18 |
| ilogbl | 18 |
| imaxabs | 19 |
| imaxdiv | 19 |
| inet_addr | all |
| inet_ntoa | all |
| inet_ntop | all |
| inet_pton | all |
| initstate | 21 |
| insque | 21 |
| ioctl | all |
| isalnum | all |
| isalnum_l | 21 |
| isalpha | all |
| isalpha_l | 21 |
| isascii | all |
| isatty | all |
| isblank | all |
| isblank_l | 21 |
| iscntrl | all |
| iscntrl_l | 21 |
| isdigit | all |
| isdigit_l | 21 |
| isfinite | 21 |
| isgraph | all |
| isgraph_l | 21 |
| isinf | 21 |
| islower | all |
| islower_l | 21 |
| isnan | all |
| isnormal | 21 |
| isprint | all |
| isprint_l | 21 |
| ispunct | all |
| ispunct_l | 21 |
| isspace | all |
| isspace_l | 21 |
| isupper | all |
| isupper_l | 21 |
| iswalnum | all |
| iswalnum_l | 21 |
| iswalpha | all |
| iswalpha_l | 21 |
| iswblank | 21 |
| iswblank_l | 21 |
| iswcntrl | all |
| iswcntrl_l | 21 |
| iswctype | all |
| iswctype_l | 21 |
| iswdigit | all |
| iswdigit_l | 21 |
| iswgraph | all |
| iswgraph_l | 21 |
| iswlower | all |
| iswlower_l | 21 |
| iswprint | all |
| iswprint_l | 21 |
| iswpunct | all |
| iswpunct_l | 21 |
| iswspace | all |
| iswspace_l | 21 |
| iswupper | all |
| iswupper_l | 21 |
| iswxdigit | all |
| iswxdigit_l | 21 |
| isxdigit | all |
| isxdigit_l | 21 |
| j0 | 18 |
| j1 | 18 |
| jn | 18 |
| jrand48 | all |
| kill | all |
| killpg | all |
| labs | 19 |
| lchown | all |
| lcong48 | 23 |
| ldexp | all |
| ldexpf | 18 |
| ldexpl | 18 |
| ldiv | all |
| lfind | 21 |
| lgamma | 18 |
| lgammaf | 18 |
| lgammal | 21 |
| link | all |
| linkat | 21 |
| lio_listio | N/A |
| listen | all |
| llabs | 19 |
| lldiv | all |
| llrint | 18 |
| llrintf | 18 |
| llrintl | 21 |
| llround | 18 |
| llroundf | 18 |
| llroundl | 18 |
| localeconv | 21 |
| localtime | all |
| localtime_r | all |
| lockf | 24 |
| log | 18 |
| log10 | 18 |
| log10f | 18 |
| log10l | 21 |
| log1p | 18 |
| log1pf | 18 |
| log1pl | 21 |
| log2 | 18 |
| log2f | 18 |
| log2l | 18 |
| logb | 18 |
| logbf | 18 |
| logbl | 18 |
| logf | 18 |
| logl | 21 |
| longjmp | all |
| lrand48 | all |
| lrint | 18 |
| lrintf | 18 |
| lrintl | 21 |
| lround | 18 |
| lroundf | 18 |
| lroundl | 18 |
| lsearch | 21 |
| lseek | all |
| lstat | all |
| malloc | all |
| mblen | 26 |
| mbrlen | all |
| mbrtowc | all |
| mbsinit | all |
| mbsnrtowcs | 21 |
| mbsrtowcs | all |
| mbstowcs | all |
| mbtowc | 21 |
| memccpy | all |
| memchr | all |
| memcmp | all |
| memcpy | all |
| memmove | all |
| memset | all |
| mkdir | all |
| mkdirat | all |
| mkdtemp | all |
| mkfifo | 21 |
| mkfifoat | 23 |
| mknod | all |
| mknodat | 21 |
| mkstemp | all |
| mktime | all |
| mlock | all |
| mlockall | 17 |
| mmap | all |
| modf | 18 |
| modff | 18 |
| modfl | 21 |
| mprotect | all |
| mq_close | N/A |
| mq_getattr | N/A |
| mq_notify | N/A |
| mq_open | N/A |
| mq_receive | N/A |
| mq_send | N/A |
| mq_setattr | N/A |
| mq_timedreceive | N/A |
| mq_timedsend | N/A |
| mq_unlink | N/A |
| mrand48 | all |
| msgctl | 26 (but disabled in kernel) |
| msgget | 26 (but disabled in kernel) |
| msgrcv | 26 (but disabled in kernel) |
| msgsnd | 26 (but disabled in kernel) |
| msync | all |
| munlock | all |
| munlockall | 17 |
| munmap | all |
| nan | 18 |
| nanf | 18 |
| nanl | 18 |
| nanosleep | all |
| nearbyint | 18 |
| nearbyintf | 18 |
| nearbyintl | 21 |
| newlocale | 21 |
| nextafter | 18 |
| nextafterf | 18 |
| nextafterl | 21 |
| nexttoward | 18 |
| nexttowardf | 18 |
| nexttowardl | 18 |
| nftw | 17 |
| nice | all |
| nl_langinfo | 26 |
| nl_langinfo_l | 26 |
| nrand48 | all |
| ntohl | 21 |
| ntohs | 21 |
| open | all |
| open_memstream | 23 |
| open_wmemstream | 23 |
| openat | all |
| opendir | all |
| openlog | all |
| optarg | all |
| opterr | all |
| optind | all |
| optopt | all |
| pathconf | all |
| pause | all |
| pclose | all |
| perror | all |
| pipe | all |
| poll | all |
| popen | all |
| posix_fadvise | 21 |
| posix_fallocate | 21 |
| posix_madvise | 23 |
| posix_memalign | 16 |
| posix_openpt | 21 |
| posix_spawn | N/A |
| posix_spawn_file_actions_addclose | N/A |
| posix_spawn_file_actions_adddup2 | N/A |
| posix_spawn_file_actions_addopen | N/A |
| posix_spawn_file_actions_destroy | N/A |
| posix_spawn_file_actions_init | N/A |
| posix_spawnattr_destroy | N/A |
| posix_spawnattr_getflags | N/A |
| posix_spawnattr_getpgroup | N/A |
| posix_spawnattr_getschedparam | N/A |
| posix_spawnattr_getschedpolicy | N/A |
| posix_spawnattr_getsigdefault | N/A |
| posix_spawnattr_getsigmask | N/A |
| posix_spawnattr_init | N/A |
| posix_spawnattr_setflags | N/A |
| posix_spawnattr_setpgroup | N/A |
| posix_spawnattr_setschedparam | N/A |
| posix_spawnattr_setschedpolicy | N/A |
| posix_spawnattr_setsigdefault | N/A |
| posix_spawnattr_setsigmask | N/A |
| posix_spawnp | N/A |
| pow | 18 |
| powf | 18 |
| powl | 21 |
| pread | all |
| printf | all |
| pselect | all |
| psiginfo | 17 |
| psignal | 17 |
| pthread_atfork | all |
| pthread_attr_destroy | all |
| pthread_attr_getdetachstate | all |
| pthread_attr_getguardsize | all |
| pthread_attr_getinheritsched | N/A |
| pthread_attr_getschedparam | all |
| pthread_attr_getschedpolicy | all |
| pthread_attr_getscope | all |
| pthread_attr_getstack | all |
| pthread_attr_getstacksize | all |
| pthread_attr_init | all |
| pthread_attr_setdetachstate | all |
| pthread_attr_setguardsize | all |
| pthread_attr_setinheritsched | N/A |
| pthread_attr_setschedparam | all |
| pthread_attr_setschedpolicy | all |
| pthread_attr_setscope | all |
| pthread_attr_setstack | all |
| pthread_attr_setstacksize | all |
| pthread_barrier_destroy | 24 |
| pthread_barrier_init | 24 |
| pthread_barrier_wait | 24 |
| pthread_barrierattr_destroy | 24 |
| pthread_barrierattr_getpshared | 24 |
| pthread_barrierattr_init | 24 |
| pthread_barrierattr_setpshared | 24 |
| pthread_cancel | N/A |
| pthread_cleanup_pop | all |
| pthread_cleanup_push | all |
| pthread_cond_broadcast | all |
| pthread_cond_destroy | all |
| pthread_cond_init | all |
| pthread_cond_signal | all |
| pthread_cond_timedwait | all |
| pthread_cond_wait | all |
| pthread_condattr_destroy | all |
| pthread_condattr_getclock | 21 |
| pthread_condattr_getpshared | all |
| pthread_condattr_init | all |
| pthread_condattr_setclock | 21 |
| pthread_condattr_setpshared | all |
| pthread_create | all |
| pthread_detach | all |
| pthread_equal | all |
| pthread_exit | all |
| pthread_getconcurrency | N/A |
| pthread_getcpuclockid | all |
| pthread_getschedparam | all |
| pthread_getspecific | all |
| pthread_join | all |
| pthread_key_create | all |
| pthread_key_delete | all |
| pthread_kill | all |
| pthread_mutex_consistent | N/A |
| pthread_mutex_destroy | all |
| pthread_mutex_getprioceiling | N/A |
| pthread_mutex_init | all |
| pthread_mutex_lock | all |
| pthread_mutex_setprioceiling | N/A |
| pthread_mutex_timedlock | 21 |
| pthread_mutex_trylock | all |
| pthread_mutex_unlock | all |
| pthread_mutexattr_destroy | all |
| pthread_mutexattr_getprioceiling | N/A |
| pthread_mutexattr_getprotocol | N/A |
| pthread_mutexattr_getpshared | all |
| pthread_mutexattr_getrobust | N/A |
| pthread_mutexattr_gettype | all |
| pthread_mutexattr_init | all |
| pthread_mutexattr_setprioceiling | N/A |
| pthread_mutexattr_setprotocol | N/A |
| pthread_mutexattr_setpshared | all |
| pthread_mutexattr_setrobust | N/A |
| pthread_mutexattr_settype | all |
| pthread_once | all |
| pthread_rwlock_destroy | all |
| pthread_rwlock_init | all |
| pthread_rwlock_rdlock | all |
| pthread_rwlock_timedrdlock | all |
| pthread_rwlock_timedwrlock | all |
| pthread_rwlock_tryrdlock | all |
| pthread_rwlock_trywrlock | all |
| pthread_rwlock_unlock | all |
| pthread_rwlock_wrlock | all |
| pthread_rwlockattr_destroy | all |
| pthread_rwlockattr_getpshared | all |
| pthread_rwlockattr_init | all |
| pthread_rwlockattr_setpshared | all |
| pthread_self | all |
| pthread_setcancelstate | N/A |
| pthread_setcanceltype | N/A |
| pthread_setconcurrency | N/A |
| pthread_setschedparam | all |
| pthread_setschedprio | N/A |
| pthread_setspecific | all |
| pthread_sigmask | all |
| pthread_spin_destroy | 24 |
| pthread_spin_init | 24 |
| pthread_spin_lock | 24 |
| pthread_spin_trylock | 24 |
| pthread_spin_unlock | 24 |
| pthread_testcancel | N/A |
| ptsname | all |
| putc | all |
| putc_unlocked | all |
| putchar | all |
| putchar_unlocked | all |
| putenv | all |
| putmsg | N/A |
| putpmsg | N/A |
| puts | all |
| pututxline | N/A |
| putwc | all |
| putwchar | all |
| pwrite | all |
| qsort | all |
| raise | all |
| rand | 21 |
| rand_r | 21 |
| random | 21 |
| read | all |
| readdir | all |
| readdir_r | all |
| readlink | all |
| readlinkat | 21 |
| readv | all |
| realloc | all |
| realpath | all |
| recv | all |
| recvfrom | all |
| recvmsg | all |
| regcomp | all |
| regerror | all |
| regexec | all |
| regfree | all |
| remainder | 18 |
| remainderf | 18 |
| remainderl | 21 |
| remove | all |
| remque | 21 |
| remquo | 18 |
| remquof | 18 |
| remquol | 21 |
| rename | all |
| renameat | all |
| rewind | all |
| rewinddir | all |
| rint | 18 |
| rintf | 18 |
| rintl | 21 |
| rmdir | all |
| round | 18 |
| roundf | 18 |
| roundl | 18 |
| scalbln | 18 |
| scalblnf | 18 |
| scalblnl | 18 |
| scalbn | 18 |
| scalbnf | 18 |
| scalbnl | 18 |
| scandir | all |
| scanf | all |
| sched_get_priority_max | all |
| sched_get_priority_min | all |
| sched_getparam | all |
| sched_getscheduler | all |
| sched_rr_get_interval | all |
| sched_setparam | all |
| sched_setscheduler | all |
| sched_yield | all |
| seed48 | all |
| seekdir | 23 |
| select | all |
| sem_close | all |
| sem_destroy | all |
| sem_getvalue | all |
| sem_init | all |
| sem_open | all |
| sem_post | all |
| sem_timedwait | all |
| sem_trywait | all |
| sem_unlink | all |
| sem_wait | all |
| semctl | 26 (but disabled in kernel) |
| semget | 26 (but disabled in kernel) |
| semop | 26 (but disabled in kernel) |
| send | all |
| sendmsg | all |
| sendto | all |
| setbuf | all |
| setegid | all |
| setenv | all |
| seteuid | all |
| setgid | all |
| setgrent | N/A |
| sethostent | N/A |
| setitimer | all |
| setjmp | all |
| setkey | N/A |
| setlocale | all |
| setlogmask | all |
| setnetent | N/A |
| setpgid | all |
| setpgrp | all |
| setpriority | all |
| setprotoent | N/A |
| setpwent | N/A |
| setregid | all |
| setreuid | all |
| setrlimit | all |
| setservent | all |
| setsid | all |
| setsockopt | all |
| setstate | 21 |
| setuid | all |
| setutxent | N/A |
| setvbuf | all |
| shm_open | N/A |
| shm_unlink | N/A |
| shmat | 26 (but disabled in kernel) |
| shmctl | 26 (but disabled in kernel) |
| shmdt | 26 (but disabled in kernel) |
| shmget | 26 (but disabled in kernel) |
| shutdown | all |
| sigaction | all |
| sigaddset | 21 |
| sigaltstack | all |
| sigdelset | 21 |
| sigemptyset | 21 |
| sigfillset | 21 |
| sighold | 26 |
| sigignore | 26 |
| siginterrupt | all |
| sigismember | 21 |
| siglongjmp | all |
| signal | 21 |
| signbit | all (compiler builtin) |
| signgam | 18 |
| sigpause | 26 |
| sigpending | all |
| sigprocmask | all |
| sigqueue | 23 |
| sigrelse | 26 |
| sigset | 26 |
| sigsetjmp | all |
| sigsuspend | all |
| sigtimedwait | 23 |
| sigwait | all |
| sigwaitinfo | 23 |
| sin | 18 |
| sinf | 18 |
| sinh | 18 |
| sinhf | 18 |
| sinhl | 21 |
| sinl | 21 |
| sleep | all |
| snprintf | all |
| sockatmark | N/A |
| socket | all |
| socketpair | all |
| sprintf | all |
| sqrt | 18 |
| sqrtf | 18 |
| sqrtl | 21 |
| srand | 21 |
| srand48 | all |
| srandom | 21 |
| sscanf | all |
| stat | all |
| statvfs | 19 |
| stderr | 23 |
| stdin | 23 |
| stdout | 23 |
| stpcpy | 21 |
| stpncpy | 21 |
| strcasecmp | all |
| strcasecmp_l | 23 |
| strcat | all |
| strchr | all |
| strcmp | all |
| strcoll | all |
| strcoll_l | 21 |
| strcpy | all |
| strcspn | all |
| strdup | all |
| strerror | all |
| strerror_l | 23 |
| strerror_r | all |
| strfmon | N/A |
| strfmon_l | N/A |
| strftime | all |
| strftime_l | 21 |
| strlen | all |
| strncasecmp | all |
| strncasecmp_l | 23 |
| strncat | all |
| strncmp | all |
| strncpy | all |
| strndup | all |
| strnlen | all |
| strpbrk | all |
| strptime | all |
| strrchr | all |
| strsignal | all |
| strspn | all |
| strstr | all |
| strtod | all |
| strtof | 21 |
| strtoimax | all |
| strtok | all |
| strtok_r | all |
| strtol | all |
| strtold | 21 |
| strtoll | all |
| strtoul | all |
| strtoull | all |
| strtoumax | all |
| strxfrm | all |
| strxfrm_l | 21 |
| swab | N/A |
| swprintf | all |
| swscanf | all |
| symlink | all |
| symlinkat | 21 |
| sync | all |
| sysconf | all |
| syslog | all |
| system | all |
| tan | 18 |
| tanf | 18 |
| tanh | 18 |
| tanhf | 18 |
| tanhl | 21 |
| tanl | 21 |
| tcdrain | 21 |
| tcflow | 21 |
| tcflush | 21 |
| tcgetattr | 21 |
| tcgetpgrp | all |
| tcgetsid | 21 |
| tcsendbreak | 21 |
| tcsetattr | 21 |
| tcsetpgrp | all |
| tdelete | 16 |
| telldir | 23 |
| tempnam | all |
| tfind | 16 |
| tgamma | 18 |
| tgammaf | 18 |
| tgammal | 21 |
| time | all |
| timer_create | all |
| timer_delete | all |
| timer_getoverrun | all |
| timer_gettime | all |
| timer_settime | all |
| times | all |
| timezone | all |
| tmpfile | all |
| tmpnam | all |
| toascii | all |
| tolower | all |
| tolower_l | 21 |
| toupper | all |
| toupper_l | 21 |
| towctrans | 26 |
| towctrans_l | 26 |
| towlower | all |
| towlower_l | 21 |
| towupper | all |
| towupper_l | 21 |
| trunc | 18 |
| truncate | all |
| truncf | 18 |
| truncl | 18 |
| tsearch | 16 |
| ttyname | all |
| ttyname_r | all |
| twalk | 21 |
| tzname | all |
| tzset | all |
| ulimit | N/A |
| umask | all |
| uname | all |
| ungetc | all |
| ungetwc | all |
| unlink | all |
| unlinkat | all |
| unlockpt | all |
| unsetenv | all |
| uselocale | 21 |
| utime | all |
| utimensat | all |
| utimes | all |
| vdprintf | 21 |
| vfprintf | all |
| vfscanf | all |
| vfwprintf | all |
| vfwscanf | 21 |
| vprintf | all |
| vscanf | all |
| vsnprintf | all |
| vsprintf | all |
| vsscanf | all |
| vswprintf | all |
| vswscanf | 21 |
| vwprintf | all |
| vwscanf | 21 |
| wait | all |
| waitid | all |
| waitpid | all |
| wcpcpy | all |
| wcpncpy | all |
| wcrtomb | all |
| wcscasecmp | all |
| wcscasecmp_l | 23 |
| wcscat | all |
| wcschr | all |
| wcscmp | all |
| wcscoll | all |
| wcscoll_l | 21 |
| wcscpy | all |
| wcscspn | all |
| wcsdup | all |
| wcsftime | all |
| wcslen | all |
| wcsncasecmp | all |
| wcsncasecmp_l | 23 |
| wcsncat | all |
| wcsncmp | all |
| wcsncpy | all |
| wcsnlen | all |
| wcsnrtombs | 21 |
| wcspbrk | all |
| wcsrchr | all |
| wcsrtombs | all |
| wcsspn | all |
| wcsstr | all |
| wcstod | all |
| wcstof | 21 |
| wcstoimax | 21 |
| wcstok | all |
| wcstol | all |
| wcstold | 21 |
| wcstoll | 21 |
| wcstombs | all |
| wcstoul | all |
| wcstoull | 21 |
| wcstoumax | 21 |
| wcswidth | all |
| wcsxfrm | all |
| wcsxfrm_l | 21 |
| wctob | all |
| wctomb | 21 |
| wctrans | 26 |
| wctrans_l | 26 |
| wctype | all |
| wctype_l | 21 |
| wcwidth | all |
| wmemchr | all |
| wmemcmp | all |
| wmemcpy | all |
| wmemmove | all |
| wmemset | all |
| wordexp | N/A |
| wordfree | N/A |
| wprintf | all |
| write | all |
| writev | all |
| wscanf | all |
| y0 | 18 |
| y1 | 18 |
| yn | 18 |
