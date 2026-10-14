#include "clock.h"
#include <stdint.h>

volatile uint64_t *hpet_address;


// #define DEBUG_CLOCK
#ifdef DEBUG_CLOCK
#include "../video/video.h"
#include "../str.h"
#endif

#define GENERAL_CONF_REGISTER_HPET_OFFSET 0x010
#define GENERAL_INTERRUPT_REGISTER_HPET_OFFSET 0x020
#define GENERAL_MAIN_COUNTER_REGISTER_HPET_OFFSET 0x010

uint64_t _read_hpet(uint16_t offset) {
    return *(volatile uint64_t *)((volatile uint8_t *)hpet_address + offset);
}
void init_clock_driver(struct HPETTable *hpet) {
    hpet_address = (volatile uint64_t *)hpet->base_address.address;

    uint64_t data_0x000 = _read_hpet(0x000);
    uint8_t num_timers = (data_0x000 & 0x1F) + 1;
    uint32_t period = (uint32_t)(data_0x000 >> 32);
    uint64_t hpet_freq = 1000000000000000ULL / period;
    
    #ifdef DEBUG_CLOCK

    char buffer[21];
    for (uint16_t i = 0; i < 21; i++) buffer[i] = ' ';
    print("period:");
    print(uint64_to_str(period, buffer));
    print("\nhpet_freq: ");
    print(uint64_to_str(hpet_freq, buffer));
    print("\nnum_timers: ");
    print(uint64_to_str(num_timers, buffer));
    print("\nis 64-bits main counter: ");
    if (((data_0x000 >> 13) & 1) < 1) {
        print("False. 32Bits");
    } else {
        print("True. 64Bits");
    }
    print_symbol('\n');
    #endif
}

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

uint64_t rdtsc_serialized(void) {
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