#ifndef HEADERS_SEARCH_H
#define HEADERS_SEARCH_H

#include "rsdp/rsdp.h"
#include <stdint.h>

struct RSDPDescriptor20* search_ACPIHeaders(void* rsdp_address);

#endif