#ifndef RSDT_H
#define RSDT_H

#include <stdint.h>

struct RSDPDescriptor {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed));

struct RSDPDescriptor20 {
    struct RSDPDescriptor firstPart;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

#endif