/*
 * Copyright (C) 2012 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

// G++ automatically defines _GNU_SOURCE, which then means that <string.h>
// gives us the GNU variant.
#undef _GNU_SOURCE

#include <string.h>

#include <errno.h>
#include <limits.h>

#include <async_safe/log.h>

#include "private/ErrnoRestorer.h"

#include <string.h>

#include "bionic/pthread_internal.h"

struct ErrorInfo {
  const char* name;
  const char* desc;
};

#define ERROR_INFO_MAP_ENTRY(_errno, _desc) [_errno] = {.name = #_errno, .desc = _desc}

static const struct ErrorInfo __sys_error_infos[] = {
    [0] = {.name = "SUCCESS", .desc = "Success"},
    ERROR_INFO_MAP_ENTRY(EPERM, "Operation not permitted"),
    ERROR_INFO_MAP_ENTRY(ENOENT, "No such file or directory"),
    ERROR_INFO_MAP_ENTRY(ESRCH, "No such process"),
    ERROR_INFO_MAP_ENTRY(EINTR, "Interrupted system call"),
    ERROR_INFO_MAP_ENTRY(EIO, "I/O error"),
    ERROR_INFO_MAP_ENTRY(ENXIO, "No such device or address"),
    ERROR_INFO_MAP_ENTRY(E2BIG, "Argument list too long"),
    ERROR_INFO_MAP_ENTRY(ENOEXEC, "Exec format error"),
    ERROR_INFO_MAP_ENTRY(EBADF, "Bad file descriptor"),
    ERROR_INFO_MAP_ENTRY(ECHILD, "No child processes"),
    ERROR_INFO_MAP_ENTRY(EAGAIN, "Try again"),
    ERROR_INFO_MAP_ENTRY(ENOMEM, "Out of memory"),
    ERROR_INFO_MAP_ENTRY(EACCES, "Permission denied"),
    ERROR_INFO_MAP_ENTRY(EFAULT, "Bad address"),
    ERROR_INFO_MAP_ENTRY(ENOTBLK, "Block device required"),
    ERROR_INFO_MAP_ENTRY(EBUSY, "Device or resource busy"),
    ERROR_INFO_MAP_ENTRY(EEXIST, "File exists"),
    ERROR_INFO_MAP_ENTRY(EXDEV, "Cross-device link"),
    ERROR_INFO_MAP_ENTRY(ENODEV, "No such device"),
    ERROR_INFO_MAP_ENTRY(ENOTDIR, "Not a directory"),
    ERROR_INFO_MAP_ENTRY(EISDIR, "Is a directory"),
    ERROR_INFO_MAP_ENTRY(EINVAL, "Invalid argument"),
    ERROR_INFO_MAP_ENTRY(ENFILE, "File table overflow"),
    ERROR_INFO_MAP_ENTRY(EMFILE, "Too many open files"),
    ERROR_INFO_MAP_ENTRY(ENOTTY, "Inappropriate ioctl for device"),
    ERROR_INFO_MAP_ENTRY(ETXTBSY, "Text file busy"),
    ERROR_INFO_MAP_ENTRY(EFBIG, "File too large"),
    ERROR_INFO_MAP_ENTRY(ENOSPC, "No space left on device"),
    ERROR_INFO_MAP_ENTRY(ESPIPE, "Illegal seek"),
    ERROR_INFO_MAP_ENTRY(EROFS, "Read-only file system"),
    ERROR_INFO_MAP_ENTRY(EMLINK, "Too many links"),
    ERROR_INFO_MAP_ENTRY(EPIPE, "Broken pipe"),
    ERROR_INFO_MAP_ENTRY(EDOM, "Math argument out of domain of func"),
    ERROR_INFO_MAP_ENTRY(ERANGE, "Math result not representable"),
    ERROR_INFO_MAP_ENTRY(EDEADLK, "Resource deadlock would occur"),
    ERROR_INFO_MAP_ENTRY(ENAMETOOLONG, "File name too long"),
    ERROR_INFO_MAP_ENTRY(ENOLCK, "No record locks available"),
    ERROR_INFO_MAP_ENTRY(ENOSYS, "Function not implemented"),
    ERROR_INFO_MAP_ENTRY(ENOTEMPTY, "Directory not empty"),
    ERROR_INFO_MAP_ENTRY(ELOOP, "Too many symbolic links encountered"),
    ERROR_INFO_MAP_ENTRY(ENOMSG, "No message of desired type"),
    ERROR_INFO_MAP_ENTRY(EIDRM, "Identifier removed"),
    ERROR_INFO_MAP_ENTRY(ECHRNG, "Channel number out of range"),
    ERROR_INFO_MAP_ENTRY(EL2NSYNC, "Level 2 not synchronized"),
    ERROR_INFO_MAP_ENTRY(EL3HLT, "Level 3 halted"),
    ERROR_INFO_MAP_ENTRY(EL3RST, "Level 3 reset"),
    ERROR_INFO_MAP_ENTRY(ELNRNG, "Link number out of range"),
    ERROR_INFO_MAP_ENTRY(EUNATCH, "Protocol driver not attached"),
    ERROR_INFO_MAP_ENTRY(ENOCSI, "No CSI structure available"),
    ERROR_INFO_MAP_ENTRY(EL2HLT, "Level 2 halted"),
    ERROR_INFO_MAP_ENTRY(EBADE, "Invalid exchange"),
    ERROR_INFO_MAP_ENTRY(EBADR, "Invalid request descriptor"),
    ERROR_INFO_MAP_ENTRY(EXFULL, "Exchange full"),
    ERROR_INFO_MAP_ENTRY(ENOANO, "No anode"),
    ERROR_INFO_MAP_ENTRY(EBADRQC, "Invalid request code"),
    ERROR_INFO_MAP_ENTRY(EBADSLT, "Invalid slot"),
    ERROR_INFO_MAP_ENTRY(EBFONT, "Bad font file format"),
    ERROR_INFO_MAP_ENTRY(ENOSTR, "Device not a stream"),
    ERROR_INFO_MAP_ENTRY(ENODATA, "No data available"),
    ERROR_INFO_MAP_ENTRY(ETIME, "Timer expired"),
    ERROR_INFO_MAP_ENTRY(ENOSR, "Out of streams resources"),
    ERROR_INFO_MAP_ENTRY(ENONET, "Machine is not on the network"),
    ERROR_INFO_MAP_ENTRY(ENOPKG, "Package not installed"),
    ERROR_INFO_MAP_ENTRY(EREMOTE, "Object is remote"),
    ERROR_INFO_MAP_ENTRY(ENOLINK, "Link has been severed"),
    ERROR_INFO_MAP_ENTRY(EADV, "Advertise error"),
    ERROR_INFO_MAP_ENTRY(ESRMNT, "Srmount error"),
    ERROR_INFO_MAP_ENTRY(ECOMM, "Communication error on send"),
    ERROR_INFO_MAP_ENTRY(EPROTO, "Protocol error"),
    ERROR_INFO_MAP_ENTRY(EMULTIHOP, "Multihop attempted"),
    ERROR_INFO_MAP_ENTRY(EDOTDOT, "RFS specific error"),
    ERROR_INFO_MAP_ENTRY(EBADMSG, "Not a data message"),
    ERROR_INFO_MAP_ENTRY(EOVERFLOW, "Value too large for defined data type"),
    ERROR_INFO_MAP_ENTRY(ENOTUNIQ, "Name not unique on network"),
    ERROR_INFO_MAP_ENTRY(EBADFD, "File descriptor in bad state"),
    ERROR_INFO_MAP_ENTRY(EREMCHG, "Remote address changed"),
    ERROR_INFO_MAP_ENTRY(ELIBACC, "Can not access a needed shared library"),
    ERROR_INFO_MAP_ENTRY(ELIBBAD, "Accessing a corrupted shared library"),
    ERROR_INFO_MAP_ENTRY(ELIBSCN, ".lib section in a.out corrupted"),
    ERROR_INFO_MAP_ENTRY(ELIBMAX, "Attempting to link in too many shared libraries"),
    ERROR_INFO_MAP_ENTRY(ELIBEXEC, "Cannot exec a shared library directly"),
    ERROR_INFO_MAP_ENTRY(EILSEQ, "Illegal byte sequence"),
    ERROR_INFO_MAP_ENTRY(ERESTART, "Interrupted system call should be restarted"),
    ERROR_INFO_MAP_ENTRY(ESTRPIPE, "Streams pipe error"),
    ERROR_INFO_MAP_ENTRY(EUSERS, "Too many users"),
    ERROR_INFO_MAP_ENTRY(ENOTSOCK, "Socket operation on non-socket"),
    ERROR_INFO_MAP_ENTRY(EDESTADDRREQ, "Destination address required"),
    ERROR_INFO_MAP_ENTRY(EMSGSIZE, "Message too long"),
    ERROR_INFO_MAP_ENTRY(EPROTOTYPE, "Protocol wrong type for socket"),
    ERROR_INFO_MAP_ENTRY(ENOPROTOOPT, "Protocol not available"),
    ERROR_INFO_MAP_ENTRY(EPROTONOSUPPORT, "Protocol not supported"),
    ERROR_INFO_MAP_ENTRY(ESOCKTNOSUPPORT, "Socket type not supported"),
    ERROR_INFO_MAP_ENTRY(EOPNOTSUPP, "Operation not supported on transport endpoint"),
    ERROR_INFO_MAP_ENTRY(EPFNOSUPPORT, "Protocol family not supported"),
    ERROR_INFO_MAP_ENTRY(EAFNOSUPPORT, "Address family not supported by protocol"),
    ERROR_INFO_MAP_ENTRY(EADDRINUSE, "Address already in use"),
    ERROR_INFO_MAP_ENTRY(EADDRNOTAVAIL, "Cannot assign requested address"),
    ERROR_INFO_MAP_ENTRY(ENETDOWN, "Network is down"),
    ERROR_INFO_MAP_ENTRY(ENETUNREACH, "Network is unreachable"),
    ERROR_INFO_MAP_ENTRY(ENETRESET, "Network dropped connection because of reset"),
    ERROR_INFO_MAP_ENTRY(ECONNABORTED, "Software caused connection abort"),
    ERROR_INFO_MAP_ENTRY(ECONNRESET, "Connection reset by peer"),
    ERROR_INFO_MAP_ENTRY(ENOBUFS, "No buffer space available"),
    ERROR_INFO_MAP_ENTRY(EISCONN, "Transport endpoint is already connected"),
    ERROR_INFO_MAP_ENTRY(ENOTCONN, "Transport endpoint is not connected"),
    ERROR_INFO_MAP_ENTRY(ESHUTDOWN, "Cannot send after transport endpoint shutdown"),
    ERROR_INFO_MAP_ENTRY(ETOOMANYREFS, "Too many references: cannot splice"),
    ERROR_INFO_MAP_ENTRY(ETIMEDOUT, "Connection timed out"),
    ERROR_INFO_MAP_ENTRY(ECONNREFUSED, "Connection refused"),
    ERROR_INFO_MAP_ENTRY(EHOSTDOWN, "Host is down"),
    ERROR_INFO_MAP_ENTRY(EHOSTUNREACH, "No route to host"),
    ERROR_INFO_MAP_ENTRY(EALREADY, "Operation already in progress"),
    ERROR_INFO_MAP_ENTRY(EINPROGRESS, "Operation now in progress"),
    ERROR_INFO_MAP_ENTRY(ESTALE, "Stale NFS file handle"),
    ERROR_INFO_MAP_ENTRY(EUCLEAN, "Structure needs cleaning"),
    ERROR_INFO_MAP_ENTRY(ENOTNAM, "Not a XENIX named type file"),
    ERROR_INFO_MAP_ENTRY(ENAVAIL, "No XENIX semaphores available"),
    ERROR_INFO_MAP_ENTRY(EISNAM, "Is a named type file"),
    ERROR_INFO_MAP_ENTRY(EREMOTEIO, "Remote I/O error"),
    ERROR_INFO_MAP_ENTRY(EDQUOT, "Quota exceeded"),
    ERROR_INFO_MAP_ENTRY(ENOMEDIUM, "No medium found"),
    ERROR_INFO_MAP_ENTRY(EMEDIUMTYPE, "Wrong medium type"),
    ERROR_INFO_MAP_ENTRY(ECANCELED, "Operation Canceled"),
    ERROR_INFO_MAP_ENTRY(ENOKEY, "Required key not available"),
    ERROR_INFO_MAP_ENTRY(EKEYEXPIRED, "Key has expired"),
    ERROR_INFO_MAP_ENTRY(EKEYREVOKED, "Key has been revoked"),
    ERROR_INFO_MAP_ENTRY(EKEYREJECTED, "Key was rejected by service"),
    ERROR_INFO_MAP_ENTRY(EOWNERDEAD, "Owner died"),
    ERROR_INFO_MAP_ENTRY(ENOTRECOVERABLE, "State not recoverable"),
    ERROR_INFO_MAP_ENTRY(ERFKILL, "Operation not possible due to RF-kill"),
    ERROR_INFO_MAP_ENTRY(EHWPOISON, "Memory page has hardware error"),
};

static inline const struct ErrorInfo* __error_info_lookup(int error_number) {
  if (error_number < 0 || error_number >= static_cast<int>(arraysize(__sys_error_infos))) {
    return nullptr;
  }
  return &__sys_error_infos[error_number];
}

static inline const char* __strerror_lookup(int error_number) {
  const struct ErrorInfo* info = __error_info_lookup(error_number);
  return (info == nullptr) ? nullptr : info->desc;
}

int strerror_r(int error_number, char* buf, size_t buf_len) {
  ErrnoRestorer errno_restorer;
  size_t length;

  const char* error_name = __strerror_lookup(error_number);
  if (error_name != nullptr) {
    length = strlcpy(buf, error_name, buf_len);
  } else {
    length = async_safe_format_buffer(buf, buf_len, "Unknown error %d", error_number);
  }
  if (length >= buf_len) {
    return ERANGE;
  }

  return 0;
}

extern "C" char* __gnu_strerror_r(int error_number, char* buf, size_t buf_len) {
  ErrnoRestorer errno_restorer; // The glibc strerror_r doesn't set errno if it truncates...
  strerror_r(error_number, buf, buf_len);
  return buf; // ...and just returns whatever fit.
}

char* strerror(int error_number) {
  // Just return the original constant in the easy cases.
  char* result = const_cast<char*>(__strerror_lookup(error_number));
  if (result != nullptr) {
    return result;
  }

  bionic_tls& tls = __get_bionic_tls();
  result = tls.strerror_buf;
  strerror_r(error_number, result, sizeof(tls.strerror_buf));
  return result;
}

extern "C" const char* __gnu_strerrorname_np(int error_number) {
  const struct ErrorInfo* info = __error_info_lookup(error_number);
  return (info == nullptr) ? nullptr : info->name;
}

extern "C" const char* __gnu_strerrordesc_np(int error_number) {
  const struct ErrorInfo* info = __error_info_lookup(error_number);
  return (info == nullptr) ? nullptr : info->desc;
}
