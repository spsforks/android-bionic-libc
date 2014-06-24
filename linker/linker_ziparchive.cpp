/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "linker_ziparchive.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "private/ScopedMmap.h"
#include "private/bionic_macros.h"

// TODO: this code won't work on big-endian architectures.
// TODO: do we need ZIP64 support here?

#define ZIP_GDBF_ENCRYPTED 0x0001

namespace {

struct EocdRecord {
  static const uint32_t kSignature = 0x06054b50;

  // The start of record signature. Must be |kSignature|.
  uint32_t eocd_signature;
  uint16_t disk_num; // ignored
  uint16_t cd_start_disk_num; // ignored
  uint16_t records_on_disk_num; // ignored
  uint16_t records_num;
  uint32_t cd_size;
  uint32_t cd_start_offset;
  uint16_t comment_length; // ignored
} __attribute__((packed));

struct CentralDirectoryRecord {
  static const uint32_t kSignature = 0x02014b50;

  // The start of record signature. Must be |kSignature|.
  uint32_t record_signature;
  uint16_t version_made_by; // ignored
  uint16_t version_needed; // ignored
  uint16_t gpb_flags; // bit 0 (indicates encryption) should be clear
  uint16_t compression_method; // should be 0 (STORED) for our purposes
  uint16_t last_mod_time; // ignored
  uint16_t last_mod_date; // ignored
  uint32_t crc32; // ignored
  uint32_t compressed_size; // ignored
  uint32_t uncompressed_size; // local file size
  uint16_t file_name_length;
  uint16_t extra_field_length; // used to calculate offset for the next record
  uint16_t comment_length; // used to calculate offset for the next record
  uint16_t file_start_disk; // ignored
  uint16_t internal_file_attributes; // ignored
  uint32_t external_file_attributes; // ignored
  // The offset to the local file header for this entry, from the
  // beginning of this archive.
  uint32_t local_file_header_offset;
 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(CentralDirectoryRecord);
} __attribute__((packed));

struct CentralDirectoryInfo {
  uint32_t start_offset;
  uint32_t size;
  uint16_t records_num;
};

// We use file_name_length & extra_field_length to calculate
// offset to file data, everything else is ignored.
struct LocalFileHeader {
  static const uint32_t kSignature = 0x04034b50;

  // The local file header signature, must be |kSignature|.
  uint32_t lfh_signature;
  uint16_t version_needed;
  uint16_t gpb_flags;
  uint16_t compression_method;
  uint16_t last_mod_time;
  uint16_t last_mod_date;
  uint32_t crc32;
  uint32_t compressed_size;
  uint32_t uncompressed_size;
  // the following two fields are used to calculate
  // data_offset of the file.
  uint16_t file_name_length;
  uint16_t extra_field_length;
 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(LocalFileHeader);
} __attribute__((packed));

};

static bool find_central_directory(int fd, CentralDirectoryInfo& cd_info,char* error_buf, size_t error_buf_size) noexcept {
#define _ERROR_ERRNO(err_msg) \
  snprintf(error_buf, error_buf_size, "%s: %s", err_msg, strerror(errno)); \
  return false;
#define _ERROR(err_msg) \
  strlcpy(error_buf, err_msg, error_buf_size); \
  return false
  // Look for End of Central Directory Record
  off_t file_size = lseek(fd, 0, SEEK_END);
  if (file_size == -1) {
    _ERROR_ERRNO("seek to the end");
  }

  if (static_cast<size_t>(file_size) < sizeof(EocdRecord)) {
    _ERROR("invalid file format");
  }

  // size of end of central directory record
  // + max size of comment.
  size_t length = sizeof(EocdRecord) + 0xFFFFU;

  // cap it by file length
  if (length > static_cast<size_t>(file_size)) {
    length = static_cast<size_t>(file_size);
  }

  // mmap end of file and start looking for eocd record
  ScopedMmap buf_mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, file_size - length);
  if (buf_mmap.get() == MAP_FAILED) {
    _ERROR_ERRNO("find_central_directory mmap");
  }

  uint8_t* buf_bytes = reinterpret_cast<uint8_t*>(buf_mmap.get());

  off_t eocd_record_mmap_offset = length - sizeof(EocdRecord);
  while (eocd_record_mmap_offset >= 0
      && *reinterpret_cast<uint32_t*>(buf_bytes+eocd_record_mmap_offset) != EocdRecord::kSignature) {
    --eocd_record_mmap_offset;
  }

  if (eocd_record_mmap_offset < 0) {
    _ERROR("invalid file format: eocd record not found");
  }

  // calculate start of central directory and end of central directory
  const EocdRecord* eocd_record = reinterpret_cast<EocdRecord*>(buf_bytes + eocd_record_mmap_offset);
  cd_info.start_offset = eocd_record->cd_start_offset;
  cd_info.size = eocd_record->cd_size;
  cd_info.records_num = eocd_record->records_num;

  return true;
#undef _ERROR_ERRNO
#undef _ERROR
}

off_t find_in_zip(int zip_fd, const char* file_name, char* error_buf, size_t error_buf_size) noexcept {
#define _ERROR_ERRNO(err_msg) \
  snprintf(error_buf, error_buf_size, "%s: %s", err_msg, strerror(errno)); \
  return -1;
#define _ERROR(err_msg) \
  strlcpy(error_buf, err_msg, error_buf_size); \
  return -1

  CentralDirectoryInfo cd_info;
  if (!find_central_directory(zip_fd, cd_info, error_buf, error_buf_size)) {
    return false;
  }

  // map central directory
  ScopedMmap cd_mmap(NULL, cd_info.size, PROT_READ, MAP_PRIVATE, zip_fd, cd_info.start_offset);
  if (cd_mmap.get() == MAP_FAILED) {
    _ERROR_ERRNO("mmap central directory");
  }

  const uint8_t* bytes = reinterpret_cast<uint8_t*>(cd_mmap.get());

  size_t file_name_length = strlen(file_name);

  bool entry_found = false;
  uint32_t local_header_offset = 0;

  for (size_t i = 0; i < cd_info.records_num; ++i) {
    const CentralDirectoryRecord* rec = reinterpret_cast<const CentralDirectoryRecord*>(bytes);
    if (rec->record_signature != CentralDirectoryRecord::kSignature) {
      _ERROR("invalid file format: unexpected central directory record signature");
    }

    if (rec->compression_method == 0
        && (rec->gpb_flags & ZIP_GDBF_ENCRYPTED) == 0
        && rec->file_name_length == file_name_length
        && strncmp(reinterpret_cast<const char*>(bytes+sizeof(CentralDirectoryRecord)), file_name, file_name_length) == 0) {
      entry_found = true;
      local_header_offset = rec->local_file_header_offset;
      break;
    }

    bytes += sizeof(CentralDirectoryRecord) + rec->file_name_length + rec->extra_field_length + rec->comment_length;
  }

  if (!entry_found) {
    *error_buf = '\0';
    return -1;
  }

  // next - read local header
  if (lseek(zip_fd, local_header_offset, SEEK_SET) == -1) {
    _ERROR_ERRNO("seek to local header offset");
  }

  uint8_t local_file_header_buf[sizeof(LocalFileHeader)];

  ssize_t actual = TEMP_FAILURE_RETRY(read(zip_fd, local_file_header_buf, sizeof(local_file_header_buf)));

  if (actual != sizeof(local_file_header_buf)) {
    _ERROR("error reading local_file_header");
  }

  const LocalFileHeader* local_file_header = reinterpret_cast<LocalFileHeader*>(local_file_header_buf);
  if (local_file_header->lfh_signature != LocalFileHeader::kSignature) {
    _ERROR("invalid local file header signature");
  }

  off_t data_offset = local_header_offset + sizeof(LocalFileHeader)
      + local_file_header->file_name_length + local_file_header->extra_field_length;

  if ((data_offset % PAGE_SIZE) != 0) {
    _ERROR("file data is misaligned");
  }

  // seek to start of file data
  if (lseek(zip_fd, data_offset, SEEK_SET) == -1) {
    _ERROR_ERRNO("seek to file data");
  }

  return data_offset;
#undef _ERROR
#undef _ERROR_ERRNO
}
