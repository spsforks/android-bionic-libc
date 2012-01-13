#ifndef _SYS_XATTR_H_
#define _SYS_XATTR_H_

#include <sys/types.h>

__BEGIN_DECLS

#define XATTR_CREATE 1
#define XATTR_REPLACE 2

extern int fsetxattr (int fd, char *name, const void *value, size_t size, int flags);
extern int setxattr (const char *path, const char *name, const void *value, size_t size, int flags);
extern int lsetxattr (const char *path, const char *name, const void *value, size_t size, int flags);

extern int fgetxattr (int fd, const char *name, void *value, size_t size);
extern int getxattr (const char *path, const char *name, void *value, size_t size);
extern int lgetxattr (const char *path, const char *name, void *value, size_t size);

extern int listxattr (const char *path, char *list, size_t size);
extern int llistxattr (const char *path, char *list, size_t size);
extern int flistxattr (int fd, char *list, size_t size);

extern int removexattr (const char *path, const char *name);
extern int lremovexattr (const char *path, const char *name);
extern int fremovexattr (int fd, const char *name);

__END_DECLS

#endif /* _SYS_XATTR_H_ */
