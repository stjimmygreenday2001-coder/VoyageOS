#include "string.h"

int strcmp(const char* a, const char* b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a - *b;
}

int strncmp(const char* a, const char* b, size_t n) {
    while (n-- > 0 && *a && *b && *a == *b) { a++; b++; }
    if (n < 0) return 0;
    return *a - *b;
}

void* memcpy(void* dest, const void* src, size_t n) {
    char* d = dest;
    const char* s = src;
    while (n--) *d++ = *s++;
    return dest;
}