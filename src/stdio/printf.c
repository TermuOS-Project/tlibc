#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char *buf;
    size_t cap;
    size_t pos;
    int use_buf;
} out_t;

static void out_char(out_t *o, char c)
{
    if (o->use_buf) {
        if (o->pos + 1 < o->cap)
            o->buf[o->pos] = c;
        o->pos++;
    } else {
        write(1, &c, 1);
        o->pos++;
    }
}

static void out_str(out_t *o, const char *s)
{
    if (!s)
        s = "(null)";
    while (*s)
        out_char(o, *s++);
}

static void out_uint(out_t *o, unsigned long v, int base, int upper)
{
    char tmp[32];
    const char *digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
    int i = 0;
    if (v == 0) {
        out_char(o, '0');
        return;
    }
    while (v && i < (int)sizeof(tmp)) {
        tmp[i++] = digits[v % (unsigned)base];
        v /= (unsigned)base;
    }
    while (i--)
        out_char(o, tmp[i]);
}

static void out_int(out_t *o, long v)
{
    if (v < 0) {
        out_char(o, '-');
        out_uint(o, (unsigned long)(-v), 10, 0);
    } else {
        out_uint(o, (unsigned long)v, 10, 0);
    }
}

static int do_printf(out_t *o, const char *fmt, va_list ap)
{
    if (!fmt)
        return 0;
    for (; *fmt; fmt++) {
        if (*fmt != '%') {
            out_char(o, *fmt);
            continue;
        }
        fmt++;
        if (!*fmt)
            break;
        switch (*fmt) {
        case '%':
            out_char(o, '%');
            break;
        case 'c':
            out_char(o, (char)va_arg(ap, int));
            break;
        case 's':
            out_str(o, va_arg(ap, const char *));
            break;
        case 'd':
        case 'i':
            out_int(o, (long)va_arg(ap, int));
            break;
        case 'u':
            out_uint(o, (unsigned long)va_arg(ap, unsigned int), 10, 0);
            break;
        case 'x':
            out_uint(o, (unsigned long)va_arg(ap, unsigned int), 16, 0);
            break;
        case 'X':
            out_uint(o, (unsigned long)va_arg(ap, unsigned int), 16, 1);
            break;
        case 'p': {
            unsigned long p = (unsigned long)va_arg(ap, void *);
            out_str(o, "0x");
            out_uint(o, p, 16, 0);
            break;
        }
        case 'l': {
            fmt++;
            if (*fmt == 'd' || *fmt == 'i')
                out_int(o, va_arg(ap, long));
            else if (*fmt == 'u')
                out_uint(o, (unsigned long)va_arg(ap, unsigned long), 10, 0);
            else if (*fmt == 'x')
                out_uint(o, (unsigned long)va_arg(ap, unsigned long), 16, 0);
            else if (*fmt == 'x' || *fmt == 'X')
                out_uint(o, (unsigned long)va_arg(ap, unsigned long), 16, *fmt == 'X');
            else {
                out_char(o, '%');
                out_char(o, 'l');
                if (*fmt)
                    out_char(o, *fmt);
            }
            break;
        }
        default:
            out_char(o, '%');
            out_char(o, *fmt);
            break;
        }
    }
    if (o->use_buf && o->cap) {
        size_t end = o->pos < o->cap ? o->pos : o->cap - 1;
        o->buf[end] = '\0';
    }
    return (int)o->pos;
}

int vprintf(const char *fmt, va_list ap)
{
    out_t o = {0};
    return do_printf(&o, fmt, ap);
}

int printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int n = vprintf(fmt, ap);
    va_end(ap);
    return n;
}

int vsnprintf(char *buf, size_t n, const char *fmt, va_list ap)
{
    out_t o = {.buf = buf, .cap = n, .pos = 0, .use_buf = 1};
    return do_printf(&o, fmt, ap);
}

int snprintf(char *buf, size_t n, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int r = vsnprintf(buf, n, fmt, ap);
    va_end(ap);
    return r;
}
