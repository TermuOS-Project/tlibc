#include <stdlib.h>
#include "../sysdep.h"

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
