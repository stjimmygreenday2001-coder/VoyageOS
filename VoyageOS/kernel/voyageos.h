#ifndef VOYAGEOS_H
#define VOYAGEOS_H

/*
 * VoyageOS Master Header
 * Include this to get access to all kernel and OS features
 */

#include <stdint.h>
#include <stddef.h>

/* === Bootloader Features === */
/* Loading bar animation: 5-stage progress display during boot (5 seconds) */

/* === Console & Display === */
#include "vga.h"
/* Functions: vga_clear(), vga_print(str, color) */
/* Colors: 16 standard colors (black, red, green, blue, cyan, magenta, yellow, white, + bright variants) */

/* === Standard C Library === */
#include "stdlib.h"
/* Memory: memset, memcpy, memcmp */
/* Strings: strlen, strcpy, strncpy, strcmp, strncmp, strcat */
/* Convert: itoa, uitoa */
/* Math: abs, udiv */
/* Alloc: malloc (65KB heap), free */

/* === I/O & Hardware === */
#include "io.h"
/* Ports: inb, outb, inw, outw, inl, outl */
/* Keyboard: keyboard_available(), keyboard_getchar() */
/* Serial: serial_init(), serial_putchar(ch), serial_puts(str) */
/* Baud: 115200 8N1 */

/* === System Timer === */
#include "timer.h"
/* Init: timer_init(freq) - default 1000 Hz */
/* Time: timer_ticks(), timer_milliseconds() */
/* Sleep: timer_sleep_ms(), timer_sleep_ticks() */

/* === Kernel Initialization === */
#include "init.h"
/* Calls: kernel_init() - initializes all subsystems */
/* Display: kernel_banner(), kernel_version() */

/* === Boot Information === */
#include "boot.h"
/* Structures for Multiboot-compatible boot info */

/* === Memory Management (Foundation) === */
#include "memory.h"
/* Structures for virtual memory and page management */

/* === System Information === */
#include "system.h"
/* CPU detection and system info queries */

/* === Exception Handling === */
/* ISRs: 32+ exception handlers (entry.asm, isr.asm) */
/* Handler: exception_handler(int_number, error_code) */

/* === IDE & Shell === */
/* Integrated development environment for on-board coding */
/* Auto-launches after kernel boot */

/* === Quick Usage Examples === */

/*
 * Example 1: Hello World
 * 
 * #include "voyageos.h"
 * 
 * void main() {
 *     uint8_t color = (COLOR_BLACK << 4) | COLOR_GREEN;
 *     vga_print("Hello from VoyageOS!\n", color);
 * }
 */

/*
 * Example 2: Using Standard Library
 * 
 * #include "voyageos.h"
 * 
 * void demo() {
 *     char buf[64];
 *     itoa(12345, buf, 10);
 *     vga_print("Number: ", COLOR_WHITE);
 *     vga_print(buf, COLOR_CYAN);
 * }
 */

/*
 * Example 3: I/O Operations
 * 
 * #include "voyageos.h"
 * 
 * void serial_debug() {
 *     serial_init();
 *     serial_puts("Debug output on COM1\n");
 * }
 */

/*
 * Example 4: Using Timer
 * 
 * #include "voyageos.h"
 * 
 * void timed_wait() {
 *     timer_init(1000);  // 1000 Hz
 *     timer_sleep_ms(5000);  // Wait 5 seconds
 *     vga_print("Done!\n", COLOR_WHITE);
 * }
 */

/*
 * Example 5: Memory Operations
 * 
 * #include "voyageos.h"
 * 
 * void memory_demo() {
 *     char buf[32];
 *     memset(buf, 0, 32);  // Clear
 *     strcpy(buf, "VoyageOS");
 *     vga_print(buf, COLOR_GREEN);
 * }
 */

/* === Feature Summary === */
#define VOYAGEOS_VERSION "0.2"
#define VOYAGEOS_ARCH "x86-64 (64-bit long mode)"
#define VOYAGEOS_BOOTFORMAT "512-byte MBR with loading bar"

/* === What's Included === */
/* ✓ 64-bit bootloader with animation */
/* ✓ VGA text console 80x25 */
/* ✓ Complete C standard library */
/* ✓ I/O port operations */
/* ✓ Keyboard input handling */
/* ✓ Serial debug port (115200 baud) */
/* ✓ System timer/PIT (1000 Hz) */
/* ✓ Exception/interrupt handling */
/* ✓ 65KB heap allocator */
/* ✓ Boot information structures */
/* ✓ Memory management foundation */
/* ✓ IDE shell environment */

/* === Ready For === */
/* • Device driver development */
/* • Interrupt handling */
/* • Task scheduling */
/* • File system implementation */
/* • Memory management expansion */
/* • Process management */

#endif /* VOYAGEOS_H */
