/*
 * Copyright (C) 2014 The Android Open Source Project
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

// Ported from chromium:
// src/third_party/android_crazy_linker/src/src/crazy_linker_zip.cpp

#include "linker_zip.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "linker_debug.h"

namespace {

// All offsets are given in bytes relative to the start of the header.
// Arithmetic is used to indicate the size of small fields that are skipped.

// http://www.pkware.com/documents/casestudies/APPNOTE.TXT Section 4.3.16
// This marker appears at the start of the end of central directory record
const uint32_t kEndOfCentralDirectoryMarker = 0x06054b50;

// Offsets of fields in End of Central Directory.
const int kOffsetNumOfEntriesInEndOfCentralDirectory = 4 + 2 + 2;
const int kOffsetOfCentralDirLengthInEndOfCentralDirectory =
    kOffsetNumOfEntriesInEndOfCentralDirectory + 2 + 2;
const int kOffsetOfStartOfCentralDirInEndOfCentralDirectory =
    kOffsetOfCentralDirLengthInEndOfCentralDirectory + 4;

// http://www.pkware.com/documents/casestudies/APPNOTE.TXT Section 4.3.12
// This marker appears at the start of the central directory
const uint32_t kCentralDirHeaderMarker = 0x2014b50;

// Offsets of fields in Central Directory Header.
const int kOffsetFilenameLengthInCentralDirectory =
    4 + 2 + 2 + 2 + 2 + 2 + 2 + 4 + 4 + 4;
const int kOffsetExtraFieldLengthInCentralDirectory =
    kOffsetFilenameLengthInCentralDirectory + 2;
const int kOffsetCommentLengthInCentralDirectory =
    kOffsetExtraFieldLengthInCentralDirectory + 2;
const int kOffsetLocalHeaderOffsetInCentralDirectory =
    kOffsetCommentLengthInCentralDirectory + 2 + 2 + 2 + 4;
const int kOffsetFilenameInCentralDirectory =
    kOffsetLocalHeaderOffsetInCentralDirectory + 4;

// http://www.pkware.com/documents/casestudies/APPNOTE.TXT Section 4.3.7
// This marker appears at the start of local header
const uint32_t kLocalHeaderMarker = 0x04034b50;

// http://www.pkware.com/documents/casestudies/APPNOTE.TXT Section 4.4.5
// This value denotes that the file is stored (no compression).
const uint32_t kCompressionMethodStored = 0;

// Offsets of fields in the Local Header.
const int kOffsetCompressionMethodInLocalHeader = 4 + 2 + 2;
const int kOffsetFilenameLengthInLocalHeader =
    kOffsetCompressionMethodInLocalHeader + 2 + 2 + 2 + 4 + 4 + 4;
const int kOffsetExtraFieldLengthInLocalHeader =
    kOffsetFilenameLengthInLocalHeader + 2;
const int kOffsetFilenameInLocalHeader =
    kOffsetExtraFieldLengthInLocalHeader + 2;

// RAII pattern for unmapping and closing the mapped file.
class ScopedMMap {
 public:
  ScopedMMap(void* mem, uint32_t len) : mem_(mem), len_(len) {}
  ~ScopedMMap() {
    if (munmap(mem_, len_) == -1) {
      PRINT("warning: munmap failed when trying to unmap zip file");
    }
  }
 private:
  void* mem_;
  uint32_t len_;
};

inline uint32_t ReadUInt16(uint8_t* mem_bytes, int offset) {
  return
      static_cast<uint32_t>(mem_bytes[offset]) |
      (static_cast<uint32_t>(mem_bytes[offset + 1]) << 8);
}

inline uint32_t ReadUInt32(uint8_t* mem_bytes, int offset) {
  return
      static_cast<uint32_t>(mem_bytes[offset]) |
      (static_cast<uint32_t>(mem_bytes[offset + 1]) << 8) |
      (static_cast<uint32_t>(mem_bytes[offset + 2]) << 16) |
      (static_cast<uint32_t>(mem_bytes[offset + 3]) << 24);
}

const uint32_t kMaxZipFileLength = 1U << 31;  // 2GB

}  // unnamed namespace

int FindStartOffsetOfFileInZipFile(int fd, const char* filename) {
  TRACE("Searching zip file in fd %d for filename %s", fd, filename);

  // Find the length of the file.
  const off64_t seek_end = lseek64(fd, 0, SEEK_END);

  if (seek_end > kMaxZipFileLength) {
    PRINT("warning: unable to map a zip file larger than %d bytes",
          kMaxZipFileLength);
    return ZIP_FIND_FAILED;
  }

  // Map the file into memory.
  const int bytes = seek_end;
  void* mem = mmap(NULL, bytes, PROT_READ, MAP_PRIVATE, fd, 0);
  if (mem == MAP_FAILED) {
    PRINT("warning: mmap failed mapping a zip file of %d bytes", bytes);
    return ZIP_FIND_FAILED;
  }
  ScopedMMap scoped_mmap(mem, bytes);

  // Scan backwards from the end of the file searching for the end of
  // central directory marker.
  uint8_t* mem_bytes = static_cast<uint8_t*>(mem);
  int scan;
  for (scan = bytes - sizeof(kEndOfCentralDirectoryMarker);
       scan >= 0; --scan) {
    if (ReadUInt32(mem_bytes, scan) == kEndOfCentralDirectoryMarker) {
      break;
    }
  }
  if (scan == -1) {
    TRACE("Failed to find end of zip file central directory");
    return ZIP_FIND_FAILED;
  }

  uint32_t off = static_cast<uint32_t>(scan);

  // We have located the end of central directory record, now locate
  // the central directory by reading the end of central directory record.

  const uint32_t length_of_central_dir = ReadUInt32(
      mem_bytes, off + kOffsetOfCentralDirLengthInEndOfCentralDirectory);
  const uint32_t start_of_central_dir = ReadUInt32(
      mem_bytes, off + kOffsetOfStartOfCentralDirInEndOfCentralDirectory);

  if (start_of_central_dir > off) {
    TRACE("Found out of range zip offset %u for start of directory",
          start_of_central_dir);
    return ZIP_FIND_FAILED;
  }

  const uint32_t end_of_central_dir =
      start_of_central_dir + length_of_central_dir;
  if (end_of_central_dir > off) {
    TRACE("Found out of range zip offset %u for end of directory",
          end_of_central_dir);
    return ZIP_FIND_FAILED;
  }

  const uint32_t num_entries = ReadUInt16(
      mem_bytes, off + kOffsetNumOfEntriesInEndOfCentralDirectory);

  // Read the headers in the central directory and locate the file.
  off = start_of_central_dir;
  const uint32_t target_len = strlen(filename);
  uint32_t n = 0;
  for (; n < num_entries && off < end_of_central_dir; ++n) {
    const uint32_t marker = ReadUInt32(mem_bytes, off);
    if (marker != kCentralDirHeaderMarker) {
      TRACE("Failed to find zip central directory header marker. "
            "Found 0x%x but expected 0x%x", marker, kCentralDirHeaderMarker);
      return ZIP_FIND_FAILED;
    }
    const uint32_t file_name_length =
        ReadUInt16(mem_bytes, off + kOffsetFilenameLengthInCentralDirectory);
    const uint32_t extra_field_length =
        ReadUInt16(mem_bytes, off + kOffsetExtraFieldLengthInCentralDirectory);
    const uint32_t comment_field_length =
        ReadUInt16(mem_bytes, off + kOffsetCommentLengthInCentralDirectory);
    const uint32_t header_length = kOffsetFilenameInCentralDirectory +
        file_name_length + extra_field_length + comment_field_length;

    const uint32_t local_header_offset =
        ReadUInt32(mem_bytes, off + kOffsetLocalHeaderOffsetInCentralDirectory);

    const uint8_t* filename_bytes =
        mem_bytes + off + kOffsetFilenameInCentralDirectory;

    if (file_name_length == target_len &&
        memcmp(filename_bytes, filename, target_len) == 0) {
      // Filename matches. Read the local header and compute the offset.
      const uint32_t marker = ReadUInt32(mem_bytes, local_header_offset);
      if (marker != kLocalHeaderMarker) {
        TRACE("Failed to find zip local file header marker. "
              "Found 0x%x but expected 0x%x", marker, kLocalHeaderMarker);
        return ZIP_FIND_FAILED;
      }

      const uint32_t compression_method =
          ReadUInt16(
              mem_bytes,
              local_header_offset + kOffsetCompressionMethodInLocalHeader);
      if (compression_method != kCompressionMethodStored) {
        PRINT("warning: %s is compressed within a zip file, skipped", filename);
        TRACE("%s is compressed. "
              "Found zip compression method %u but expected %u",
              filename, compression_method, kCompressionMethodStored);
        return ZIP_FIND_FAILED;
      }

      const uint32_t file_name_length =
          ReadUInt16(
              mem_bytes,
              local_header_offset + kOffsetFilenameLengthInLocalHeader);
      const uint32_t extra_field_length =
          ReadUInt16(
              mem_bytes,
              local_header_offset + kOffsetExtraFieldLengthInLocalHeader);
      const uint32_t header_length =
          kOffsetFilenameInLocalHeader + file_name_length + extra_field_length;

      TRACE("Found filename %s at offset %d in zip file fd %d",
            filename, local_header_offset + header_length, fd);
      return local_header_offset + header_length;
    }

    off += header_length;
  }

  if (n < num_entries) {
    TRACE("Did not find all the expected entries in the zip central directory. "
          "Found %d but expected %d", n, num_entries);
  }

  if (off < end_of_central_dir) {
    TRACE("There are %d extra bytes at the end of the zip central directory.",
          end_of_central_dir - off);
  }

  TRACE("Did not find %s in zip file size %d", filename, bytes);
  return ZIP_FIND_FAILED;
}
