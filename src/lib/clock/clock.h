#ifndef CLOCK_FILE_DRIVER
#define CLOCK_FILE_DRIVER

#include "../utils/outb_and_inb.h"
#include "../apic/hpet/hpet.h"
#include <stdint.h>

void init_clock_driver(struct HPETTable *hpet);
uint64_t rdtsc(void);
uint64_t rdtscp(void);
uint64_t rdtsc_serialized(void);
static inline uint64_t inline_rdtsc(void) {
    uint32_t low, high;

    __asm__ volatile("rdtsc" : "=a"(low), "=d"(high));
    return ((uint64_t)high << 32) | low;
}

static inline uint64_t inline_rdtscp(void) {
    uint32_t low, high;
    
    __asm__ volatile("rdtscp" : "=a"(low), "=d"(high) :: "%ecx");
    return ((uint64_t)high << 32) | low;
}

static inline uint64_t rdtsc_serialized_s(void) {
    uint32_t low, high;
    __asm__ __volatile__ (
        "lfence\n\t"
        "rdtsc\n\t"
        : "=a"(low), "=d"(high)
        :
        : "memory"
    );
    return ((uint64_t)high << 32) | low;
}
#endif