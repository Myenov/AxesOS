#include "headers_search.h"
#include "rsdp/rsdp.h"

struct RSDPDescriptor20* search_ACPIHeaders(void* rsdp_address) {
    return (struct RSDPDescriptor20*)rsdp_address;
}