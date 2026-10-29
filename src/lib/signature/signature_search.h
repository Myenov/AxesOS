#ifndef SEARCH_SIGNATURE_H
#define SEARCH_SIGNATURE_H

#include "stdbool.h"
#include "stddef.h"
static inline int sig_eq(const char *a, const char *b, size_t n) {
    for (size_t i = 0; i < n; i++)
        if (a[i] != b[i]) return 0;
    return 1;
}

unsigned char *find_sign(unsigned char *address, size_t size, const unsigned char *bmask,const char *szmask);

#endif