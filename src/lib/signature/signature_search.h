#ifndef SEARCH_SIGNATURE_H
#define SEARCH_SIGNATURE_H

#include "stdbool.h"
#include "stddef.h"

unsigned char *find_sign(unsigned char *address, size_t size, const unsigned char *bmask,const char *szmask);

#endif