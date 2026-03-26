#include "init.h"
#include "vga.h"
#include "io.h"
#include "timer.h"
#include "stdlib.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 2
#define VERSION_PATCH 0
#define BUILD_DATE __DATE__

void kernel_version(void) {
    uint8_t color = (COLOR_BLUE << 4) | COLOR_WHITE;
    char version_str[64];
    strcpy(version_str, "VoyageOS v");
    itoa(VERSION_MAJOR, version_str + strlen(version_str), 10);
    strcat(version_str, ".");
    itoa(VERSION_MINOR, version_str + strlen(version_str), 10);
    strcat(version_str, ".");
    itoa(VERSION_PATCH, version_str + strlen(version_str), 10);
    vga_print(version_str, color);
}

void pic_init(void) {
    // ICW1: Start initialization
    outb(0x20, 0x11); // Master PIC
    outb(0xA0, 0x11); // Slave PIC
    
    // ICW2: Interrupt vector offsets
    outb(0x21, 0x20); // Master PIC vectors 32-39
    outb(0xA1, 0x28); // Slave PIC vectors 40-47
    
    // ICW3: Cascade
    outb(0x21, 0x04); // Tell master there's a slave at IRQ2
    outb(0xA1, 0x02); // Tell slave its cascade identity
    
    // ICW4: 8086 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    
    // Mask all interrupts initially
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
    
    // Unmask timer (IRQ 0) and keyboard (IRQ 1)
    outb(0x21, 0xFC); // Unmask IRQ 0 and 1 (bits 0 and 1 set to 0)
}

void kernel_banner(void) {
    uint8_t title_color = (COLOR_BLUE << 4) | COLOR_WHITE;
    uint8_t text_color = (COLOR_BLACK << 4) | COLOR_LIGHT_GREY;
    
    vga_clear();
    vga_print("\n", text_color);
    vga_print("╔════════════════════════════════════════════════════════════════════╗\n", title_color);
    vga_print("║                                                                    ║\n", title_color);
    vga_print("║                      Welcome to VoyageOS                           ║\n", title_color);
    vga_print("║                   A Journey Through Code                           ║\n", title_color);
    vga_print("║                                                                    ║\n", title_color);
    vga_print("╚════════════════════════════════════════════════════════════════════╝\n", title_color);
    vga_print("\n", text_color);
}

void kernel_init(void) {
    uint8_t info_color = (COLOR_BLACK << 4) | COLOR_GREEN;
    uint8_t warn_color = (COLOR_BLACK << 4) | COLOR_YELLOW;
    
    /* Display banner */
    kernel_banner();
    
    /* Initialize hardware */
    vga_print("Initializing hardware...\n", info_color);
    
    /* Initialize I/O */
    serial_init();
    vga_print("  ✓ Serial I/O initialized\n", info_color);
    
    keyboard_init();
    vga_print("  ✓ Keyboard initialized\n", info_color);
    
    /* Initialize timer */
    timer_init(1000);  /* 1000 Hz timer */
    vga_print("  ✓ System timer initialized\n", info_color);
    
    /* Initialize PIC */
    pic_init();
    vga_print("  ✓ Interrupt controller initialized\n", info_color);
    
    /* Memory status */
    vga_print("\nSystem status:\n", info_color);
    vga_print("  • 64-bit long mode enabled\n", info_color);
    vga_print("  • VGA console ready\n", info_color);
    vga_print("  • Exception handling active\n", info_color);
    vga_print("  • Standard library available\n", info_color);
    
    vga_print("\n", warn_color);
    kernel_version();
    vga_print(" - Ready for development\n", warn_color);
}
