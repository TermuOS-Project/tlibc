#include <unistd.h>
#include <syscall.h>
#include <stdint.h>

void *brk(void *addr)
{
    return (void *)(uintptr_t)__syscall1(SYS_BRK, (long)(uintptr_t)addr);
}

void *sbrk(long incr)
{
    uintptr_t cur = (uintptr_t)brk(0);
    if (incr == 0)
        return (void *)cur;
    uintptr_t neu = (uintptr_t)brk((void *)(cur + (uintptr_t)incr));
    if (neu < cur + (uintptr_t)incr && incr > 0)
        return (void *)-1;
    return (void *)cur;
}
