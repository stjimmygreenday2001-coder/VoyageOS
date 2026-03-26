#ifndef KERNEL_IO_H
#define KERNEL_IO_H

#include <stdint.h>

/* I/O Port functions */
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__("in %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void outb(uint16_t port, uint8_t value) {
    __asm__("out %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t result;
    __asm__("in %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void outw(uint16_t port, uint16_t value) {
    __asm__("out %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint32_t inl(uint32_t port) {
    uint32_t result;
    __asm__("in %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void outl(uint32_t port, uint32_t value) {
    __asm__("out %0, %1" : : "a"(value), "Nd"(port));
}

/* Keyboard input */
#define KEYBOARD_DATA_PORT       0x60
#define KEYBOARD_STATUS_PORT     0x64
#define KEYBOARD_READY           0x01

int keyboard_getchar(void);
int keyboard_available(void);
int keyboard_get_scancode(void);
char keyboard_get_ascii(void);
int keyboard_get_interrupt_count(void);

/* Serial output (for debugging) */
void serial_init(void);
void serial_putchar(char ch);
void serial_puts(const char* str);

/* Keyboard initialization */
void keyboard_init(void);

#endif /* KERNEL_IO_H */
