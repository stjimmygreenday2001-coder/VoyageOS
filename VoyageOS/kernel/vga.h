#ifndef VGA_H
#define VGA_H

#include <stdint.h>

/* VGA Buffer Constants */
#define VGA_BUFFER_ADDRESS  0xB8000
#define VGA_WIDTH           80
#define VGA_HEIGHT          25
#define VGA_BUFFER_SIZE     (VGA_WIDTH * VGA_HEIGHT)

/* VGA Color Palette (8 standard colors) */
enum vga_color {
    COLOR_BLACK         = 0,
    COLOR_BLUE          = 1,
    COLOR_GREEN         = 2,
    COLOR_CYAN          = 3,
    COLOR_RED           = 4,
    COLOR_MAGENTA       = 5,
    COLOR_BROWN         = 6,
    COLOR_LIGHT_GREY    = 7,
    COLOR_DARK_GREY     = 8,
    COLOR_LIGHT_BLUE    = 9,
    COLOR_LIGHT_GREEN   = 10,
    COLOR_LIGHT_CYAN    = 11,
    COLOR_LIGHT_RED     = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_YELLOW        = 14,
    COLOR_WHITE         = 15,
};

/* VGA Character Entry Structure */
typedef struct {
    uint8_t character;
    uint8_t color;
} vga_entry_t;

/* VGA Character and Attribute (word format) */
#define VGA_ENTRY(char, fg, bg) \
    ((uint16_t)(char) | ((uint16_t)(fg) << 8) | ((uint16_t)(bg) << 12))

/* Utility Macros */
#define VGA_MAKE_COLOR(fg, bg)  (((bg) << 4) | (fg))
#define VGA_ENTRY_COLOR(fg, bg) ((bg) << 4 | (fg))

/* Function Declarations */

/**
 * Clear the VGA screen and fill with spaces
 */
void vga_clear(void);

/**
 * Print a string to the VGA buffer with specified color
 * @param str   - String to print
 * @param color - Color byte (background in high nibble, foreground in low nibble)
 */
void vga_print(const char* str, uint8_t color);

/**
 * Print a character at the current cursor position
 * @param ch    - Character to print
 * @param color - Color byte
 */
void vga_putchar(char ch, uint8_t color);

/**
 * Set the cursor position
 * @param row - Row (0-24)
 * @param col - Column (0-79)
 */
void vga_set_cursor(uint8_t row, uint8_t col);

/**
 * Get the current cursor position
 * @param row - Pointer to store row position
 * @param col - Pointer to store column position
 */
void vga_get_cursor(uint8_t* row, uint8_t* col);

/**
 * Write a character at a specific position
 * @param row - Row (0-24)
 * @param col - Column (0-79)
 * @param ch  - Character to write
 * @param color - Color byte
 */
void vga_write_at(uint8_t row, uint8_t col, char ch, uint8_t color);

#endif /* VGA_H */
