#ifndef PIC_INIT_FILE_H
#define PIC_INIT_FILE_H
#include "../../lib/msr/msr.h"
#include "../../lib/apic/madt/madt.h"
#include <stdint.h>

void init_apic(uintptr_t lapic_address, uintptr_t ioapic_address);
static inline void outb(uint16_t port, uint8_t value);
static inline uint8_t inb(uint16_t port);
uint32_t read_lapic(uint32_t offset);
void write_lapic(uint32_t offset, uint32_t value);
void lapic_eoi();
void ioapic_write(uint32_t reg, uint32_t data);
void ioapic_set_irq(uint8_t irq, uint64_t apic_id, uint8_t vector);
uint32_t lapic_get_id();
uint32_t ioapic_read(uint32_t reg);
#endif