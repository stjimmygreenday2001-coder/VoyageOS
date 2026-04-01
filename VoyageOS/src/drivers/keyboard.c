#include "io.h"
#include "drivers.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

char scancode_to_ascii[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboard_init() {
    // Enable keyboard
    outb(KEYBOARD_STATUS_PORT, 0xAE);
}

char keyboard_getchar() {
    while ((inb(KEYBOARD_STATUS_PORT) & 1) == 0);
    unsigned char scancode = inb(KEYBOARD_DATA_PORT);
    if (scancode & 0x80) return 0; // Key release
    return scancode_to_ascii[scancode];
}