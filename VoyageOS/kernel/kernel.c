#include "vga.h"
#include "init.h"
#include "timer.h"
#include "io.h"

// Forward declaration
extern void ide_main(void);

// VGA driver (same as before)
static uint16_t* vga_buffer = (uint16_t*)0xB8000;

void vga_clear(void) {
    for (int i = 0; i < 80 * 25; i++) {
        vga_buffer[i] = (uint16_t)(' ' | ((COLOR_BLACK << 4) | COLOR_LIGHT_GREY) << 8);
    }
}

void vga_print(const char* str, uint8_t color) {
    uint16_t color_code = (color << 8);
    while (*str) {
        if (*str == '\n') {
            // Very basic newline
            static int row = 0;
            row = (row + 1) % 25;
            vga_buffer = (uint16_t*)0xB8000 + (row * 80);
            str++;
            continue;
        }
        *vga_buffer++ = (uint16_t)(*str | color_code);
        str++;
    }
}

// Exception handler - called from ISR
void exception_handler(uint64_t int_number, uint64_t error_code) {
    if (int_number >= 32) {
        // Hardware interrupt
        if (int_number == 32) {
            // Timer interrupt
            extern void timer_interrupt(void);
            timer_interrupt();
        } else if (int_number == 33) {
            // Keyboard interrupt
            extern void keyboard_interrupt(void);
            keyboard_interrupt();
        }
        // Send EOI to PIC
        outb(0x20, 0x20); // Master PIC
        if (int_number >= 40) {
            outb(0xA0, 0x20); // Slave PIC
        }
    } else {
        // Exception
        uint16_t* vga = (uint16_t*)0xB8000;
        uint8_t error_color = (COLOR_RED << 4) | COLOR_WHITE;
        
        // Write at position 79 (top right) to indicate exception
        vga[79] = ('E' | (error_color << 8));
    }
}

void kmain(void) {
    /* Initialize kernel systems */
    kernel_init();
    
    /* Enable interrupts for timer */
    __asm__("sti");

    /* Show IDE loading animation */
    uint8_t info = (COLOR_BLACK << 4) | COLOR_GREEN;
    vga_print("kernel: Initializing VoyageIDE...\n", info);
    
    vga_print("kernel: Launching VoyageIDE...\n\n", info);

    ide_main();                 // Auto start IDE

    while (1) { __asm__("hlt"); }
}