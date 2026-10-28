#ifndef ALLOCATOR
#define ALLOCATOR
#include <stddef.h>
#include <stdint.h>

#define KMALLOC_MAGIC 0x4B4D414C

typedef struct {
    uint32_t magic;
    uint16_t level;    
    uint16_t user_lvl; 
    uint64_t pad;      
} __attribute__((packed)) BlockHeader;

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