#ifndef CPU_H 
#define CPU_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t acpi_processor_id;
    uint8_t apic_id;
    uint32_t flags;
} CpuCore;

typedef struct {
    CpuCore* cores;
    uint32_t count;
} CpuTopology;

extern CpuTopology cpu_topology;

void init_chipset_driver_cpu();
#endif