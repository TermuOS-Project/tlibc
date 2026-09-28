#include <stdio.h>
#include "../sysdep.h"

int putchar(int c)
{
    char ch = (char)c;
    if (write(1, &ch, 1) != 1)
        return -1;
    return (unsigned char)ch;
}
