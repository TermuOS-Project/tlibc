#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void exit(int code);
void abort(void);

void *malloc(size_t n);
void  free(void *p);
void *calloc(size_t n, size_t sz);
void *realloc(void *p, size_t n);

#ifdef __cplusplus
}
#endif
