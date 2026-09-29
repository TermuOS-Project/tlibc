#pragma once

#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

int putchar(int c);
int puts(const char *s);

int printf(const char *fmt, ...);
int vprintf(const char *fmt, va_list ap);
int snprintf(char *buf, size_t n, const char *fmt, ...);
int vsnprintf(char *buf, size_t n, const char *fmt, va_list ap);

#ifdef __cplusplus
}
#endif
