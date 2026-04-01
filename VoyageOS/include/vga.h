#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void vga_init();
void vga_putc(char c);
void vga_puts(const char* str);

#endif