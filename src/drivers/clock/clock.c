#include "../../lib/clock/clock.h"
#include <stdint.h>

volatile uint64_t *hpet_address;


#define DEBUG_CLOCK
#ifdef DEBUG_CLOCK
#include "../../lib/video/video.h"
#include "../../lib/string/str.h"
#endif

#define GENERAL_CONF_REGISTER_HPET_OFFSET 0x010
#define GENERAL_INTERRUPT_REGISTER_HPET_OFFSET 0x020
#define GENERAL_MAIN_COUNTER_REGISTER_HPET_OFFSET 0x010

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

static inline uint64_t _read_hpet(uint16_t offset) {
    return *(volatile uint64_t *)((volatile uint8_t *)hpet_address + offset);
}
static inline void _write_hpet(uint16_t offset, uint64_t data) {
    *(volatile uint64_t *)((volatile uint8_t *)hpet_address + offset) = data;
}
uint64_t rdtsc_freq_mhz = 0;

void init_clock_driver(struct HPETTable *hpet) {
    hpet_address = (volatile uint64_t *)hpet->base_address.address;

    uint64_t data_0x000 = _read_hpet(0x000);
    uint32_t period = (uint32_t)(data_0x000 >> 32);

    uint64_t hpet_freq = (period != 0) ? (1000000000000000ULL / period) : 0;

    #ifdef DEBUG_CLOCK
        char buffer[21];
        for (uint16_t i = 0; i < 21; i++) buffer[i] = ' ';
        print("period:");
        print(uint64_to_str(period, buffer));
        for (uint16_t i = 0; i < 21; i++) buffer[i] = ' ';
        print("\nhpet_freq: ");
        print(uint64_to_str(hpet_freq, buffer));
        for (uint16_t i = 0; i < 21; i++) buffer[i] = ' ';
        print("\nnum_timers: ");
        for (uint16_t i = 0; i < 21; i++) buffer[i] = ' ';
        print(uint64_to_str((data_0x000 & 0x1F) + 1, buffer));
        print("\nis 64-bits main counter: ");
        if (((data_0x000 >> 13) & 1) == 0) print("False. 32Bits");
        else print("True. 64Bits");
        print_symbol('\n');
    #endif

    uint64_t ticks_to_wait = 100000000000000ULL / period; 
    uint64_t wait_fs = ticks_to_wait * (uint64_t)period;

    uint64_t cfg = _read_hpet(0x10);
    cfg |= 1ULL;
    _write_hpet(0x10, cfg);

    uint64_t hpet_start = _read_hpet(0xF0);
    uint64_t rdtsc_start = rdtsc_serialized();
    uint64_t hpet_target = hpet_start + ticks_to_wait;
    uint64_t guard = 0;
    while (_read_hpet(0xF0) <= hpet_target) {
        if (++guard > (ticks_to_wait * 4 + 1000000ULL)) break;
    }

    uint64_t rdtsc_final = rdtsc_serialized();
    uint64_t rdtsc_ticks = rdtsc_final - rdtsc_start;
    uint64_t num = 1000000000ULL;
    uint64_t den = wait_fs;

    uint64_t a = num, b = den;
    while (b) { uint64_t t = a % b; a = b; b = t; }
    uint64_t g = a ? a : 1;
    num /= g;
    den /= g;

    if (den == 0) den = 1;
    if (num == 0) num = 1;

    if (num == 1) {
        rdtsc_freq_mhz = rdtsc_ticks / den;
    } else {
        uint64_t q = rdtsc_ticks / den;
        uint64_t r = rdtsc_ticks % den;
        rdtsc_freq_mhz = q * num + (r / den) * num + ((r % den) * num) / den;
    }

    if (rdtsc_freq_mhz == 0) {
        #ifdef DEBUG_CLOCK
            print("rdtsc_freq calibration FAILED\n");
        #endif
    }

    #ifdef DEBUG_CLOCK
        for (uint16_t i = 0; i < 21; i++) buffer[i] = ' ';
        print("hpet_start:");
        print(uint64_to_str(hpet_start, buffer));
        for (uint16_t i = 0; i < 21; i++) buffer[i] = ' ';
        print("\nrdtsc_start: ");
        print(uint64_to_str(rdtsc_start, buffer));
        for (uint16_t i = 0; i < 21; i++) buffer[i] = ' ';
        print("\nhpet_target: ");
        print(uint64_to_str(hpet_target, buffer));
        for (uint16_t i = 0; i < 21; i++) buffer[i] = ' ';
        print("\nrdtsc_final: ");
        print(uint64_to_str(rdtsc_final, buffer));
        for (uint16_t i = 0; i < 21; i++) buffer[i] = ' ';
        print("\nrdtsc_ticks: ");
        print(uint64_to_str(rdtsc_ticks, buffer));
        for (uint16_t i = 0; i < 21; i++) buffer[i] = ' ';
        print("\nrdtsc_freq_mhz: ");
        print(uint64_to_str(rdtsc_freq_mhz, buffer));
        print_symbol('\n');
    #endif
}