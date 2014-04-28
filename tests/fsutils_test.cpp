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

/*
 * Contributed by: Intel Corporation
 */

/* ===
 *
 * Tests for filesystem utility-functions in Bionic libc.
 *
 * Functions categories:
 *   - Managing file/dir permissions
 *   - Making/removing directory
 *   - Doing same stuff under specifed base-directory
 *   - Making non-regular files like FIFO-pipes.
 *
 * Functions covered:
 *   chmod(2)    - change permissions
 *   fchmod(2)   - change permissions by FD
 *   mkdir(2)    - make directory
 *   stat(2)     - file/directory info
 *   fstat(2)    - file/directory info by FD
 *   lstat(2)    - stat() for symlinking status
 *   mknod(3)    - make non-regular file (ex. FIFO-pipe)
 *   umask(1)    - set permissions mask
 *   mkfifo(2)   - make FIFO-pipe (macros for mknod())
 *   getcwd(2)   - get working directory
 *   chdir(1)    - set working directory
 *   fstatat(4)  - stat() with base directory by FD
 *   mkdirat(3)  - mkdir() in directory by FD
 *   fchownat(5) - change file uid/gid in directory by FD
 *   fchmodat(4) - change file permissions in directory by FD
 *   renameat(4) - rename a file relative to directories by FD
 *   rename(2)   - rename a file (maybe move)
 *   remove(1)   - delete a file
 *   rmdir(1)    - delete a directory
 */

#include <gtest/gtest.h>
#include "CrashCatch.h"
#include "TemporaryFile.h"

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

using std::string;

static string NON_EXISTANT_FILE;

static string TEST_FILE;
static string TEST_FILE_DIR;
static string TEST_FILE_NAME;

static string TEST_DIRECTORY;
static string TEST_DIRECTORY_DIR;
static string TEST_DIRECTORY_NAME;

static string TEST_INVALID_DIR;

static string TEST_SYMLINK;
static string TEST_SYMLINK_DIR;
static string TEST_SYMLINK_NAME;

static string TEST_MKNOD_FILE;


#define MODE_XXX(st_mode) (st_mode & 0x01FF)

const static mode_t MODE_300 = 0       | S_IWUSR | S_IXUSR |
                               0       | 0       | 0       |
                               0       | 0       | 0       ;

const static mode_t MODE_444 = S_IRUSR | 0       | 0       |
                               S_IRGRP | 0       | 0       |
                               S_IROTH | 0       | 0       ;

const static mode_t MODE_644 = S_IRUSR | S_IWUSR | 0       |
                               S_IRGRP | 0       | 0       |
                               S_IROTH | 0       | 0       ;

const static mode_t MODE_755 = S_IRUSR | S_IWUSR | S_IXUSR |
                               S_IRGRP | 0       | S_IXGRP |
                               S_IROTH | 0       | S_IXOTH ;

const static mode_t MODE_777 = S_IRUSR | S_IWUSR | S_IXUSR |
                               S_IRGRP | S_IWGRP | S_IXGRP |
                               S_IROTH | S_IWOTH | S_IXOTH ;

class fsutils
    : public ::testing::Test
{
    TemporaryDir *td;
public:
    // Clean-up after every test
    virtual void TearDown()
    {
        remove(TEST_FILE.c_str());
        remove(TEST_SYMLINK.c_str());
        remove(TEST_MKNOD_FILE.c_str());
        rmdir(TEST_DIRECTORY.c_str());
        delete td;
    }

    virtual void SetUp()
    {
        td = new TemporaryDir();
        string BASE_DIR = td->dirname;

        NON_EXISTANT_FILE   = BASE_DIR + string("/probablyNonExistantFile.spam");
        TEST_FILE           = BASE_DIR + string("/usableFile.spam");
        TEST_FILE_DIR       = BASE_DIR + string("");
        TEST_FILE_NAME      = "usableFile.spam";

        TEST_DIRECTORY      = BASE_DIR + string("/bigBinOfSpam");
        TEST_DIRECTORY_DIR  = BASE_DIR + string("");
        TEST_DIRECTORY_NAME = "bigBinOfSpam";

        TEST_INVALID_DIR    = BASE_DIR + string("/someScaryDir/ThereIsNothingToBe");

        TEST_SYMLINK        = BASE_DIR + string("/usableSpamFile.symlink");
        TEST_SYMLINK_DIR    = BASE_DIR + string("");
        TEST_SYMLINK_NAME   = "usableSpamFile.symlink";

        TEST_MKNOD_FILE     = BASE_DIR + string("/spamBaseForMknod");
    }
};

TEST_F(fsutils, chmod)
{
    CRASH_CATCH();

    struct stat testStat;

    { SCOPED_TRACE("chmod() non-existant");
        EXPECT_FALSE( chmod(NON_EXISTANT_FILE.c_str(), MODE_777) == 0 );
    }

    { SCOPED_TRACE("create file");
        int testFd = open(TEST_FILE.c_str(), O_CREAT | O_WRONLY | O_TRUNC, MODE_777);
        ASSERT_TRUE(testFd != -1);
        close(testFd);
    }
    { SCOPED_TRACE("chmod() 0777");
        EXPECT_TRUE( chmod(TEST_FILE.c_str(), MODE_777) == 0 );
        EXPECT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_777, MODE_XXX(testStat.st_mode));
    }
    { SCOPED_TRACE("chmod() 0444");
        EXPECT_TRUE( chmod(TEST_FILE.c_str(), MODE_444) == 0 );
        EXPECT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_444, MODE_XXX(testStat.st_mode));
    }
    { SCOPED_TRACE("chmod() 0755");
        EXPECT_TRUE( chmod(TEST_FILE.c_str(), MODE_755) == 0 );
        EXPECT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_755, MODE_XXX(testStat.st_mode));
    }
}

TEST_F(fsutils, fchmod)
{
    CRASH_CATCH();

    int testFd;
    struct stat testStat;

    { SCOPED_TRACE("fchmod() invalid");
        EXPECT_FALSE(fchmod(-1, MODE_777) == 0);
    }

    { SCOPED_TRACE("create file");
        testFd = open(TEST_FILE.c_str(), O_CREAT | O_WRONLY | O_TRUNC, MODE_644);
        ASSERT_TRUE(testFd != -1);
    }
    { SCOPED_TRACE("fchmod() 0777");
        EXPECT_TRUE(fchmod(testFd, MODE_777) == 0);
        EXPECT_TRUE(stat(TEST_FILE.c_str(), &testStat) == 0);
        EXPECT_EQ(MODE_777, MODE_XXX(testStat.st_mode));
    }
    { SCOPED_TRACE("fchmod() 0444");
        EXPECT_TRUE(fchmod(testFd, MODE_444) == 0);
        EXPECT_TRUE(stat(TEST_FILE.c_str(), &testStat) == 0);
        EXPECT_EQ(MODE_444, MODE_XXX(testStat.st_mode));
    }
    { SCOPED_TRACE("fchmod() 0755");
        EXPECT_TRUE(fchmod(testFd, MODE_755) == 0);
        EXPECT_TRUE(stat(TEST_FILE.c_str(), &testStat) == 0);
        EXPECT_EQ(MODE_755, MODE_XXX(testStat.st_mode));
    }

    // Cleaning up
    close(testFd);
}

TEST_F(fsutils, mkdir)
{
    CRASH_CATCH();

    { SCOPED_TRACE("mkdir() create");
        EXPECT_TRUE(mkdir(TEST_DIRECTORY.c_str(), MODE_644) == 0);
    }
    { SCOPED_TRACE("mkdir() re-create");
        EXPECT_FALSE(mkdir(TEST_DIRECTORY.c_str(), MODE_777) == 0);
    }
    { SCOPED_TRACE("check permissions");
        struct stat testStat;
        EXPECT_TRUE(stat(TEST_DIRECTORY.c_str(), &testStat) == 0);
        EXPECT_EQ(MODE_644, MODE_XXX(testStat.st_mode));
    }
}

TEST_F(fsutils, mkdir_invalid)
{
    CRASH_CATCH();

    { SCOPED_TRACE("mkdir() create");
        EXPECT_TRUE(mkdir(TEST_DIRECTORY.c_str(), MODE_644) == 0);
    }
    { SCOPED_TRACE("mkdir() re-create");
        EXPECT_FALSE(mkdir(TEST_DIRECTORY.c_str(), MODE_777) == 0);
    }
    { SCOPED_TRACE("check permissions");
        struct stat testStat;
        EXPECT_TRUE(stat(TEST_DIRECTORY.c_str(), &testStat) == 0);
        EXPECT_EQ(MODE_644, MODE_XXX(testStat.st_mode));
    }
}

TEST_F(fsutils, stat)
{
    CRASH_CATCH();

    struct stat testStat, testStatSym;

    { SCOPED_TRACE("stat() non-existant");
        EXPECT_FALSE(stat(NON_EXISTANT_FILE.c_str(), &testStat) == 0);
    }

    { SCOPED_TRACE("create file");
        int testFd = open(TEST_FILE.c_str(), O_CREAT | O_WRONLY | O_TRUNC, MODE_644);
        ASSERT_TRUE(testFd != -1);
        close(testFd);
    }
    { SCOPED_TRACE("stat()");
        EXPECT_TRUE(stat(TEST_FILE.c_str(), &testStat) == 0);
    }

    { SCOPED_TRACE("create symlink");
        EXPECT_TRUE(symlink(TEST_FILE.c_str(), TEST_SYMLINK.c_str()) != -1);
    }
    { SCOPED_TRACE("stat() on symlink");
        EXPECT_TRUE(stat(TEST_SYMLINK.c_str(), &testStatSym) == 0);
        // Compare inode numbers
        EXPECT_EQ(testStat.st_ino, testStatSym.st_ino);
    }
}

TEST_F(fsutils, fstat)
{
    CRASH_CATCH();

    int testFd;
    struct stat testStat;

    { SCOPED_TRACE("fstat() invalid");
        EXPECT_FALSE(fstat(-1, &testStat) == 0);
    }

    { SCOPED_TRACE("create file");
        testFd = open(TEST_FILE.c_str(), O_CREAT | O_WRONLY | O_TRUNC, MODE_644);
        ASSERT_TRUE(testFd != -1);
    }
    { SCOPED_TRACE("stat()");
        EXPECT_TRUE(fstat(testFd, &testStat) == 0);
    }
}

TEST_F(fsutils, lstat)
{
    CRASH_CATCH();

    struct stat testStat, testStatSym;

    { SCOPED_TRACE("lstat() non-existant");
        EXPECT_FALSE(lstat(NON_EXISTANT_FILE.c_str(), &testStat) == 0);
    }

    { SCOPED_TRACE("create file");
        int testFd = open(TEST_FILE.c_str(), O_CREAT | O_WRONLY | O_TRUNC, MODE_644);
        ASSERT_TRUE(testFd != -1);
        close(testFd);
    }
    { SCOPED_TRACE("lstat() for generic file");
        EXPECT_TRUE(lstat(TEST_FILE.c_str(), &testStat) == 0);
    }

    { SCOPED_TRACE("create symlink");
        EXPECT_TRUE(symlink(TEST_FILE.c_str(), TEST_SYMLINK.c_str()) != -1);
    }
    { SCOPED_TRACE("lstat() on symlink");
        EXPECT_TRUE(lstat(TEST_SYMLINK.c_str(), &testStatSym) == 0);
        // Compare inode numbers (behavior differ from stat())
        EXPECT_NE(testStat.st_ino, testStatSym.st_ino);
    }
}

static void testMknod(mode_t mode)
{
    dev_t testDev = 0;
    struct stat testStat;

    EXPECT_TRUE(mknod(TEST_MKNOD_FILE.c_str(), mode, testDev) == 0);

    // Checking result
    EXPECT_TRUE(stat(TEST_MKNOD_FILE.c_str(), &testStat) == 0);
    EXPECT_EQ(mode, testStat.st_mode & S_IFMT);

    // Cleaning up
    remove(TEST_MKNOD_FILE.c_str());
}
TEST_F(fsutils, mknod)
{
    CRASH_CATCH();

    { SCOPED_TRACE("mknod() S_IFSOCK");
        testMknod(S_IFSOCK);
    }
    { SCOPED_TRACE("mknod() S_IFREG");
        testMknod(S_IFREG);
    }
    { SCOPED_TRACE("mknod() S_IFBLK");
        testMknod(S_IFBLK);
    }
    { SCOPED_TRACE("mknod() S_IFCHR");
        testMknod(S_IFCHR);
    }
    { SCOPED_TRACE("mknod() S_IFIFO");
        testMknod(S_IFIFO);
    }
}

TEST_F(fsutils, umask)
{
    CRASH_CATCH();

    mode_t oldPermissions;
    struct stat testStat;

    { SCOPED_TRACE("umask() 0755");
        oldPermissions = umask( S_IWGRP | S_IWOTH );
    }

    { SCOPED_TRACE("create file");
        int testFd = open(TEST_FILE.c_str(), O_CREAT | O_WRONLY | O_TRUNC, MODE_644);
        ASSERT_TRUE(testFd != -1);
        close(testFd);
    }
    { SCOPED_TRACE("check permissions");
        EXPECT_TRUE(stat(TEST_FILE.c_str(), &testStat) == 0);
        // Permissions should be changed
        EXPECT_NE(MODE_XXX(oldPermissions), MODE_XXX(testStat.st_mode));
        EXPECT_NE(S_IWGRP, MODE_XXX(testStat.st_mode) & S_IWGRP);
        EXPECT_NE(S_IWOTH, MODE_XXX(testStat.st_mode) & S_IWOTH);
    }

    // Cleaning up
    umask(0);  // Reset permissions masking
}

TEST_F(fsutils, mkfifo)
{
    CRASH_CATCH();

    { SCOPED_TRACE("mkfifo() 0644");
        ASSERT_TRUE(mkfifo(TEST_MKNOD_FILE.c_str(), MODE_644) == 0);
    }

    { SCOPED_TRACE("check permissions");
        struct stat testStat;
        EXPECT_TRUE(stat(TEST_MKNOD_FILE.c_str(), &testStat) == 0);
        EXPECT_EQ(MODE_644, MODE_XXX(testStat.st_mode));
    }

    { SCOPED_TRACE("check pipe");
        int fdRead, fdWrite;
        const char strSrc[45] = "Some test string to check the water pipes =P";
        char buffDst[ sizeof(strSrc) + 5 ];

        fdRead  = open(TEST_MKNOD_FILE.c_str(), O_RDONLY | O_NONBLOCK);
        ASSERT_TRUE( fdRead != -1 );
        fdWrite = open(TEST_MKNOD_FILE.c_str(), O_WRONLY | O_NONBLOCK);
        ASSERT_TRUE( fdWrite != -1 );

        write(fdWrite, strSrc, sizeof(strSrc));
        usleep(20000);  // Sleep 20 ms

        int nRead = read(fdRead, buffDst, sizeof(buffDst));
        EXPECT_EQ(sizeof(strSrc), nRead);

        close(fdRead);
        close(fdWrite);
    }
}

TEST_F(fsutils, getcwd_chdir)
{
    CRASH_CATCH();

    char oldCwd[256];
    char newCwd[256];
    int cwdSize = 255;

    { SCOPED_TRACE("getcwd()");
        ASSERT_TRUE( getcwd(oldCwd, cwdSize) == oldCwd );
    }

    { SCOPED_TRACE("chdir() and check");
        ASSERT_TRUE( chdir("/data/") == 0 );
        EXPECT_TRUE( getcwd(newCwd, cwdSize) == newCwd );
        EXPECT_TRUE( strncmp("/data", newCwd, cwdSize) == 0 );
    }

    { SCOPED_TRACE("chdir() invalid");
        ASSERT_FALSE( chdir(TEST_INVALID_DIR.c_str()) == 0 );
    }

    { SCOPED_TRACE("cwd restore and check");
        ASSERT_TRUE( chdir(oldCwd) == 0 );
        EXPECT_TRUE( getcwd(newCwd, cwdSize) == newCwd );
        EXPECT_TRUE( strncmp(oldCwd, newCwd, cwdSize) == 0 );
    }
}

TEST_F(fsutils, fstatat)
{
    CRASH_CATCH();

    int testFd;
    struct stat testStat;

    char oldCwd[256];
    { SCOPED_TRACE("save old cwd");
        ASSERT_TRUE( getcwd(oldCwd, 255) == oldCwd );
    }

    { SCOPED_TRACE("create file");
        int testFd = open(TEST_FILE.c_str(), O_CREAT | O_WRONLY | O_TRUNC, MODE_644);
        ASSERT_TRUE(testFd != -1);
        close(testFd);
    }

    { SCOPED_TRACE("fstatat() file absolute");
        EXPECT_TRUE( fstatat(AT_FDCWD, TEST_FILE.c_str(), &testStat, 0) == 0 );
        EXPECT_TRUE( (testStat.st_mode & S_IFMT) == S_IFREG );
    }

    { SCOPED_TRACE("fstatat() file relative");
        ASSERT_TRUE( chdir(TEST_FILE_DIR.c_str()) == 0 );
        EXPECT_TRUE( fstatat(AT_FDCWD, TEST_FILE_NAME.c_str(), &testStat, 0) == 0 );
        EXPECT_TRUE( (testStat.st_mode & S_IFMT) == S_IFREG );
        ASSERT_TRUE( chdir(oldCwd) == 0 );
    }

    { SCOPED_TRACE("get dir fd");
        testFd = open(TEST_FILE_DIR.c_str(), O_DIRECTORY);
        ASSERT_FALSE( testFd == -1 );
    }

    { SCOPED_TRACE("fstatat() file absolute (by dirfd)");
        EXPECT_TRUE( fstatat(testFd, TEST_FILE.c_str(), &testStat, 0) == 0 );
        EXPECT_TRUE( (testStat.st_mode & S_IFMT) == S_IFREG );
    }

    { SCOPED_TRACE("fstatat() file relative (by dirfd)");
        EXPECT_TRUE( fstatat(testFd, TEST_FILE_NAME.c_str(), &testStat, 0) == 0 );
        EXPECT_TRUE( (testStat.st_mode & S_IFMT) == S_IFREG );
    }

    { SCOPED_TRACE("symlinking");
        ASSERT_TRUE( symlink(TEST_FILE.c_str(), TEST_SYMLINK.c_str()) != -1 );
    }

    { SCOPED_TRACE("fstatat() sym absolute");
        EXPECT_TRUE( fstatat(AT_FDCWD, TEST_SYMLINK.c_str(), &testStat, 0) == 0 );
        EXPECT_TRUE( (testStat.st_mode & S_IFMT) == S_IFREG );
    }

    { SCOPED_TRACE("fstatat() sym relative");
        ASSERT_TRUE( chdir(TEST_SYMLINK_DIR.c_str()) == 0 );
        EXPECT_TRUE( fstatat(AT_FDCWD, TEST_SYMLINK_NAME.c_str(), &testStat, 0) == 0 );
        EXPECT_TRUE( (testStat.st_mode & S_IFMT) == S_IFREG );
        ASSERT_TRUE( chdir(oldCwd) == 0 );
    }

    { SCOPED_TRACE("get dir fd");
        close(testFd);
        testFd = open(TEST_SYMLINK_DIR.c_str(), O_DIRECTORY);
        ASSERT_FALSE( testFd == -1 );
    }

    { SCOPED_TRACE("fstatat() sym absolute (by dirfd)");
        EXPECT_TRUE( fstatat(testFd, TEST_SYMLINK.c_str(), &testStat, 0) == 0 );
        EXPECT_TRUE( (testStat.st_mode & S_IFMT) == S_IFREG );
    }

    { SCOPED_TRACE("fstatat() sym relative (by dirfd)");
        EXPECT_TRUE( fstatat(testFd, TEST_SYMLINK_NAME.c_str(), &testStat, 0) == 0 );
        EXPECT_TRUE( (testStat.st_mode & S_IFMT) == S_IFREG );
    }

    // Cleaning up
    close(testFd);
}

TEST_F(fsutils, mkdirat)
{
    CRASH_CATCH();

    int testFd;
    struct stat testStat;

    char oldCwd[256];
    { SCOPED_TRACE("save old cwd");
        ASSERT_TRUE( getcwd(oldCwd, 255) == oldCwd );
    }

    { SCOPED_TRACE("mkdirat() file absolute");
        ASSERT_TRUE( mkdirat(AT_FDCWD, TEST_DIRECTORY.c_str(), MODE_777) == 0 );
        EXPECT_TRUE( stat(TEST_DIRECTORY.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_777, MODE_XXX(testStat.st_mode));
        rmdir(TEST_DIRECTORY.c_str());
    }

    { SCOPED_TRACE("mkdirat() file relative");
        ASSERT_TRUE( chdir(TEST_DIRECTORY_DIR.c_str()) == 0 );
        EXPECT_TRUE( mkdirat(AT_FDCWD, TEST_DIRECTORY_NAME.c_str(), MODE_777) == 0 );
        EXPECT_TRUE( stat(TEST_DIRECTORY.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_777, MODE_XXX(testStat.st_mode));
        rmdir(TEST_DIRECTORY.c_str());
        ASSERT_TRUE( chdir(oldCwd) == 0 );
    }

    { SCOPED_TRACE("get dir fd");
        testFd = open(TEST_DIRECTORY_DIR.c_str(), O_DIRECTORY);
        ASSERT_FALSE( testFd == -1 );
    }

    { SCOPED_TRACE("mkdirat() file absolute (by dirfd)");
        ASSERT_TRUE( mkdirat(testFd, TEST_DIRECTORY.c_str(), MODE_777) == 0 );
        EXPECT_TRUE( stat(TEST_DIRECTORY.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_777, MODE_XXX(testStat.st_mode));
        rmdir(TEST_DIRECTORY.c_str());
    }

    { SCOPED_TRACE("mkdirat() file relative (by dirfd)");
        EXPECT_TRUE( mkdirat(testFd, TEST_DIRECTORY_NAME.c_str(), MODE_777) == 0 );
        EXPECT_TRUE( stat(TEST_DIRECTORY.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_777, MODE_XXX(testStat.st_mode));
        rmdir(TEST_DIRECTORY.c_str());
    }

    // Cleaning up
    close(testFd);
}

TEST_F(fsutils, fchownat)
{
    CRASH_CATCH();

    int testFd;
    struct stat testStat;

    unsigned long oldOwner;
    unsigned long oldGroup;
    unsigned long testOwner = 10;
    unsigned long testGroup = 10;

    char oldCwd[256];
    { SCOPED_TRACE("save old cwd");
        ASSERT_TRUE( getcwd(oldCwd, 255) == oldCwd );
    }

    { SCOPED_TRACE("create file");
        testFd = open(TEST_FILE.c_str(), O_CREAT | O_WRONLY | O_TRUNC, MODE_644);
        ASSERT_TRUE( testFd != -1 );
        EXPECT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        oldOwner = testStat.st_uid;
        oldGroup = testStat.st_gid;
    }

    { SCOPED_TRACE("fchownat() file absolute");
        ++testOwner;
        ++testGroup;
        ASSERT_TRUE( fchownat(AT_FDCWD, TEST_FILE.c_str(), testOwner, testGroup, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(testOwner, testStat.st_uid);
        EXPECT_EQ(testGroup, testStat.st_gid);
    }

    { SCOPED_TRACE("fchownat() file relative");
        ++testOwner;
        ++testGroup;
        ASSERT_TRUE( chdir(TEST_DIRECTORY_DIR.c_str()) == 0 );
        ASSERT_TRUE( fchownat(AT_FDCWD, TEST_FILE_NAME.c_str(), testOwner, testGroup, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(testOwner, testStat.st_uid);
        EXPECT_EQ(testGroup, testStat.st_gid);
        ASSERT_TRUE( chdir(oldCwd) == 0 );
    }

    { SCOPED_TRACE("get dir fd");
        testFd = open(TEST_FILE_DIR.c_str(), O_DIRECTORY);
        ASSERT_FALSE( testFd == -1 );
    }

    { SCOPED_TRACE("fchownat() file absolute (by dirfd)");
        ++testOwner;
        ++testGroup;
        ASSERT_TRUE( fchownat(testFd, TEST_FILE.c_str(), testOwner, testGroup, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(testOwner, testStat.st_uid);
        EXPECT_EQ(testGroup, testStat.st_gid);
    }

    { SCOPED_TRACE("fchownat() file relative (by dirfd)");
        ++testOwner;
        ++testGroup;
        ASSERT_TRUE( fchownat(testFd, TEST_FILE_NAME.c_str(), testOwner, testGroup, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(testOwner, testStat.st_uid);
       EXPECT_EQ(testGroup, testStat.st_gid);
    }

    { SCOPED_TRACE("fchownat() restore");
        ASSERT_TRUE( fchownat(testFd, TEST_FILE_NAME.c_str(), oldOwner, oldGroup, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(oldOwner, testStat.st_uid);
        EXPECT_EQ(oldGroup, testStat.st_gid);
    }

    { SCOPED_TRACE("symlinking");
        ASSERT_TRUE( symlink(TEST_FILE.c_str(), TEST_SYMLINK.c_str()) != -1 );
    }

    { SCOPED_TRACE("fchownat() sym absolute");
        ++testOwner;
        ++testGroup;
        ASSERT_TRUE( fchownat(AT_FDCWD, TEST_SYMLINK.c_str(), testOwner, testGroup, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(testOwner, testStat.st_uid);
        EXPECT_EQ(testGroup, testStat.st_gid);
    }

    { SCOPED_TRACE("fchownat() sym relative");
        ++testOwner;
        ++testGroup;
        ASSERT_TRUE( chdir(TEST_DIRECTORY_DIR.c_str()) == 0 );
        ASSERT_TRUE( fchownat(AT_FDCWD, TEST_SYMLINK_NAME.c_str(), testOwner, testGroup, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(testOwner, testStat.st_uid);
        EXPECT_EQ(testGroup, testStat.st_gid);
        ASSERT_TRUE( chdir(oldCwd) == 0 );
    }

    { SCOPED_TRACE("get dir fd");
        close(testFd);
        testFd = open(TEST_SYMLINK_DIR.c_str(), O_DIRECTORY);
        ASSERT_FALSE( testFd == -1 );
    }

    { SCOPED_TRACE("fchownat() sym absolute (by dirfd)");
        ++testOwner;
        ++testGroup;
        ASSERT_TRUE( fchownat(testFd, TEST_SYMLINK.c_str(), testOwner, testGroup, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(testOwner, testStat.st_uid);
        EXPECT_EQ(testGroup, testStat.st_gid);
    }

    { SCOPED_TRACE("fchownat() sym relative (by dirfd)");
        ++testOwner;
        ++testGroup;
        ASSERT_TRUE( fchownat(testFd, TEST_SYMLINK_NAME.c_str(), testOwner, testGroup, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(testOwner, testStat.st_uid);
        EXPECT_EQ(testGroup, testStat.st_gid);
    }

    // Cleaning up
    close(testFd);
}

TEST_F(fsutils, fchmodat)
{
    CRASH_CATCH();

    int testFd;
    struct stat testStat;

    char oldCwd[256];
    { SCOPED_TRACE("save old cwd");
        ASSERT_TRUE( getcwd(oldCwd, 255) == oldCwd );
    }

    { SCOPED_TRACE("create file");
        testFd = open(TEST_FILE.c_str(), O_CREAT | O_WRONLY | O_TRUNC, MODE_644);
        ASSERT_TRUE( testFd != -1 );
        ASSERT_TRUE( chmod(TEST_FILE.c_str(), MODE_777) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        ASSERT_EQ(MODE_777, MODE_XXX(testStat.st_mode));
    }

    { SCOPED_TRACE("fchmodat() file absolute");
        ASSERT_TRUE( fchmodat(AT_FDCWD, TEST_FILE.c_str(), MODE_644, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_644, MODE_XXX(testStat.st_mode));
        ASSERT_TRUE( chmod(TEST_FILE.c_str(), MODE_777) == 0 );
    }

    { SCOPED_TRACE("fchmodat() file relative");
        ASSERT_TRUE( chdir(TEST_DIRECTORY_DIR.c_str()) == 0 );
        ASSERT_TRUE( fchmodat(AT_FDCWD, TEST_FILE_NAME.c_str(), MODE_644, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_644, MODE_XXX(testStat.st_mode));
        ASSERT_TRUE( chmod(TEST_FILE.c_str(), MODE_777) == 0 );
        ASSERT_TRUE( chdir(oldCwd) == 0 );
    }

    { SCOPED_TRACE("get dir fd");
        testFd = open(TEST_FILE_DIR.c_str(), O_DIRECTORY);
        ASSERT_FALSE( testFd == -1 );
    }

    { SCOPED_TRACE("fchmodat() file absolute (by dirfd)");
        ASSERT_TRUE( fchmodat(testFd, TEST_FILE.c_str(), MODE_644, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_644, MODE_XXX(testStat.st_mode));
        ASSERT_TRUE( chmod(TEST_FILE.c_str(), MODE_777) == 0 );
    }

    { SCOPED_TRACE("fchmodat() file relative (by dirfd)");
        ASSERT_TRUE( fchmodat(testFd, TEST_FILE_NAME.c_str(), MODE_644, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_644, MODE_XXX(testStat.st_mode));
        ASSERT_TRUE( chmod(TEST_FILE.c_str(), MODE_777) == 0 );
    }

    { SCOPED_TRACE("symlinking");
        ASSERT_TRUE( symlink(TEST_FILE.c_str(), TEST_SYMLINK.c_str()) != -1 );
    }

    { SCOPED_TRACE("fchmodat() sym absolute");
        ASSERT_TRUE( fchmodat(AT_FDCWD, TEST_SYMLINK.c_str(), MODE_644, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_644, MODE_XXX(testStat.st_mode));
        ASSERT_TRUE( chmod(TEST_FILE.c_str(), MODE_777) == 0 );
    }

    { SCOPED_TRACE("fchmodat() sym relative");
        ASSERT_TRUE( chdir(TEST_DIRECTORY_DIR.c_str()) == 0 );
        ASSERT_TRUE( fchmodat(AT_FDCWD, TEST_SYMLINK_NAME.c_str(), MODE_644, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_644, MODE_XXX(testStat.st_mode));
        ASSERT_TRUE( chmod(TEST_FILE.c_str(), MODE_777) == 0 );
        ASSERT_TRUE( chdir(oldCwd) == 0 );
    }

    { SCOPED_TRACE("get dir fd");
        close(testFd);
        testFd = open(TEST_FILE_DIR.c_str(), O_DIRECTORY);
        ASSERT_FALSE( testFd == -1 );
    }

    { SCOPED_TRACE("fchmodat() sym absolute (by dirfd)");
        ASSERT_TRUE( fchmodat(testFd, TEST_SYMLINK.c_str(), MODE_644, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_644, MODE_XXX(testStat.st_mode));
        ASSERT_TRUE( chmod(TEST_FILE.c_str(), MODE_777) == 0 );
    }

    { SCOPED_TRACE("fchmodat() sym relative (by dirfd)");
        ASSERT_TRUE( fchmodat(testFd, TEST_SYMLINK_NAME.c_str(), MODE_644, 0) == 0 );
        ASSERT_TRUE( stat(TEST_FILE.c_str(), &testStat) == 0 );
        EXPECT_EQ(MODE_644, MODE_XXX(testStat.st_mode));
        ASSERT_TRUE( chmod(TEST_FILE.c_str(), MODE_777) == 0 );
    }

    // Cleaning up
    close(testFd);
}

TEST_F(fsutils, renameat)
{
    CRASH_CATCH();

    int rootFd;
    int procFd;
    int testFd;

    { SCOPED_TRACE("prepare files");
        ASSERT_TRUE( creat(TEST_FILE.c_str(), MODE_755) != -1 );
        ASSERT_TRUE( symlink(TEST_FILE.c_str(), TEST_SYMLINK.c_str()) != -1 );
        ASSERT_TRUE( access(TEST_FILE.c_str(), F_OK) == 0 );
        ASSERT_TRUE( access(TEST_SYMLINK.c_str(), F_OK) == 0 );
    }

    { SCOPED_TRACE("get dir fd");
        rootFd = open("/", O_DIRECTORY);
        ASSERT_FALSE( rootFd == -1 );
        procFd = open("/proc", O_DIRECTORY);
        ASSERT_FALSE( procFd == -1 );
        testFd = open(TEST_FILE_DIR.c_str(), O_DIRECTORY);
        ASSERT_FALSE( testFd == -1 );
    }

    { SCOPED_TRACE("renameat() not-allowed");
        ASSERT_FALSE( renameat(rootFd, TEST_FILE.c_str(), rootFd, "/proc/abc") == 0 );
        ASSERT_FALSE( renameat(rootFd, TEST_FILE.c_str(), procFd, "abc") == 0 );
    }

    { SCOPED_TRACE("renameat()");
        ASSERT_TRUE( renameat(testFd, TEST_FILE_NAME.c_str(), rootFd, NON_EXISTANT_FILE.c_str()) == 0 );
        EXPECT_FALSE( access(TEST_FILE.c_str(), F_OK) == 0 );
        EXPECT_TRUE( access(NON_EXISTANT_FILE.c_str(), F_OK) == 0 );
        EXPECT_FALSE( access(TEST_SYMLINK.c_str(), F_OK) == 0 );  // It's soft link
        remove(TEST_SYMLINK.c_str());
        remove(NON_EXISTANT_FILE.c_str());
    }

    { SCOPED_TRACE("renameat() invalid");
        ASSERT_FALSE( renameat(rootFd, NON_EXISTANT_FILE.c_str(), testFd, TEST_FILE_NAME.c_str()) == 0 );
    }

    // Cleaning up
    close(rootFd);
    close(procFd);
    close(testFd);
}

TEST_F(fsutils, rename)
{
    CRASH_CATCH();

    { SCOPED_TRACE("prepare files");
        ASSERT_TRUE( creat(TEST_FILE.c_str(), MODE_755) != -1 );
        ASSERT_TRUE( symlink(TEST_FILE.c_str(), TEST_SYMLINK.c_str()) != -1 );
        ASSERT_TRUE( access(TEST_FILE.c_str(), F_OK) == 0 );
        ASSERT_TRUE( access(TEST_SYMLINK.c_str(), F_OK) == 0 );
    }

    { SCOPED_TRACE("rename() not-allowed");
        ASSERT_FALSE( rename(TEST_FILE.c_str(), "/proc/abc") == 0 );
    }

    { SCOPED_TRACE("rename()");
        ASSERT_TRUE( rename(TEST_FILE.c_str(), NON_EXISTANT_FILE.c_str()) == 0 );
        EXPECT_FALSE( access(TEST_FILE.c_str(), F_OK) == 0 );
        EXPECT_TRUE( access(NON_EXISTANT_FILE.c_str(), F_OK) == 0 );
        EXPECT_FALSE( access(TEST_SYMLINK.c_str(), F_OK) == 0 );  // It's soft link
        remove(TEST_SYMLINK.c_str());
        remove(NON_EXISTANT_FILE.c_str());
    }

    { SCOPED_TRACE("rename() invalid");
        ASSERT_FALSE( rename(NON_EXISTANT_FILE.c_str(), TEST_FILE.c_str()) == 0 );
    }
}

TEST_F(fsutils, remove)
{
    CRASH_CATCH();

    { SCOPED_TRACE("prepare files");
        ASSERT_TRUE( creat(TEST_FILE.c_str(), MODE_755) != -1 );
        ASSERT_TRUE( symlink(TEST_FILE.c_str(), TEST_SYMLINK.c_str()) != -1 );
        ASSERT_TRUE( access(TEST_FILE.c_str(), F_OK) == 0 );
        ASSERT_TRUE( access(TEST_SYMLINK.c_str(), F_OK) == 0 );
    }

    { SCOPED_TRACE("remove()");
        EXPECT_TRUE( remove(TEST_SYMLINK.c_str()) == 0 );
        EXPECT_TRUE( remove(TEST_FILE.c_str()) == 0 );
        EXPECT_FALSE( access(TEST_FILE.c_str(), F_OK) == 0 );
        EXPECT_FALSE( access(TEST_SYMLINK.c_str(), F_OK) == 0 );
    }

    { SCOPED_TRACE("remove() invalid");
        EXPECT_FALSE( remove(TEST_SYMLINK.c_str()) == 0 );
        EXPECT_FALSE( remove(TEST_FILE.c_str()) == 0 );
    }
}

TEST_F(fsutils, rmdir)
{
    CRASH_CATCH();

    struct stat testStat;

    { SCOPED_TRACE("prepare dir");
        ASSERT_TRUE( mkdir(TEST_DIRECTORY.c_str(), MODE_755) != -1 );
        ASSERT_TRUE( stat(TEST_DIRECTORY.c_str(), &testStat) == 0 );
    }

    { SCOPED_TRACE("rmdir()");
        EXPECT_TRUE( rmdir(TEST_DIRECTORY.c_str()) == 0 );
        EXPECT_FALSE( stat(TEST_DIRECTORY.c_str(), &testStat) == 0 );
    }

    { SCOPED_TRACE("rmdir() invalid");
        EXPECT_FALSE( rmdir(TEST_DIRECTORY.c_str()) == 0 );
    }
}
