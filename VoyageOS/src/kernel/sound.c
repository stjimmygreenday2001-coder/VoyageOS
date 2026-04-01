#include "io.h"

void sound_beep() {
    outb(0x43, 0xb6);
    outb(0x42, 0x04);
    outb(0x42, 0x00);
    uint8_t tmp = inb(0x61);
    outb(0x61, tmp | 3);
    // Simple delay
    for (volatile int i = 0; i < 100000; i++);
    outb(0x61, tmp);
}