#include "IDT.h"
#include <stdint.h>

#pragma pack(push, 1)
struct idt_entry_64 {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));
#pragma pack(pop)

struct idtr_64 idtr;
struct idt_entry_64 idt[256];

void set_idt_entry(int vector, uint64_t handler, uint8_t ist, uint8_t type_attr) {
    idt[vector].offset_low = (uint16_t)(handler & 0xFFFF);
    idt[vector].selector = 0x08;
    idt[vector].ist = ist;
    idt[vector].type_attr = type_attr;
    idt[vector].offset_mid = (uint16_t)((handler >> 16) & 0xFFFF);
    idt[vector].offset_high = (uint32_t)((handler >> 32) & 0xFFFFFFFF);
    idt[vector].reserved = 0;
}

void init_idt() {
    idtr.limit = sizeof(struct idt_entry_64) * 256 - 1;
    idtr.base = (uint64_t)&idt;
    
    asm volatile("lidt (%0)" : : "r" (&idtr));
}