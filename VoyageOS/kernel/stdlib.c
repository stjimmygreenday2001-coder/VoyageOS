#include "stdlib.h"

/* Memory operations */
void* memset(void* s, int c, size_t n) {
    uint8_t* p = (uint8_t*)s;
    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }
    return s;
}

void* memcpy(void* dest, const void* src, size_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

int memcmp(const void* s1, const void* s2, size_t n) {
    const uint8_t* a = (const uint8_t*)s1;
    const uint8_t* b = (const uint8_t*)s2;
    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i]) return a[i] - b[i];
    }
    return 0;
}

/* String operations */
char* strcpy(char* dest, const char* src) {
    char* d = dest;
    while (*src) {
        *d++ = *src++;
    }
    *d = '\0';
    return dest;
}

char* strncpy(char* dest, const char* src, size_t n) {
    for (size_t i = 0; i < n && src[i]; i++) {
        dest[i] = src[i];
    }
    return dest;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (s1[i] != s2[i] || !s1[i]) {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
    }
    return 0;
}

size_t strlen(const char* s) {
    size_t len = 0;
    while (s[len]) len++;
    return len;
}

char* strcat(char* dest, const char* src) {
    char* d = dest;
    while (*d) d++;
    while (*src) {
        *d++ = *src++;
    }
    *d = '\0';
    return dest;
}

/* Integer to string conversion */
char* itoa(int value, char* str, int base) {
    if (base < 2 || base > 36) return str;
    
    char* p = str;
    int sign = 0;
    
    if (value < 0 && base == 10) {
        sign = 1;
        value = -value;
    }
    
    unsigned int uval = (unsigned int)value;
    int len = 0;
    unsigned int temp = uval;
    
    do {
        len++;
        temp /= base;
    } while (temp);
    
    p += len + sign;
    *p = '\0';
    
    temp = uval;
    do {
        int digit = temp % base;
        *--p = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        temp /= base;
    } while (temp);
    
    if (sign) {
        *--p = '-';
    }
    
    return str;
}

char* uitoa(unsigned int value, char* str, int base) {
    if (base < 2 || base > 36) return str;
    
    char* p = str;
    int len = 0;
    unsigned int temp = value;
    
    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }
    
    do {
        len++;
        temp /= base;
    } while (temp);
    
    p += len;
    *p = '\0';
    
    temp = value;
    do {
        int digit = temp % base;
        *--p = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        temp /= base;
    } while (temp);
    
    return str;
}

/* Math utilities */
int abs(int x) {
    return (x < 0) ? -x : x;
}

unsigned int udiv(unsigned int a, unsigned int b, unsigned int* remainder) {
    if (remainder) *remainder = a % b;
    return a / b;
}

/* Simple heap allocator */
#define HEAP_SIZE 65536
static uint8_t heap[HEAP_SIZE];
static size_t heap_ptr = 0;

void* malloc(size_t size) {
    if (heap_ptr + size > HEAP_SIZE) return NULL;
    void* ptr = &heap[heap_ptr];
    heap_ptr += size;
    return ptr;
}

void free(void* ptr) {
    /* Simple allocator - no free for now */
    (void)ptr;
}
