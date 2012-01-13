int              fsetxattr (int, char *, const void *, size_t, int);
int              fgetxattr (int, const char *, void *, size_t);
int              setxattr (const char *, const char *, const void *, size_t, int);
int              lsetxattr (const char *, const char *, const void *, size_t, int);
int              getxattr (const char *, const char *, void *, size_t);
int              lgetxattr (const char *, const char *, void *, size_t);
