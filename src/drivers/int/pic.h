#ifndef PIC_INIT_FILE_H
#define PIC_INIT_FILE_H

#include <stdint.h>

void init_pic();
static inline void outb(uint16_t port, uint8_t value);
static inline uint8_t inb(uint16_t port);

#endif