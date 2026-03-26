#ifndef KERNEL_STDLIB_H
#define KERNEL_STDLIB_H

#include <stddef.h>
#include <stdint.h>

/* Memory operations */
void* memset(void* s, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);

/* String operations */
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
size_t strlen(const char* s);
char* strcat(char* dest, const char* src);

/* Integer to string conversion */
char* itoa(int value, char* str, int base);
char* uitoa(unsigned int value, char* str, int base);

/* Math utilities */
int abs(int x);
unsigned int udiv(unsigned int a, unsigned int b, unsigned int* remainder);

/* Allocation (simple for now) */
void* malloc(size_t size);
void free(void* ptr);

#endif /* KERNEL_STDLIB_H */
