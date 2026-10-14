#ifndef RSDP_H
#define RSDP_H

#include "../ACPI.h"
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