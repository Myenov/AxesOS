#include "../ACPI.h"
#include "../../../lib/apic/xsdt/xsdt.h"
#include "../../../lib/signature/signature_search.h"
#include <stdint.h>
#include "../../../lib/apic/hpet/hpet.h"
#include <stddef.h>

int sig_eq(const char *a, const char *b, size_t n) {
    for (size_t i = 0; i < n; i++)
        if (a[i] != b[i]) return 0;
    return 1;
}

struct HPETTable* search_HPET_table_in_XSDT(struct XSDT *xsdt) {
    uint64_t count = (xsdt->header.length - sizeof(struct ACPISDTHeader)) / sizeof(uint64_t);

    for (uint64_t i = 0; i < count; i++) {
        struct ACPISDTHeader *table = (struct ACPISDTHeader *)(uintptr_t)xsdt->pointers[i];

        if (sig_eq(table->sign, "HPET", 4)) {
            return (struct HPETTable *)table;
        }
    }

    return NULL;
}

struct HPETTable* search_HPET_table_in_RSDT(struct RSDT *rsdt) {
    uint32_t count = (rsdt->header.length - sizeof(struct ACPISDTHeader)) / sizeof(uint32_t);

    for (uint32_t i = 0; i < count; i++) {
        struct ACPISDTHeader *table = (struct ACPISDTHeader *)(uintptr_t)rsdt->pointers[i];

        if (sig_eq(table->sign, "HPET", 4)) {
            return (struct HPETTable *)table;
        }
    }

    return NULL;
}