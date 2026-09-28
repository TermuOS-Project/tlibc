#include <string.h>
char *strncpy(char *d, const char *s, size_t n)
{
    char *r = d;
    size_t i = 0;
    if (!d) return d;
    if (!s) { while (i < n) d[i++] = 0; return r; }
    while (i < n && s[i]) { d[i] = s[i]; i++; }
    while (i < n) d[i++] = 0;
    return r;
}
