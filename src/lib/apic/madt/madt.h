#ifndef MADT_STRUCT
#define MADT_STRUCT

#include "../../../drivers/apic/ACPI.h"
#include "../rsdp/rsdp.h"
#include "../xsdt/xsdt.h"
#include <stddef.h>
#include <stdint.h>

struct MADT {
  struct ACPISDTHeader header;
  uint32_t lapicaddress;
  uint32_t flags;
} __attribute__((packed)) ;

struct MADTEntry {
    uint8_t type;
    uint8_t length;
} __attribute__((packed));

typedef struct MADT_Stat {
    struct MADTEntry* entry;
    struct MADT_Stat* next;
} MADT_Stat;

//type 0
struct MADT_LocalAPIC {
    struct MADTEntry header;
    uint8_t acpiprocessorid;
    uint8_t apicid;
    uint32_t flags;
} __attribute__((packed));

//type 1
struct MADT_IOAPIC {
    struct MADTEntry header;
    uint8_t ioapicid;
    uint8_t reserved;
    uint32_t ioapicaddress;
    uint32_t gsinumber;
} __attribute__((packed));

//type 2
struct MADT_ISO {
    struct MADTEntry header;
    uint8_t bussource;
    uint8_t irqsource;
    uint32_t globalsysteminterrupt;
    uint16_t flags;
} __attribute__((packed));

//type 4
struct MADT_LAPIC_NMI {
    struct MADTEntry header;
    uint8_t acpiprocessorid;
    uint16_t flags;
    uint8_t linti;
} __attribute__((packed));

extern MADT_Stat *status;

struct MADT* search_MADT_in_XSDT(struct XSDT *xsdt);
struct MADT* search_MADT_in_RSDT(struct RSDT *rsdt);
void parse_madt_entry(struct MADT* madt);
#endif