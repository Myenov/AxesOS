#ifndef ALLOCATOR_FILE
#define ALLOCATOR_FILE

#define E820_MAX_ENTRIES 128
#include <stdint.h>
#include <stdbool.h>
#define MIN_BLOCK_SIZE  4096     // 4 КБ
#define MAX_LEVEL       11       // ПМакс блок = 4КБ * 2^10 = 4 МБ
#define MAX_BLOCK_SIZE  ((uint64_t)MIN_BLOCK_SIZE << 10)
#define PAGE_SHIFT 12

void reserve_range(uint64_t start, uint64_t end);
void init_alloc_driver(uint64_t e820_count, uintptr_t ebda_address, uint8_t ebda_size);
static bool take_block(uint64_t addr, int level);
void buddy_free(void* ptr, uint64_t size);
void* buddy_alloc(uint64_t size);


#endif