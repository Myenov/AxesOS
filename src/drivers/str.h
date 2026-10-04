#ifndef STR_H
#define STR_H

#include <stdint.h>

static inline void reverse_string(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

static inline char* uint64_to_str(uint64_t num, char* buf) {
    int i = 0;
    if (num == 0) {
        buf[i++] = '0';
        buf[i] = '\0';
        return buf;
    }

    while (num > 0) {
        buf[i++] = (char)((num % 10) + '0'); 
        num /= 10;                           
    }

    buf[i] = '\0';
    reverse_string(buf, i);

    return buf;
}

#endif