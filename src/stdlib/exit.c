#include <stdlib.h>
#include <unistd.h>

void exit(int code)
{
    _exit(code);
    for (;;)
        ;
}

void abort(void)
{
    _exit(127);
    for (;;)
        ;
}
