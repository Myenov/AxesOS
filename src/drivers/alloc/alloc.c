#include "../../lib/memory/alloc.h"
#include "../../lib/memory/allocator.h"

void* memcpy(void* dst, const void* src, size_t n) {
    uint8_t* d = (uint8_t*)dst;
    const uint8_t* s = (const uint8_t*)src;
    while (n--) *d++ = *s++;
    return dst;
}

void* memset(void* dst, int c, size_t n) {
    uint8_t* d = (uint8_t*)dst;
    while (n--) *d++ = (uint8_t)c;
    return dst;
}

void* kmalloc(uint64_t size) {
    if (size == 0) return NULL;

    uint64_t need = size + HEADER_SIZE;
    if (need > MAX_BLOCK_SIZE) return NULL;

    void* raw = buddy_alloc(need);
    if (!raw) return NULL;

    int lvl = 0;
    uint64_t s = MIN_BLOCK_SIZE;
    while (s < need) { s <<= 1; lvl++; }

    BlockHeader* h = (BlockHeader*)raw;
    h->magic    = KMALLOC_MAGIC;
    h->level    = (uint16_t)lvl;
    h->user_lvl = (uint16_t)lvl;
    h->pad      = 0;

    return (void*)((uint64_t)raw + HEADER_SIZE);
}

void kfree(void* ptr) {
    if (!ptr) return;

    BlockHeader* h = (BlockHeader*)((uint64_t)ptr - HEADER_SIZE);
    if (h->magic != KMALLOC_MAGIC) {
        return;
    }

    uint64_t raw = (uint64_t)h;
    int lvl = h->level;
    uint64_t size = (uint64_t)MIN_BLOCK_SIZE << lvl;
    h->magic = 0;

    buddy_free((void*)raw, size);
}

void* malloc(size_t size) { return kmalloc((uint64_t)size); }
void  free(void* ptr)     { kfree(ptr); }
void* calloc(size_t n, size_t sz) {
    uint64_t total = (uint64_t)n * sz;
    if (n && total / n != sz) return NULL;
    void* p = kmalloc(total);
    if (p) memset(p, 0, total);
    return p;
}
void* realloc(void* p, size_t new_size) {
    if (!p) return malloc(new_size);
    if (new_size == 0) { free(p); return NULL; }
    BlockHeader* h = (BlockHeader*)((uint64_t)p - HEADER_SIZE);
    uint64_t old_size = ((uint64_t)MIN_BLOCK_SIZE << h->level) - HEADER_SIZE;
    if (new_size <= old_size) return p;
    void* np = kmalloc(new_size);
    if (!np) return NULL;
    memcpy(np, p, old_size);
    free(p);
    return np;
}