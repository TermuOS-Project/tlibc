#include <stdio.h>
#include <string.h>
#include <unistd.h>

int puts(const char *s)
{
    if (!s)
        s = "(null)";
    size_t n = strlen(s);
    if (n && write(1, s, n) < 0)
        return -1;
    if (write(1, "\n", 1) < 0)
        return -1;
    return 0;
}
