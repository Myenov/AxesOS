#include "clock.h"

uint64_t rdtsc(void) {
    uint32_t low, high;

    __asm__ volatile("rdtsc" : "=a"(low), "=d"(high));
    return ((uint64_t)high << 32) | low;
}

uint64_t rdtscp(void) {
    uint32_t low, high;
    
    __asm__ volatile("rdtscp" : "=a"(low), "=d"(high) :: "%ecx");
    return ((uint64_t)high << 32) | low;
}