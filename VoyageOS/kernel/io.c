#include "io.h"

/* Serial port initialization and output */
#define SERIAL_PORT 0x3F8
#define SERIAL_THR 0
#define SERIAL_LCR 3
#define SERIAL_LSR 5

void serial_init(void) {
    /* Basic serial initialization at 115200 baud */
    outb(SERIAL_PORT + 1, 0x00);    /* Disable all interrupts */
    outb(SERIAL_PORT + 3, 0x80);    /* Enable DLAB */
    outb(SERIAL_PORT + 0, 0x01);    /* Set low byte divisor */
    outb(SERIAL_PORT + 1, 0x00);    /* Set high byte divisor */
    outb(SERIAL_PORT + 3, 0x03);    /* 8 bits, no parity, 1 stop bit */
}

void keyboard_init(void) {
    // Wait for keyboard controller to be ready
    while (inb(KEYBOARD_STATUS_PORT) & 0x02);
    
    // Enable keyboard
    outb(KEYBOARD_STATUS_PORT, 0xAE);
    
    // Wait for keyboard to be ready
    while (inb(KEYBOARD_STATUS_PORT) & 0x02);
    
    // Set keyboard to scan code set 1
    outb(KEYBOARD_DATA_PORT, 0xF0);
    while (inb(KEYBOARD_STATUS_PORT) & 0x02);
    outb(KEYBOARD_DATA_PORT, 0x01);
}

void serial_putchar(char ch) {
    /* Wait for transmit buffer to be empty */
    while ((inb(SERIAL_PORT + SERIAL_LSR) & 0x20) == 0);
    outb(SERIAL_PORT + SERIAL_THR, (uint8_t)ch);
}

void serial_puts(const char* str) {
    while (*str) {
        serial_putchar(*str++);
    }
}

/* Keyboard input */
int keyboard_available(void) {
    return (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_READY) != 0;
}

int keyboard_getchar(void) {
    while (!keyboard_available());
    return inb(KEYBOARD_DATA_PORT);
}

/* Keyboard interrupt handler */
#define KEYBOARD_BUFFER_SIZE 256
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int keyboard_buffer_head = 0;
static int keyboard_buffer_tail = 0;
static int keyboard_interrupt_count = 0;

void keyboard_interrupt(void) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    keyboard_interrupt_count++;
    
    // Handle break codes (key releases) - they have bit 7 set
    if (scancode & 0x80) {
        // Key release - ignore for now
        return;
    }
    
    // Store make codes only
    if ((keyboard_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE != keyboard_buffer_head) {
        keyboard_buffer[keyboard_buffer_tail] = scancode;
        keyboard_buffer_tail = (keyboard_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    }
}

int keyboard_get_scancode(void) {
    if (keyboard_buffer_head != keyboard_buffer_tail) {
        char scancode = keyboard_buffer[keyboard_buffer_head];
        keyboard_buffer_head = (keyboard_buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
        return scancode;
    }
    return -1; // No data
}

/* Scancode to ASCII conversion */
static const char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ', 0
};

char keyboard_get_ascii(void) {
    int scancode = keyboard_get_scancode();
    if (scancode >= 0 && scancode < sizeof(scancode_to_ascii)) {
        return scancode_to_ascii[scancode];
    }
    return 0;
}

int keyboard_get_interrupt_count(void) {
    return keyboard_interrupt_count;
}
