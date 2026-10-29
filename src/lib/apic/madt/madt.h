#ifndef MADT_STRUCT
#define MADT_STRUCT

#include "../../../drivers/apic/ACPI.h"
#include <stdint.h>

struct MADT {
  struct ACPISDTHeader header;
  uint32_t lapicaddress;
  uint32_t flags;
};
#endif