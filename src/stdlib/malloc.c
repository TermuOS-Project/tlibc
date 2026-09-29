#include <stdlib.h>
#include <string.h>

#define HEAP_SIZE (256 * 1024)

typedef struct block {
    size_t size;
    int free;
    struct block *next;
} block_t;

static unsigned char heap[HEAP_SIZE];
static block_t *head;
static int ready;

static void heap_init(void)
{
    head = (block_t *)heap;
    head->size = HEAP_SIZE - sizeof(block_t);
    head->free = 1;
    head->next = 0;
    ready = 1;
}

void *malloc(size_t n)
{
    if (n == 0)
        return 0;
    if (!ready)
        heap_init();

    /* align */
    n = (n + 7u) & ~7u;

    block_t *b = head;
    while (b) {
        if (b->free && b->size >= n) {
            if (b->size >= n + sizeof(block_t) + 8) {
                block_t *rest = (block_t *)((unsigned char *)b + sizeof(block_t) + n);
                rest->size = b->size - n - sizeof(block_t);
                rest->free = 1;
                rest->next = b->next;
                b->next = rest;
                b->size = n;
            }
            b->free = 0;
            return (unsigned char *)b + sizeof(block_t);
        }
        b = b->next;
    }
    return 0;
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
