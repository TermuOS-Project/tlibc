#pragma once
#include <stdint.h>
#include <syscall.h>

#define OB_ACCESS_READ (1u << 0)
#define OB_ACCESS_WRITE (1u << 1)
#define OB_ACCESS_EXECUTE (1 << 2)
#define OB_ACCESS_ALL (OB_ACCESS_READ | OB_ACCESS_WRITE | OB_ACCESS_EXECUTE)

#define SYS_OB_OPEN 80
#define SYS_OB_CLOSE 81
#define SYS_OB_DUP 82

static inline long ob_open(const char *path, uint32_t access)
{
    return __syscall2(SYS_OB_OPEN, (long)path, (long)access);
}

static inline long ob_close(int handle)
{
    return __syscall1(SYS_OB_CLOSE, (long)handle);
}

static inline long ob_dup(int handle)
{
    return __syscall1(SYS_OB_DUP, (long)handle);
}
