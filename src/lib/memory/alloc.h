#ifndef ALLOCATOR
#define ALLOCATOR
#include <stddef.h>
#include <stdint.h>

#define KMALLOC_MAGIC 0x4B4D414C
#define SLAB_MAGIC 0x534C4142

#define SLAB_MIN_SHIFT 4
#define SLAB_MAX_SHIFT 11
#define SLAB_NUM_CLASSES (SLAB_MAX_SHIFT - SLAB_MIN_SHIFT + 1)

#define SLAB_PAGE_SIZE 4096

typedef struct {
    uint32_t magic;
    uint16_t level;    
    uint16_t user_lvl; 
    uint64_t pad;      
} __attribute__((packed)) BlockHeader;

typedef struct {
    uint32_t magic;
    uint16_t class_idx;
    uint16_t pad0;
    uint64_t pad1;
} __attribute__((packed)) SlabHeader;
static const uint64_t HEADER_SIZE = sizeof(BlockHeader); /* 16 */

void* kmalloc(uint64_t size);
void kfree(void* ptr);
void* malloc(size_t size);
void  free(void* ptr);
void* calloc(size_t n, size_t sz);
void* realloc(void* p, size_t new_size);
void* memcpy(void* dst, const void* src, size_t n);
void* memset(void* dst, int c, size_t n);
#endif