#ifndef IDT_TABLE_INIT
#define IDT_TABLE_INIT
#include <stdint.h>

#pragma pack(push, 1)
struct idtr_64 {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));
#pragma pack(pop)

void init_idt();
void set_idt_entry(int vector, uint64_t handler, uint8_t ist, uint8_t type_attr);

#endif