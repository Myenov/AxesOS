#include "../../lib/apic/apic.h"
#include "../../lib/msr/msr.h"
#include "../../lib/apic/madt/madt.h"
#include <stdint.h>

volatile uint32_t* lapic_base;
volatile uint32_t* ioapic_base;

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

uint32_t lapic_get_id() {
    return read_lapic(0x20) >> 24;
}

void write_lapic(uint32_t offset, uint32_t value) {
    *(volatile uint32_t*)((uintptr_t)lapic_base + offset) = value;
}

uint32_t read_lapic(uint32_t offset) {
    return *(volatile uint32_t*)((uintptr_t)lapic_base + offset);
}

void ioapic_write(uint32_t reg, uint32_t data) {
    *(volatile uint32_t*)((uintptr_t)ioapic_base + 0x00) = reg;
    *(volatile uint32_t*)((uintptr_t)ioapic_base + 0x10) = data;
}

uint32_t ioapic_read(uint32_t reg) {
    *(volatile uint32_t*)((uintptr_t)ioapic_base + 0x00) = reg;
    return *(volatile uint32_t*)((uintptr_t)ioapic_base + 0x10);
}


void ioapic_set_irq(uint8_t irq, uint64_t apic_id, uint8_t vector) {
    uint32_t reg_low = 0x10 + (irq * 2);
    uint32_t reg_high = 0x11 + (irq * 2);

    uint32_t low_part = vector;
    uint32_t high_part = (uint32_t)apic_id << 24;

    ioapic_write(reg_low, low_part);
    ioapic_write(reg_high, high_part);
}

void lapic_eoi() {
    write_lapic(0x0B0, 0);
}

void init_apic(uintptr_t lapic_address, uintptr_t ioapic_address) {
    outb(0x21, 0xFF); // Маскировка master PIC
    outb(0xA1, 0xFF); // Маскировка slave PIC
    uint64_t apic_base_msr = read_msr(IA32_APIC_BASE_MSR);
    apic_base_msr |= IA32_APIC_BASE_MSR_ENABLE;
    write_msr(IA32_APIC_BASE_MSR, apic_base_msr);
    
    lapic_base = (volatile uint32_t *)lapic_address;
    ioapic_base = (volatile uint32_t *)ioapic_address;
    
    uint32_t data = read_lapic(0x0F0);
    data |= (1 << 8);
    data |= 0xFF;
    write_lapic(0x0F0, data);
    write_lapic(0x080, 0);

    uint32_t current_apic_id = lapic_get_id();
    ioapic_set_irq(1, current_apic_id, 33);
    // ioapic_set_irq(1, current_apic_id, 40);
}
