#include "vga.h"

void compile(const char* code) {
    // Simple parser for print statements
    const char* ptr = code;
    while (*ptr) {
        if (strncmp(ptr, "print ", 6) == 0) {
            ptr += 6;
            const char* start = ptr;
            while (*ptr && *ptr != '\n' && *ptr != ';') ptr++;
            // For simplicity, assume the rest is the string
            vga_puts(start);
            vga_putc('\n');
            break;
        } else {
            ptr++;
        }
    }
    if (!*code) {
        vga_puts("No code to compile\n");
    }
}