#ifndef IO_H
#define IO_H

#include <stdint.h>

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void serial_init() {
    outb(0x3f8 + 1, 0x00);    // Disable interrupts
    outb(0x3f8 + 3, 0x80);    // Enable DLAB
    outb(0x3f8 + 0, 0x03);    // Set divisor low
    outb(0x3f8 + 1, 0x00);    // Set divisor high
    outb(0x3f8 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(0x3f8 + 2, 0xC7);    // Enable FIFO
    outb(0x3f8 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

static inline void serial_putc(char c) {
    while ((inb(0x3f8 + 5) & 0x20) == 0);
    outb(0x3f8, c);
}

#endif