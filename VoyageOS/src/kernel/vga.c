#include "vga.h"
#include "io.h"

static uint16_t* vga_buffer = (uint16_t*) VGA_ADDRESS;
static int vga_row = 0;
static int vga_column = 0;

void vga_init() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = (uint16_t) ' ' | (uint16_t) 0x0F << 8;
        }
    }
    vga_row = 0;
    vga_column = 0;
}

void vga_putc(char c) {
    if (c == '\n') {
        vga_row++;
        vga_column = 0;
        if (vga_row >= VGA_HEIGHT) {
            vga_row = 0;
        }
        return;
    }
    vga_buffer[vga_row * VGA_WIDTH + vga_column] = (uint16_t) c | (uint16_t) 0x0F << 8;
    vga_column++;
    if (vga_column >= VGA_WIDTH) {
        vga_row++;
        vga_column = 0;
        if (vga_row >= VGA_HEIGHT) {
            vga_row = 0;
        }
    }
}

void vga_puts(const char* str) {
    while (*str) {
        vga_putc(*str);
        serial_putc(*str);
        str++;
    }
}