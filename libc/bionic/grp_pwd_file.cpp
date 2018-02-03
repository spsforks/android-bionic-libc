/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <grp.h>
#include <pwd.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "private/bionic_lock.h"

namespace {

template <size_t kNumFields>
class FieldsBase {
 public:
  char* ParseLine(char* line_begin, const char* end) {
    size_t fields_written = 0;
    char* position = line_begin;
    fields_[fields_written++] = position;

    while (position < end && fields_written < kNumFields) {
      if (*position == '\0') {
        fields_[fields_written++] = position + 1;
      }
      position++;
    }

    while (position < end && *position != '\0') {
      position++;
    };

    return position;
  }

  char* fields_[kNumFields] = { 0 };
};

class PasswdFields : public FieldsBase<7> {
 public:
  char* name() const {
    return fields_[0];
  }
  // Password is not supported.
  char* uid() const {
    return fields_[2];
  }
  char* gid() const {
    return fields_[3];
  }
  // User Info is not supported
  char* dir() const {
    return fields_[5];
  }
  char* shell() const {
    return fields_[6];
  }

  bool ToPasswd(passwd* result) {
    result->pw_name = name();
    result->pw_passwd = nullptr;

    char* end = nullptr;
    errno = 0;
    result->pw_uid = strtoul(uid(), &end, 0);
    if (errno != 0 || uid() == end || *end != '\0') {
      return false;
    }

    errno = 0;
    result->pw_gid = strtoul(gid(), &end, 0);
    if (errno != 0 || gid() == end || *end != '\0') {
      return false;
    }

#ifdef __LP64__
    result->pw_gecos = nullptr;
#endif

    result->pw_dir = dir();
    result->pw_shell = shell();

    return true;
  }
};

class GroupFields : public FieldsBase<4> {
 public:
  char* name() const {
    return fields_[0];
  }
  // Password is not supported.
  char* gid() const {
    return fields_[2];
  }
  char* user_list() const {
    return fields_[3];
  }

  bool ToGroup(group* result) {
    result->gr_name = name();
    result->gr_passwd = nullptr;

    char* end = nullptr;
    errno = 0;
    result->gr_gid = strtoul(gid(), &end, 0);
    if (errno != 0 || gid() == end || *end != '\0') {
      return false;
    }

    result->gr_mem = nullptr;

    return true;
  }
};

template <typename File, typename Predicate>
bool Find(File* file, typename File::Fields* result, Predicate predicate) {
  char* start;
  const char* end;
  if (!file->GetFile(&start, &end)) {
    return false;
  }

  char* line_beginning = start;

  do {
    line_beginning = result->ParseLine(line_beginning, end) + 1;
    if (predicate(result)) return true;

  } while (line_beginning < end);

  return false;
}

template <typename File, typename Id>
bool FindById(File* file, Id id, typename File::Fields* result) {
  return Find(file, result, [id](const auto& line) {
    char* end = nullptr;
    errno = 0;
    Id id_line = strtoul(line->fields_[2], &end, 0);
    if (errno != 0 || line->fields_[2] == end || *end != '\0') {
      return false;
    }
    return id_line == id;
  });
}

template <typename File>
bool FindByName(File* file, const char* name, typename File::Fields* result) {
  return Find(file, result, [name](const auto& line) { return !strcmp(line->fields_[0], name); });
}

void ConvertCharsToNull(char* start, const char* end) {
  while (start != end) {
    if (*start == ':' || *start == '\n') {
      *start = '\0';
    }
    start++;
  }
}

class MmapFileBase {
 public:
  MmapFileBase(const char* filename) : filename_(filename) {
  }

  bool GetFile(char** start, const char** end) {
    if (status_ == FileStatus::Initialized) {
      *start = start_;
      *end = end_;
      return true;
    }
    if (status_ == FileStatus::Error) {
      return false;
    }

    LockGuard guard(lock_);
    if (status_ == FileStatus::Initialized) {
      *start = start_;
      *end = end_;
      return true;
    }
    if (status_ == FileStatus::Error) {
      return false;
    }

    if (!MmapFile()) {
      status_ = FileStatus::Error;
      return false;
    }

    ConvertCharsToNull(start_, end_);

    // the passwd and group structs are non-const pointers, so try to save people from themselves
    // by making this range RO after we've converted the characters.
    mprotect(start_, end_ - start_, PROT_READ);

    status_ = FileStatus::Initialized;
    *start = start_;
    *end = end_;
    return true;
  }

 private:
  enum class FileStatus {
    Uninitialized,
    Initialized,
    Error,
  };

  bool MmapFile() {
    int fd = open(filename_, O_CLOEXEC | O_NOFOLLOW | O_RDONLY);

    struct stat fd_stat;
    if (fstat(fd, &fd_stat) < 0) {
      close(fd);
      return false;
    }

    auto mmap_size = fd_stat.st_size;

    void* map_result = mmap(nullptr, mmap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (map_result == MAP_FAILED) {
      close(fd);
      return false;
    }

    start_ = static_cast<char*>(map_result);
    end_ = start_ + mmap_size;
    close(fd);
    return true;
  }

  FileStatus status_ = FileStatus::Uninitialized;
  Lock lock_;
  const char* filename_ = nullptr;
  char* start_ = nullptr;
  const char* end_ = nullptr;
};

class PasswdFile : public MmapFileBase {
 public:
  using Fields = PasswdFields;

  PasswdFile(const char* filename) : MmapFileBase(filename) {
  }
};

class GroupFile : public MmapFileBase {
 public:
  using Fields = GroupFields;

  GroupFile(const char* filename) : MmapFileBase(filename) {
  }
};

PasswdFile passwd_files[] = {
  { "/system/etc/passwd" },
  { "/vendor/etc/passwd" },
};

GroupFile group_files[] = {
  { "/system/etc/group" },
  { "/vendor/etc/group" },
};

}  // namespace

bool FindPasswdById(uid_t uid, passwd* result) {
  PasswdFields passwd_fields;
  for (auto& passwd_file : passwd_files) {
    if (FindById(&passwd_file, uid, &passwd_fields)) {
      return passwd_fields.ToPasswd(result);
    }
  }
  return false;
}

bool FindPasswdByName(const char* name, passwd* result) {
  PasswdFields passwd_fields;
  for (auto& passwd_file : passwd_files) {
    if (FindByName(&passwd_file, name, &passwd_fields)) {
      return passwd_fields.ToPasswd(result);
    }
  }
  return false;
}

bool FindGroupById(gid_t gid, group* result) {
  GroupFields group_fields;
  for (auto& group_file : group_files) {
    if (FindById(&group_file, gid, &group_fields)) {
      return group_fields.ToGroup(result);
    }
  }
  return false;
}

bool FindGroupByName(const char* name, group* result) {
  GroupFields group_fields;
  for (auto& group_file : group_files) {
    if (FindByName(&group_file, name, &group_fields)) {
      return group_fields.ToGroup(result);
    }
  }
  return false;
}
