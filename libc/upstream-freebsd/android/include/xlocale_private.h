#define __get_locale() NULL
#define FIX_LOCALE(_loc)

static inline struct lconv* localeconv_l(locale_t unused __attribute__((__unused__))) {
  return localeconv();
}
