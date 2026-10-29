#include "../../../lib/apic/madt/madt.h"
#include "../../../lib/apic/xsdt/xsdt.h"
#include <stdint.h>

int sig_eq(const char *a, const char *b, size_t n) {
    for (size_t i = 0; i < n; i++)
        if (a[i] != b[i]) return 0;
    return 1;
}

struct MADT* search_MADT_in_XSDT(struct XSDT *xsdt) {
    uint64_t count = (xsdt->header.length - sizeof(struct ACPISDTHeader)) / sizeof(uint64_t);

    for (uint64_t i = 0; i < count; i++) {
        struct ACPISDTHeader *table = (struct ACPISDTHeader *)(uintptr_t)xsdt->pointers[i];

        if (sig_eq(table->sign, "APIC", 4)) {
            return (struct MADT *)table;
        }
    }

    return NULL;
}

struct MADT* search_MADT_in_RSDT(struct RSDT *rsdt) {
    uint32_t count = (rsdt->header.length - sizeof(struct ACPISDTHeader)) / sizeof(uint64_t);

    for (uint32_t i = 0; i < count; i++) {
        struct ACPISDTHeader *table = (struct ACPISDTHeader *)(uintptr_t)rsdt->pointers[i];

        if (sig_eq(table->sign, "APIC", 4)) {
            return (struct MADT *)table;
        }
    }

    return NULL;
}

struct MADTEntry {
    uint8_t type;
    uint8_t length;
} __attribute__((packed));

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

typedef struct MADT_Stat {
    struct MADTEntry* entry;
    struct MADT_Stat* next;
} MADT_Stat;

MADT_Stat *status;

void add_list_MADT_Storage_stat(MADT_Stat* new_el) {
    MADT_Stat *stat = status;
    uint64_t count;
    while (stat->next) {
        stat = stat->next;
    }
    stat->next = new_el;
}

void parse_madt_entry(struct MADT* madt) {
    status->entry = NULL;
    status->next = NULL;
    uintptr_t current = (uintptr_t)madt + sizeof(struct MADT);
    uintptr_t end = (uintptr_t)current + madt->header.length;

    uintptr_t lapic_paddr = madt->lapicaddress;
    
    while (current < end) {
        struct MADTEntry* entry = (struct MADTEntry *)current;
        MADT_Stat* entry_stat;
        entry_stat->next = NULL;
        switch (entry->type) {
            case 0: {
                struct MADT_LocalAPIC* lapic = (struct MADT_LocalAPIC *)entry;
                if (lapic->flags & 1) {} else continue;
                break;
            }

            case 1: {
                struct MADT_IOAPIC* ioapic = (struct MADT_IOAPIC *)entry;
                break;
            }

            case 2: {
                struct MADT_ISO* isoapic = (struct MADT_ISO *)entry;
                break;
            }

            case 4: {
                struct MADT_LAPIC_NMI* nmiapic = (struct MADT_LAPIC_NMI *)entry;
                break;
            }
        }
        entry_stat->entry = entry;
        add_list_MADT_Storage_stat(entry_stat);
    }
}