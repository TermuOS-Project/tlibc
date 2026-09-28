#include <string.h>
int memcmp(const void *a, const void *b, size_t n)
{
    const unsigned char *aa = a, *bb = b;
    while (n--) {
        if (*aa != *bb) return *aa - *bb;
        aa++; b++;
    }
    return 0;
}
