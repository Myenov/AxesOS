#include <stdint.h>
typedef struct {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t acpiext;
} __attribute__((packed)) MMEmmap;

volatile uint64_t all_ram = 0;
volatile uint64_t use_ram = 0;

void init_alloc_driver(uint64_t e820_count) {
    MMEmmap* mmap = (MMEmmap*)0x6000;
    
    for (uint32_t i = 0; i < e820_count; i++) {
        all_ram += mmap[i].length;
        
        if (mmap[i].type == 1) {
            use_ram += mmap[i].length;
        }
    }
}