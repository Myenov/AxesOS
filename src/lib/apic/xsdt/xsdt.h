#ifndef XSDP_H
#define XSDP_H

#include "../../../drivers/apic/ACPI.h"
#include <stdint.h>

struct XSDT {
    struct ACPISDTHeader header;
    uint64_t pointers[];
} __attribute__((packed));

struct RSDT {
    struct ACPISDTHeader header;
    uint32_t pointers[];
} __attribute__((packed));
#endif