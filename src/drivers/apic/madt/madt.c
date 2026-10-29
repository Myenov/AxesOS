#include "../../../lib/apic/madt/madt.h"
#include "../../../lib/apic/xsdt/xsdt.h"
#include "../../../lib/signature/signature_search.h"
#include <stdint.h>
#include <stddef.h>
#include "../../../lib/memory/alloc.h"

MADT_Stat *status = NULL; 

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
    uint32_t count = (rsdt->header.length - sizeof(struct ACPISDTHeader)) / sizeof(uint32_t);

    for (uint32_t i = 0; i < count; i++) {
        struct ACPISDTHeader *table = (struct ACPISDTHeader *)(uintptr_t)rsdt->pointers[i];

        if (sig_eq(table->sign, "APIC", 4)) {
            return (struct MADT *)table;
        }
    }

    return NULL;
}

void add_list_MADT_Storage_stat(MADT_Stat* new_el) {
    new_el->next = NULL;
    if (status == NULL) {
        status = new_el;
        return;
    }
    MADT_Stat *stat = status;
    while (stat->next) stat = stat->next;
    stat->next = new_el;
}

void parse_madt_entry(struct MADT* madt) {
    status = NULL;
    uintptr_t current = (uintptr_t)madt + sizeof(struct MADT);
    uintptr_t end = (uintptr_t)madt + madt->header.length;

    uintptr_t lapic_paddr = madt->lapicaddress;
    
    while (current < end) {
        struct MADTEntry* entry = (struct MADTEntry *)current;
        current += entry->length;
        MADT_Stat* entry_stat = (MADT_Stat*)malloc(sizeof(MADT_Stat));
        if (!entry_stat) return;
        entry_stat->next = NULL;
        switch (entry->type) {
            case 0: {
                struct MADT_LocalAPIC* lapic = (struct MADT_LocalAPIC *)entry;
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