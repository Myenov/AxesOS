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

static inline int int_to_str(long val, char* str, int d) {
    int i = 0;
    while (val) {
        str[i++] = (val % 10) + '0';
        val /= 10;
    }
    if (i == 0) {
        str[i++] = '0';
    }
    while (i < d) {
        str[i++] = '0';
    }
    reverse_string(str, i);
    str[i] = '\0';
    return i;
}


static inline char* float_to_char(float n, char* buf, int afterpoint) {
    int i = 0;
    if (n < 0.0f) {
        buf[i++] = '-';
        n = -n;
    }

    long ipart = (long)n;
    float fpart = n - (float)ipart;

    char int_str[20];
    int int_len = int_to_str(ipart, int_str, 0);
    for (int j = 0; j < int_len; j++) {
        buf[i++] = int_str[j];
    }

    if (afterpoint != 0) {
        buf[i++] = '.';
        long mult = 1;
        for (int j = 0; j < afterpoint; j++) {
            mult *= 10;
        }

        long dpart = (long)(fpart * mult + 0.5f);
        char frac_str[20];
        int frac_len = int_to_str(dpart, frac_str, afterpoint);
        for (int j = 0; j < frac_len; j++) {
            buf[i++] = frac_str[j];
        }
    }
    buf[i] = '\0';
    return buf;
}
#endif