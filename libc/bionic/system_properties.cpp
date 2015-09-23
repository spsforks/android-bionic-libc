/*
 * Copyright (C) 2008 The Android Open Source Project
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
#include <new>
#include <stdatomic.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#include <sys/mman.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <netinet/in.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/system_properties.h>

#include "private/bionic_futex.h"
#include "private/bionic_macros.h"

static const char property_service_socket[] = "/dev/socket/" PROP_SERVICE_NAME;


/*
 * Properties are stored in a hybrid trie/binary tree structure.
 * Each property's name is delimited at '.' characters, and the tokens are put
 * into a trie structure.  Siblings at each level of the trie are stored in a
 * binary tree.  For instance, "ro.secure"="1" could be stored as follows:
 *
 * +-----+   children    +----+   children    +--------+
 * |     |-------------->| ro |-------------->| secure |
 * +-----+               +----+               +--------+
 *                       /    \                /   |
 *                 left /      \ right   left /    |  prop   +===========+
 *                     v        v            v     +-------->| ro.secure |
 *                  +-----+   +-----+     +-----+            +-----------+
 *                  | net |   | sys |     | com |            |     1     |
 *                  +-----+   +-----+     +-----+            +===========+
 */

// Represents a node in the trie.
struct prop_bt {
    uint8_t namelen;
    uint8_t reserved[3];

    // The property trie is updated only by the init process (single threaded) which provides
    // property service. And it can be read by multiple threads at the same time.
    // As the property trie is not protected by locks, we use atomic_uint_least32_t types for the
    // left, right, children "pointers" in the trie node. To make sure readers who see the
    // change of "pointers" can also notice the change of prop_bt structure contents pointed by
    // the "pointers", we always use release-consume ordering pair when accessing these "pointers".

    // prop "points" to prop_info structure if there is a propery associated with the trie node.
    // Its situation is similar to the left, right, children "pointers". So we use
    // atomic_uint_least32_t and release-consume ordering to protect it as well.

    // We should also avoid rereading these fields redundantly, since not
    // all processor implementations ensure that multiple loads from the
    // same field are carried out in the right order.
    atomic_uint_least32_t prop;

    atomic_uint_least32_t left;
    atomic_uint_least32_t right;

    atomic_uint_least32_t children;

    char name[0];

    prop_bt(const char *name, const uint8_t name_length) {
        this->namelen = name_length;
        memcpy(this->name, name, name_length);
        this->name[name_length] = '\0';
    }

private:
    DISALLOW_COPY_AND_ASSIGN(prop_bt);
};

class prop_area {
public:

    prop_area(const uint32_t magic, const uint32_t version) :
        magic_(magic), version_(version) {
        atomic_init(&serial_, 0);
        memset(reserved_, 0, sizeof(reserved_));
        // Allocate enough space for the root node.
        bytes_used_ = sizeof(prop_bt);
    }

    const prop_info *find(const char *name);
    bool add(const char *name, unsigned int namelen,
             const char *value, unsigned int valuelen);

    bool foreach(void (*propfn)(const prop_info *pi, void *cookie), void *cookie);

    atomic_uint_least32_t *serial() { return &serial_; }
    uint32_t magic() const { return magic_; }
    uint32_t version() const { return version_; }

private:
    void *allocate_obj(const size_t size, uint_least32_t *const off);
    prop_bt *new_prop_bt(const char *name, uint8_t namelen, uint_least32_t *const off);
    prop_info *new_prop_info(const char *name, uint8_t namelen,
                             const char *value, uint8_t valuelen,
                             uint_least32_t *const off);
    void *to_prop_obj(uint_least32_t off);
    prop_bt *to_prop_bt(atomic_uint_least32_t *off_p);
    prop_info *to_prop_info(atomic_uint_least32_t *off_p);

    prop_bt *root_node();

    prop_bt *find_prop_bt(prop_bt *const bt, const char *name,
                          uint8_t namelen, bool alloc_if_needed);

    const prop_info *find_property(prop_bt *const trie, const char *name,
                                   uint8_t namelen, const char *value,
                                   uint8_t valuelen, bool alloc_if_needed);

    bool foreach_property(prop_bt *const trie,
                          void (*propfn)(const prop_info *pi, void *cookie),
                          void *cookie);

    uint32_t bytes_used_;
    atomic_uint_least32_t serial_;
    uint32_t magic_;
    uint32_t version_;
    uint32_t reserved_[28];
    char data_[0];

    DISALLOW_COPY_AND_ASSIGN(prop_area);
};

struct prop_info {
    atomic_uint_least32_t serial;
    char value[PROP_VALUE_MAX];
    char name[0];

    prop_info(const char *name, const uint8_t namelen, const char *value,
              const uint8_t valuelen) {
        memcpy(this->name, name, namelen);
        this->name[namelen] = '\0';
        atomic_init(&this->serial, valuelen << 24);
        memcpy(this->value, value, valuelen);
        this->value[valuelen] = '\0';
    }
private:
    DISALLOW_COPY_AND_ASSIGN(prop_info);
};

struct find_nth_cookie {
    uint32_t count;
    const uint32_t n;
    const prop_info *pi;

    find_nth_cookie(uint32_t n) : count(0), n(n), pi(NULL) {
    }
};

static char property_filename[PATH_MAX] = PROP_FILENAME;
static char secure_property_filename[PATH_MAX] = PROP_FILENAME"secure__";
static bool compat_mode = false;
static size_t pa_data_size;
static size_t pa_size;

// NOTE: This isn't static because system_properties_compat.c
// requires it.
prop_area *__system_property_area__ = NULL;

static int get_fd_from_env(void)
{
    // This environment variable consistes of two decimal integer
    // values separated by a ",". The first value is a file descriptor
    // and the second is the size of the system properties area. The
    // size is currently unused.
    char *env = getenv("ANDROID_PROPERTY_WORKSPACE");

    if (!env) {
        return -1;
    }

    return atoi(env);
}

static prop_area* map_prop_area_rw(const char* filename, const char* context)
{
    /* dev is a tmpfs that we can use to carve a shared workspace
     * out of, so let's do that...
     */
    const int fd = open(filename,
                        O_RDWR | O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_EXCL, 0444);

    if (fd < 0) {
        if (errno == EACCES) {
            /* for consistency with the case where the process has already
             * mapped the page in and segfaults when trying to write to it
             */
            abort();
        }
        return nullptr;
    }

    if (context) {
        // TODO: error handling
        fsetxattr(fd, "security.selinux" /* XATTR_NAME_SELINUX */, context, strlen(context) + 1, 0);
    }

    if (ftruncate(fd, PA_SIZE) < 0) {
        close(fd);
        return nullptr;
    }

    pa_size = PA_SIZE;
    pa_data_size = pa_size - sizeof(prop_area);
    compat_mode = false;

    void *const memory_area = mmap(NULL, pa_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memory_area == MAP_FAILED) {
        close(fd);
        return nullptr;
    }

    prop_area *pa = new(memory_area) prop_area(PROP_AREA_MAGIC, PROP_AREA_VERSION);

    close(fd);
    return pa;
}

static prop_area *map_fd_ro(const int fd) {
    struct stat fd_stat;
    if (fstat(fd, &fd_stat) < 0) {
        return nullptr;
    }

    if ((fd_stat.st_uid != 0)
            || (fd_stat.st_gid != 0)
            || ((fd_stat.st_mode & (S_IWGRP | S_IWOTH)) != 0)
            || (fd_stat.st_size < static_cast<off_t>(sizeof(prop_area))) ) {
        return nullptr;
    }

    pa_size = fd_stat.st_size;
    pa_data_size = pa_size - sizeof(prop_area);

    void* const map_result = mmap(NULL, pa_size, PROT_READ, MAP_SHARED, fd, 0);
    if (map_result == MAP_FAILED) {
        return nullptr;
    }

    prop_area* pa = reinterpret_cast<prop_area*>(map_result);
    if ((pa->magic() != PROP_AREA_MAGIC) ||
        (pa->version() != PROP_AREA_VERSION &&
         pa->version() != PROP_AREA_VERSION_COMPAT)) {
        munmap(pa, pa_size);
        return nullptr;
    }

    if (pa->version() == PROP_AREA_VERSION_COMPAT) {
        compat_mode = true;
    }

    return pa;
}

static prop_area* map_prop_area(const char* filename, bool is_default)
{
    int fd = open(filename, O_CLOEXEC | O_NOFOLLOW | O_RDONLY);
    bool close_fd = true;
    if (fd == -1 && errno == ENOENT && is_default) {
        /*
         * For backwards compatibility, if the file doesn't
         * exist, we use the environment to get the file descriptor.
         * For security reasons, we only use this backup if the kernel
         * returns ENOENT. We don't want to use the backup if the kernel
         * returns other errors such as ENOMEM or ENFILE, since it
         * might be possible for an external program to trigger this
         * condition.
         * Only do this for the default prop file, secured prop files
         * do not have a backup
         */
        fd = get_fd_from_env();
        close_fd = false;
    }

    if (fd < 0) {
        return nullptr;
    }

    prop_area *map_result = map_fd_ro(fd);
    if (close_fd) {
        close(fd);
    }

    return map_result;
}

void *prop_area::allocate_obj(const size_t size, uint_least32_t *const off)
{
    const size_t aligned = BIONIC_ALIGN(size, sizeof(uint_least32_t));
    if (bytes_used_ + aligned > pa_data_size) {
        return NULL;
    }

    *off = bytes_used_;
    bytes_used_ += aligned;
    return data_ + *off;
}

prop_bt *prop_area::new_prop_bt(const char *name, uint8_t namelen, uint_least32_t *const off)
{
    uint_least32_t new_offset;
    void *const p = allocate_obj(sizeof(prop_bt) + namelen + 1, &new_offset);
    if (p != NULL) {
        prop_bt* bt = new(p) prop_bt(name, namelen);
        *off = new_offset;
        return bt;
    }

    return NULL;
}

prop_info *prop_area::new_prop_info(const char *name, uint8_t namelen,
        const char *value, uint8_t valuelen, uint_least32_t *const off)
{
    uint_least32_t new_offset;
    void* const p = allocate_obj(sizeof(prop_info) + namelen + 1, &new_offset);
    if (p != NULL) {
        prop_info* info = new(p) prop_info(name, namelen, value, valuelen);
        *off = new_offset;
        return info;
    }

    return NULL;
}

void *prop_area::to_prop_obj(uint_least32_t off)
{
    if (off > pa_data_size)
        return NULL;

    return (data_ + off);
}

inline prop_bt *prop_area::to_prop_bt(atomic_uint_least32_t* off_p) {
  uint_least32_t off = atomic_load_explicit(off_p, memory_order_consume);
  return reinterpret_cast<prop_bt*>(to_prop_obj(off));
}

inline prop_info *prop_area::to_prop_info(atomic_uint_least32_t* off_p) {
  uint_least32_t off = atomic_load_explicit(off_p, memory_order_consume);
  return reinterpret_cast<prop_info*>(to_prop_obj(off));
}

inline prop_bt *prop_area::root_node()
{
    return reinterpret_cast<prop_bt*>(to_prop_obj(0));
}

static int cmp_prop_name(const char *one, uint8_t one_len, const char *two,
        uint8_t two_len)
{
    if (one_len < two_len)
        return -1;
    else if (one_len > two_len)
        return 1;
    else
        return strncmp(one, two, one_len);
}

prop_bt *prop_area::find_prop_bt(prop_bt *const bt, const char *name,
                                 uint8_t namelen, bool alloc_if_needed)
{

    prop_bt* current = bt;
    while (true) {
        if (!current) {
            return NULL;
        }

        const int ret = cmp_prop_name(name, namelen, current->name, current->namelen);
        if (ret == 0) {
            return current;
        }

        if (ret < 0) {
            uint_least32_t left_offset = atomic_load_explicit(&current->left, memory_order_relaxed);
            if (left_offset != 0) {
                current = to_prop_bt(&current->left);
            } else {
                if (!alloc_if_needed) {
                   return NULL;
                }

                uint_least32_t new_offset;
                prop_bt* new_bt = new_prop_bt(name, namelen, &new_offset);
                if (new_bt) {
                    atomic_store_explicit(&current->left, new_offset, memory_order_release);
                }
                return new_bt;
            }
        } else {
            uint_least32_t right_offset = atomic_load_explicit(&current->right, memory_order_relaxed);
            if (right_offset != 0) {
                current = to_prop_bt(&current->right);
            } else {
                if (!alloc_if_needed) {
                   return NULL;
                }

                uint_least32_t new_offset;
                prop_bt* new_bt = new_prop_bt(name, namelen, &new_offset);
                if (new_bt) {
                    atomic_store_explicit(&current->right, new_offset, memory_order_release);
                }
                return new_bt;
            }
        }
    }
}

const prop_info *prop_area::find_property(prop_bt *const trie, const char *name,
        uint8_t namelen, const char *value, uint8_t valuelen,
        bool alloc_if_needed)
{
    if (!trie) return NULL;

    const char *remaining_name = name;
    prop_bt* current = trie;
    while (true) {
        const char *sep = strchr(remaining_name, '.');
        const bool want_subtree = (sep != NULL);
        const uint8_t substr_size = (want_subtree) ?
            sep - remaining_name : strlen(remaining_name);

        if (!substr_size) {
            return NULL;
        }

        prop_bt* root = NULL;
        uint_least32_t children_offset = atomic_load_explicit(&current->children, memory_order_relaxed);
        if (children_offset != 0) {
            root = to_prop_bt(&current->children);
        } else if (alloc_if_needed) {
            uint_least32_t new_offset;
            root = new_prop_bt(remaining_name, substr_size, &new_offset);
            if (root) {
                atomic_store_explicit(&current->children, new_offset, memory_order_release);
            }
        }

        if (!root) {
            return NULL;
        }

        current = find_prop_bt(root, remaining_name, substr_size, alloc_if_needed);
        if (!current) {
            return NULL;
        }

        if (!want_subtree)
            break;

        remaining_name = sep + 1;
    }

    uint_least32_t prop_offset = atomic_load_explicit(&current->prop, memory_order_relaxed);
    if (prop_offset != 0) {
        return to_prop_info(&current->prop);
    } else if (alloc_if_needed) {
        uint_least32_t new_offset;
        prop_info* new_info = new_prop_info(name, namelen, value, valuelen, &new_offset);
        if (new_info) {
            atomic_store_explicit(&current->prop, new_offset, memory_order_release);
        }

        return new_info;
    } else {
        return NULL;
    }
}

static int send_prop_msg(const prop_msg *msg)
{
    const int fd = socket(AF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (fd == -1) {
        return -1;
    }

    const size_t namelen = strlen(property_service_socket);

    sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    strlcpy(addr.sun_path, property_service_socket, sizeof(addr.sun_path));
    addr.sun_family = AF_LOCAL;
    socklen_t alen = namelen + offsetof(sockaddr_un, sun_path) + 1;
    if (TEMP_FAILURE_RETRY(connect(fd, reinterpret_cast<sockaddr*>(&addr), alen)) < 0) {
        close(fd);
        return -1;
    }

    const int num_bytes = TEMP_FAILURE_RETRY(send(fd, msg, sizeof(prop_msg), 0));

    int result = -1;
    if (num_bytes == sizeof(prop_msg)) {
        // We successfully wrote to the property server but now we
        // wait for the property server to finish its work.  It
        // acknowledges its completion by closing the socket so we
        // poll here (on nothing), waiting for the socket to close.
        // If you 'adb shell setprop foo bar' you'll see the POLLHUP
        // once the socket closes.  Out of paranoia we cap our poll
        // at 250 ms.
        pollfd pollfds[1];
        pollfds[0].fd = fd;
        pollfds[0].events = 0;
        const int poll_result = TEMP_FAILURE_RETRY(poll(pollfds, 1, 250 /* ms */));
        if (poll_result == 1 && (pollfds[0].revents & POLLHUP) != 0) {
            result = 0;
        } else {
            // Ignore the timeout and treat it like a success anyway.
            // The init process is single-threaded and its property
            // service is sometimes slow to respond (perhaps it's off
            // starting a child process or something) and thus this
            // times out and the caller thinks it failed, even though
            // it's still getting around to it.  So we fake it here,
            // mostly for ctl.* properties, but we do try and wait 250
            // ms so callers who do read-after-write can reliably see
            // what they've written.  Most of the time.
            // TODO: fix the system properties design.
            result = 0;
        }
    }

    close(fd);
    return result;
}

static void find_nth_fn(const prop_info *pi, void *ptr)
{
    find_nth_cookie *cookie = reinterpret_cast<find_nth_cookie*>(ptr);

    if (cookie->n == cookie->count)
        cookie->pi = pi;

    cookie->count++;
}

bool prop_area::foreach_property(prop_bt *const trie,
        void (*propfn)(const prop_info *pi, void *cookie), void *cookie)
{
    if (!trie)
        return false;

    uint_least32_t left_offset = atomic_load_explicit(&trie->left, memory_order_relaxed);
    if (left_offset != 0) {
        const int err = foreach_property(to_prop_bt(&trie->left), propfn, cookie);
        if (err < 0)
            return false;
    }
    uint_least32_t prop_offset = atomic_load_explicit(&trie->prop, memory_order_relaxed);
    if (prop_offset != 0) {
        prop_info *info = to_prop_info(&trie->prop);
        if (!info)
            return false;
        propfn(info, cookie);
    }
    uint_least32_t children_offset = atomic_load_explicit(&trie->children, memory_order_relaxed);
    if (children_offset != 0) {
        const int err = foreach_property(to_prop_bt(&trie->children), propfn, cookie);
        if (err < 0)
            return false;
    }
    uint_least32_t right_offset = atomic_load_explicit(&trie->right, memory_order_relaxed);
    if (right_offset != 0) {
        const int err = foreach_property(to_prop_bt(&trie->right), propfn, cookie);
        if (err < 0)
            return false;
    }

    return true;
}

const prop_info *prop_area::find(const char *name) {
    return find_property(root_node(), name, strlen(name), nullptr, 0, false);
}

bool prop_area::add(const char *name, unsigned int namelen,
                    const char *value, unsigned int valuelen) {
    return find_property(root_node(), name, namelen, value, valuelen, true);
}

bool prop_area::foreach(void (*propfn)(const prop_info* pi, void* cookie), void* cookie) {
    return foreach_property(root_node(), propfn, cookie);
}

struct context_node {
    context_node(struct context_node* next, const char* context, prop_area* pa)
        : mutex(PTHREAD_MUTEX_INITIALIZER), context(strdup(context)),
          pa(pa), checked_access(false), next(next) {
    }
    ~context_node() {
        if (pa) {
            munmap(pa, pa_size);
        }
        free(context);
    }
    pthread_mutex_t mutex;
    char* context;
    prop_area* pa;
    bool checked_access;
    struct context_node* next;
};

struct prefix_node {
    prefix_node(struct prefix_node* next, const char* prefix, context_node* context)
        : prefix(strdup(prefix)), prefix_len(strlen(prefix)), context(context), next(next) {
    }
    ~prefix_node() {
        free(prefix);
    }
    char* prefix;
    const size_t prefix_len;
    context_node* context;
    struct prefix_node* next;
};

template<typename List, typename... Args>
static inline void list_add(List** list, Args... args) {
    *list = new List(*list, args...);
}

static void list_add_after_len(prefix_node** list,
                               const char* prefix, context_node* context) {
    size_t prefix_len = strlen(prefix);

    auto next_list = list;

    while (*next_list) {
        if ((*next_list)->prefix_len < prefix_len ||
            (*next_list)->prefix[0] == '*') {
            list_add(next_list, prefix, context);
            return;
        }
        next_list = &(*next_list)->next;
    }
    list_add(next_list, prefix, context);
}

template<typename List, typename Func>
static void list_foreach(List* list, Func func) {
    while (list) {
        func(list);
        list = list->next;
    }
}

template<typename List, typename Func>
static List* list_find(List* list, Func func) {
    while (list) {
        if (func(list)) {
            return list;
        }
        list = list->next;
    }
    return nullptr;
}

template<typename List>
static void list_free(List** list) {
    while (*list) {
        auto old_list = *list;
        *list = old_list->next;
        delete old_list;
    }
}

prefix_node* prefixes = nullptr;
context_node* contexts = nullptr;

static void open_prop_file(context_node* cnode, bool access_rw) {
    pthread_mutex_lock(&cnode->mutex);
    if (cnode->pa) {
        pthread_mutex_unlock(&cnode->mutex);
        return;
    }

    char* filename_with_context;
    asprintf(&filename_with_context, "%s/%s",
             secure_property_filename, cnode->context);

    if (access_rw) {
        cnode->pa =
            map_prop_area_rw(filename_with_context, cnode->context);
    } else {
        cnode->pa =
            map_prop_area(filename_with_context, false);
    }
    free(filename_with_context);
    pthread_mutex_unlock(&cnode->mutex);
}

static prop_area* get_prop_area_for_name(const char* name) {
    auto entry = list_find(prefixes, [name] (auto l) {
            return l->prefix[0] == '*' || !strncmp(l->prefix, name, l->prefix_len);
        });
    if (!entry) {
        return nullptr;
    }

    auto cnode = entry->context;
    if (!cnode->pa) {
        open_prop_file(cnode, false);
    }
    return cnode->pa;
}

static int initialize_properties() {
    list_free(&prefixes);
    list_free(&contexts);

    FILE* fd = fopen("/property_contexts", "re");

    if (!fd) {
        return -1;
    }

    char* buffer = new char[512];
    char* prop_prefix = new char[256];
    char* context = new char[256];

    while (fgets(buffer, 512, fd)) {
        if (buffer[0] == '#') {
            continue;
        }
        if (sscanf(buffer, " %255s %255s\n", prop_prefix, context) == 2) {
            auto old_context = list_find(contexts, [context] (auto l) {
                    return !strcmp(l->context, context);
                });
            if (old_context) {
                list_add_after_len(&prefixes, prop_prefix, old_context);
            } else {
                list_add(&contexts, context, nullptr);
                list_add_after_len(&prefixes, prop_prefix, contexts);
            }
        }
    }

    delete[] buffer;
    delete[] prop_prefix;
    delete[] context;
    fclose(fd);
    return 0;
}

bool check_access(context_node* cnode) {
    char* filename_with_context;
    asprintf(&filename_with_context, "%s/%s",
             secure_property_filename, cnode->context);

    bool ret = (access(filename_with_context, R_OK) == 0);
    free(filename_with_context);
    return ret;
}

int __system_properties_init()
{
    if (initialize_properties()) {
        return -1;
    }
    __system_property_area__ = map_prop_area(property_filename, true);
    return __system_property_area__ ? 0 : -1;
}

int __system_property_set_filename(const char *filename)
{
    size_t len = strlen(filename) + strlen("secure__");;
    if (len >= sizeof(secure_property_filename))
        return -1;

    strcpy(property_filename, filename);
    snprintf(secure_property_filename, sizeof(secure_property_filename),
             "%ssecure__", property_filename);
    return 0;
}

int __system_property_area_init()
{
    mkdir(secure_property_filename, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    if (initialize_properties()) {
        return -1;
    }
    list_foreach(contexts, [] (auto l) {
            open_prop_file(l, true);
        });
    __system_property_area__ = map_prop_area_rw(property_filename, nullptr);
    return __system_property_area__ ? 0 : -1;
}

unsigned int __system_property_area_serial()
{
    prop_area *pa = __system_property_area__;
    if (!pa) {
        return -1;
    }
    // Make sure this read fulfilled before __system_property_serial
    return atomic_load_explicit(pa->serial(), memory_order_acquire);
}

const prop_info *__system_property_find(const char *name)
{
    if (__predict_false(compat_mode)) {
        return __system_property_find_compat(name);
    }

    prop_area* pa = get_prop_area_for_name(name);
    if (!pa) {
        return nullptr;
    }

    return pa->find(name);
}

// The C11 standard doesn't allow atomic loads from const fields,
// though C++11 does.  Fudge it until standards get straightened out.
static inline uint_least32_t load_const_atomic(const atomic_uint_least32_t* s,
                                               memory_order mo) {
    atomic_uint_least32_t* non_const_s = const_cast<atomic_uint_least32_t*>(s);
    return atomic_load_explicit(non_const_s, mo);
}

int __system_property_read(const prop_info *pi, char *name, char *value)
{
    if (__predict_false(compat_mode)) {
        return __system_property_read_compat(pi, name, value);
    }

    while (true) {
        uint32_t serial = __system_property_serial(pi); // acquire semantics
        size_t len = SERIAL_VALUE_LEN(serial);
        memcpy(value, pi->value, len + 1);
        // TODO: Fix the synchronization scheme here.
        // There is no fully supported way to implement this kind
        // of synchronization in C++11, since the memcpy races with
        // updates to pi, and the data being accessed is not atomic.
        // The following fence is unintuitive, but would be the
        // correct one if memcpy used memory_order_relaxed atomic accesses.
        // In practice it seems unlikely that the generated code would
        // would be any different, so this should be OK.
        atomic_thread_fence(memory_order_acquire);
        if (serial ==
                load_const_atomic(&(pi->serial), memory_order_relaxed)) {
            if (name != 0) {
                strcpy(name, pi->name);
            }
            return len;
        }
    }
}

int __system_property_get(const char *name, char *value)
{
    const prop_info *pi = __system_property_find(name);

    if (pi != 0) {
        return __system_property_read(pi, 0, value);
    } else {
        value[0] = 0;
        return 0;
    }
}

int __system_property_set(const char *key, const char *value)
{
    if (key == 0) return -1;
    if (value == 0) value = "";
    if (strlen(key) >= PROP_NAME_MAX) return -1;
    if (strlen(value) >= PROP_VALUE_MAX) return -1;

    prop_msg msg;
    memset(&msg, 0, sizeof msg);
    msg.cmd = PROP_MSG_SETPROP;
    strlcpy(msg.name, key, sizeof msg.name);
    strlcpy(msg.value, value, sizeof msg.value);

    const int err = send_prop_msg(&msg);
    if (err < 0) {
        return err;
    }

    return 0;
}

int __system_property_update(prop_info *pi, const char *value, unsigned int len)
{
    prop_area *pa = __system_property_area__;

    if (len >= PROP_VALUE_MAX)
        return -1;

    uint32_t serial = atomic_load_explicit(&pi->serial, memory_order_relaxed);
    serial |= 1;
    atomic_store_explicit(&pi->serial, serial, memory_order_relaxed);
    // The memcpy call here also races.  Again pretend it
    // used memory_order_relaxed atomics, and use the analogous
    // counterintuitive fence.
    atomic_thread_fence(memory_order_release);
    memcpy(pi->value, value, len + 1);
    atomic_store_explicit(
        &pi->serial,
        (len << 24) | ((serial + 1) & 0xffffff),
        memory_order_release);
    __futex_wake(&pi->serial, INT32_MAX);

    atomic_store_explicit(
        pa->serial(),
        atomic_load_explicit(pa->serial(), memory_order_relaxed) + 1,
        memory_order_release);
    __futex_wake(pa->serial(), INT32_MAX);

    return 0;
}

int __system_property_add(const char *name, unsigned int namelen,
            const char *value, unsigned int valuelen)
{
    if (namelen >= PROP_NAME_MAX)
        return -1;
    if (valuelen >= PROP_VALUE_MAX)
        return -1;
    if (namelen < 1)
        return -1;

    prop_area* pa = get_prop_area_for_name(name);

    if (!pa) {
        return -1;
    }

    bool ret = pa->add(name, namelen, value, valuelen);
    if (!ret)
        return -1;

    // There is only a single mutator, but we want to make sure that
    // updates are visible to a reader waiting for the update.
    atomic_store_explicit(
        __system_property_area__->serial(),
        atomic_load_explicit(__system_property_area__->serial(), memory_order_relaxed) + 1,
        memory_order_release);
    __futex_wake(__system_property_area__->serial(), INT32_MAX);
    return 0;
}

// Wait for non-locked serial, and retrieve it with acquire semantics.
unsigned int __system_property_serial(const prop_info *pi)
{
    uint32_t serial = load_const_atomic(&pi->serial, memory_order_acquire);
    while (SERIAL_DIRTY(serial)) {
        __futex_wait(const_cast<volatile void *>(
                        reinterpret_cast<const void *>(&pi->serial)),
                     serial, NULL);
        serial = load_const_atomic(&pi->serial, memory_order_acquire);
    }
    return serial;
}

unsigned int __system_property_wait_any(unsigned int serial)
{
    prop_area *pa = __system_property_area__;
    uint32_t my_serial;

    do {
        __futex_wait(pa->serial(), serial, NULL);
        my_serial = atomic_load_explicit(pa->serial(), memory_order_acquire);
    } while (my_serial == serial);

    return my_serial;
}

const prop_info *__system_property_find_nth(unsigned n)
{
    find_nth_cookie cookie(n);

    const int err = __system_property_foreach(find_nth_fn, &cookie);
    if (err < 0) {
        return NULL;
    }

    return cookie.pi;
}

int __system_property_foreach(void (*propfn)(const prop_info *pi, void *cookie),
        void *cookie)
{
    if (__predict_false(compat_mode)) {
        return __system_property_foreach_compat(propfn, cookie);
    }

    list_foreach(contexts, [propfn, cookie] (auto l) {
            if (!l->pa && !l->checked_access) {
                if (check_access(l)) {
                    open_prop_file(l, false);
                }
                l->checked_access = true;
            }
            if (l->pa) {
                l->pa->foreach(propfn, cookie);
            }
        });
    return 0;
}
