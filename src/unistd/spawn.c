#include <unistd.h>
#include <syscall.h>

long spawn(const char *path)
{
    if (!path)
        return -1;
    return __syscall1(SYS_SPAWN, (long)path);
}

long waitpid(long pid)
{
    return __syscall1(SYS_WAIT, pid);
}
