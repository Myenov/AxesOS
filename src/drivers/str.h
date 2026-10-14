#ifndef STR_H
#define STR_H

#include <stdint.h>
#include <stddef.h>

static inline int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

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

static inline char* ptr_to_str(void *ptr, char *buf) {
    unsigned long long val = (unsigned long long)ptr;
    int num_chars = sizeof(void *) * 2;
    int buf_len = 0;
    while(buf[buf_len]) buf_len++;
    if (buf_len < num_chars + 3) {
        if (buf_len > 0) buf[0] = '\0';
        return NULL;
    }
    const char hex_chars[] = "0123456789abcdef";

    buf[0] = '0';
    buf[1] = 'x';

    for (int i = num_chars - 1; i >= 0; i--) {
        buf[2 + i] = hex_chars[val & 0xF];
        val >>= 4;
    }

    buf[2 + num_chars] = '\0';
    return buf;
}

#endif