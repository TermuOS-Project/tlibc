#include <string.h>
void *memset(void *s, int c, size_t n)
{
    unsigned char *p = s;
    unsigned char v = (unsigned char)c;
    while (n--) *p++ = v;
    return s;
}
