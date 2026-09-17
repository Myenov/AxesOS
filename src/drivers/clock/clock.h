#ifndef CLOCK_FILE_DRIVER
#define CLOCK_FILE_DRIVER

#include "../outb_and_inb.h"
#include <stdint.h>



uint64_t rdtsc(void);
uint64_t rdtscp(void);
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
#endif