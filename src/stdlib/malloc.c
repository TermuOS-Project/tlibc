#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

typedef struct block {
    size_t size;
    int free;
    struct block *next;
} block_t;

static block_t *head;
static int ready;

static int heap_grow(size_t need)
{
    size_t total = need + sizeof(block_t) + 32;
    total = (total + 0xFFF) & ~0xFFFul;

    void *p = sbrk((long)total);
    if (p == (void *)-1 || p == 0)
        return -1;

    block_t *b = (block_t *)p;
    b->size = total - sizeof(block_t);
    b->free = 1;
    b->next = 0;

    if (!head) {
        head = b;
    } else {
        block_t *t = head;
        while (t->next)
            t = t->next;
        t->next = b;
    }
    return 0;
}

static void heap_init(void)
{
    if (heap_grow(4096) == 0)
        ready = 1;
}

void *malloc(size_t n)
{
    if (n == 0)
        return 0;
    n = (n + 7u) & ~7u;
    if (!ready)
        heap_init();
    if (!ready)
        return 0;

    for (;;) {
        for (block_t *b = head; b; b = b->next) {
            if (b->free && b->size >= n) {
                if (b->size >= n + sizeof(block_t) + 8) {
                    block_t *rest =
                        (block_t *)((unsigned char *)b + sizeof(block_t) + n);
                    rest->size = b->size - n - sizeof(block_t);
                    rest->free = 1;
                    rest->next = b->next;
                    b->next = rest;
                    b->size = n;
                }
                b->free = 0;
                return (unsigned char *)b + sizeof(block_t);
            }
        }
        if (heap_grow(n) != 0)
            return 0;
    }
}

void free(void *p)
{
    if (!p)
        return;
    block_t *b = (block_t *)((unsigned char *)p - sizeof(block_t));
    b->free = 1;

    /* merge forward */
    if (b->next && b->next->free) {
        b->size += sizeof(block_t) + b->next->size;
        b->next = b->next->next;
    }
}

void *calloc(size_t n, size_t sz)
{
    size_t total = n * sz;
    void *p = malloc(total);
    if (p)
        memset(p, 0, total);
    return p;
}

void *realloc(void *p, size_t n)
{
    if (!p)
        return malloc(n);
    if (n == 0) {
        free(p);
        return 0;
    }
    block_t *b = (block_t *)((unsigned char *)p - sizeof(block_t));
    if (b->size >= n)
        return p;
    void *q = malloc(n);
    if (!q)
        return 0;
    memcpy(q, p, b->size);
    free(p);
    return q;
}
