#ifndef __LIBC_H_
#define __LIBC_H_

#include "streams.h"

#ifndef CONFIG_HAVE_DOUBLE
#define CONFIG_HAVE_DOUBLE
#endif

#ifndef CONFIG_HAVE_LONG_LONG
#define CONFIG_HAVE_LONG_LONG
#endif

#ifndef set_errno
#define set_errno(v)
#endif

void lib_skipspace(const char **pptr);

int lib_checkbase(int base, FAR const char **pptr);

bool lib_isbasedigit(int ch, int base, int *value);

long strtol(FAR const char *nptr, FAR char **endptr, int base);

double strtod(FAR const char *str, FAR char **endptr);

float strtof(FAR const char *str, FAR char **endptr);

int snprintf(char *__restrict __s, size_t __maxlen,
			 const char *__restrict __format, ...);

int vsnprintf(char *__restrict __s, size_t __maxlen,
			  const char *__restrict __format, __gnuc_va_list __arg);

#endif