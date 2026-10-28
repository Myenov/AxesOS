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
typedef struct SlabBlock {
    struct SlabBlock* next;
} SlabBlock;

typedef struct {
    uint32_t block_size;
    SlabBlock* free_list;
    uint64_t total;
    uint64_t used;
} SlabCache;

static SlabCache slab_caches[SLAB_NUM_CLASSES];
static int slab_inited = 0;

static inline uint32_t slab_class_size(int idx) {
    return 1u << (SLAB_MIN_SHIFT + idx);
}

static void slab_cache_init(void) {
    for (int i = 0; i < SLAB_NUM_CLASSES; i++) {
        slab_caches[i].block_size = slab_class_size(i);
        slab_caches[i].free_list  = NULL;
        slab_caches[i].total      = 0;
        slab_caches[i].used       = 0;
    }
    slab_inited = 1;
}

static void slab_grow(SlabCache* c, int idx) {
    void* page = buddy_alloc(SLAB_PAGE_SIZE);
    if (!page) return;

    uint32_t bs = c->block_size;
    uint32_t count = SLAB_PAGE_SIZE / bs;
    uint8_t* base = (uint8_t*)page;

    SlabHeader* sh = (SlabHeader*)base;
    sh->magic     = SLAB_MAGIC;
    sh->class_idx = (uint16_t)idx;
    sh->pad0 = 0;
    sh->pad1 = 0;

    uint32_t data_off = sizeof(SlabHeader);
    uint32_t n = (SLAB_PAGE_SIZE - data_off) / bs;
    if (n < count) count = n;

    for (uint32_t i = 0; i < count; i++) {
        SlabBlock* b = (SlabBlock*)(base + data_off + i * bs);
        b->next = c->free_list;
        c->free_list = b;
    }
    c->total += count;
}

static int slab_class_for(uint64_t size) {
    for (int i = 0; i < SLAB_NUM_CLASSES; i++) {
        if (size <= slab_class_size(i)) return i;
    }
    return -1;
}

static void* slab_alloc(uint64_t size) {
    if (!slab_inited) slab_cache_init();
    int idx = slab_class_for(size);
    if (idx < 0) return NULL;

    SlabCache* c = &slab_caches[idx];
    if (!c->free_list) {
        slab_grow(c, idx);
        if (!c->free_list) return NULL;
    }
    SlabBlock* b = c->free_list;
    c->free_list = b->next;
    c->used++;
    return (void*)b;
}

static void slab_free(void* ptr) {
    SlabHeader* sh = (SlabHeader*)((uint64_t)ptr & ~(uint64_t)(SLAB_PAGE_SIZE - 1));
    if (sh->magic != SLAB_MAGIC) return;
    int idx = sh->class_idx;
    if (idx < 0 || idx >= SLAB_NUM_CLASSES) return;

    SlabCache* c = &slab_caches[idx];
    SlabBlock* b = (SlabBlock*)ptr;
    b->next = c->free_list;
    c->free_list = b;
    if (c->used) c->used--;
}

static void* big_alloc(uint64_t size) {
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

static void big_free(void* ptr) {
    BlockHeader* h = (BlockHeader*)((uint64_t)ptr - HEADER_SIZE);
    if (h->magic != KMALLOC_MAGIC) return;
    uint64_t raw = (uint64_t)h;
    int lvl = h->level;
    uint64_t size = (uint64_t)MIN_BLOCK_SIZE << lvl;
    h->magic = 0;
    buddy_free((void*)raw, size);
}

void* kmalloc(uint64_t size) {
    if (size == 0) return NULL;
    if (size <= slab_class_size(SLAB_NUM_CLASSES - 1))
        return slab_alloc(size);
    return big_alloc(size);
}

void kfree(void* ptr) {
    if (!ptr) return;

    BlockHeader* h = (BlockHeader*)((uint64_t)ptr - HEADER_SIZE);
    if (h->magic == KMALLOC_MAGIC) {
        big_free(ptr);
        return;
    }

    SlabHeader* sh = (SlabHeader*)((uint64_t)ptr & ~(uint64_t)(SLAB_PAGE_SIZE - 1));
    if (sh->magic == SLAB_MAGIC) {
        slab_free(ptr);
        return;
    }
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
    if (h->magic == KMALLOC_MAGIC) {
        uint64_t old = ((uint64_t)MIN_BLOCK_SIZE << h->level) - HEADER_SIZE;
        if (new_size <= old) return p;
        void* np = kmalloc(new_size);
        if (!np) return NULL;
        memcpy(np, p, old);
        free(p);
        return np;
    }

    SlabHeader* sh = (SlabHeader*)((uint64_t)p & ~(uint64_t)(SLAB_PAGE_SIZE - 1));
    if (sh->magic == SLAB_MAGIC) {
        int idx = sh->class_idx;
        uint64_t old = slab_class_size(idx);
        if (new_size <= old) return p;
        void* np = kmalloc(new_size);
        if (!np) return NULL;
        memcpy(np, p, old);
        free(p);
        return np;
    }
    return NULL;
}