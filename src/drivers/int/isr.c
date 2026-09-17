#include "isr.h"
#include "IDT.h"
#include <stdint.h>

void* isr_stub_table[] = {
    isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7,
    isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15,
    isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
    isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31,
    irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7,
    irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15
};

extern struct idtr_64 idtr;

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void idt_init() {
    init_idt();

    // Устанавливаем обработчики исключений (0-31)
    for (int i = 0; i < 32; i++) {
        set_idt_entry(i, (uint64_t)isr_stub_table[i], 0, 0x8E);
    }

    // Устанавливаем обработчики IRQ (32-47)
    for (int i = 32; i < 48; i++) {
        set_idt_entry(i, (uint64_t)isr_stub_table[i], 0, 0x8E);
    }
    asm volatile("lidt (%0)" : : "r" (&idtr));
}

void isr_handler(registers_t* regs) {
    (void)regs;
}

#include "../keyboard/toclick.h"

void irq_handler(registers_t* regs) {
    (void)regs;
    
    switch (regs->vector_number) {
        case 33: {
            uint8_t scancode = inb(0x60);
            to_source_click(scancode);
            break;
        }

        default: {
            if (regs->vector_number >= 40) {
                asm volatile("outb %%al, %%dx" : : "a"(0x20), "d"(0xA0));
            }
            asm volatile("outb %%al, %%dx" : : "a"(0x20), "d"(0x20));
            return;
        }
    }
    
    if (regs->vector_number >= 40) {
        asm volatile("outb %%al, %%dx" : : "a"(0x20), "d"(0xA0));
    }
    asm volatile("outb %%al, %%dx" : : "a"(0x20), "d"(0x20));
}

