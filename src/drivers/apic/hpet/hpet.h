#ifndef HPET_H
#define HPET_H

#include "../ACPI.h"
#include "../xsdt/xsdt.h"
#include <stdint.h>

struct HPETTable {
    struct ACPISDTHeader header;
    uint32_t event_timer_blick_id;
    struct ACPIAddress base_address;
    uint8_t hpet_number;
    uint16_t main_counter_minimum_clock_tick;
    uint8_t page_protection_and_oem_attribute;
} __attribute__((packed));

struct HPETTable* search_HPET_table_in_XSDT(struct XSDT *xsdt);
struct HPETTable* search_HPET_table_in_RSDT(struct RSDT *rsdt);
#endif