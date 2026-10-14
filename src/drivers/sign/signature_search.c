#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"


unsigned char *find_sign(unsigned char *address, size_t size, const unsigned char *bmask,const char *szmask) {
    size_t mask_length = 0;
    while (szmask[mask_length]) mask_length++;

    if (size < mask_length) return NULL;

    for (size_t i = 0; i <= size - mask_length; i++) {
        bool ok = true;
        for (size_t j = 0; j < mask_length; j++) {
            if (szmask[j] == 'x' && address[i + j] != bmask[j]) { ok = false; break; }
        }
        if (ok) return address + i;
    }
    return NULL;
}