#ifndef STRING_H
#define STRING_H

#include <stddef.h>

int strcmp(const char* a, const char* b);

int strncmp(const char* a, const char* b, size_t n);

void* memcpy(void* dest, const void* src, size_t n);

#endif