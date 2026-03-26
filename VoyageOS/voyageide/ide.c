/*
 * VoyageIDE - The VoyageOS Integrated Development Environment
 * 
 * A self-hosted IDE that runs within VoyageOS, allowing developers
 * to write and compile code directly on the OS.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../kernel/io.h"
#include "../kernel/vga.h"
#include "../kernel/stdlib.h"

/* Forward declarations */
void ide_draw_editor(void);
void ide_update_cursor(void);

/* Screen definitions */
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define VGA_ADDRESS 0xB8000

volatile uint8_t *screen = (uint8_t *)VGA_ADDRESS;

/* Color definitions */
#define COLOR_BLACK     0x00
#define COLOR_BLUE      0x01
#define COLOR_GREEN     0x02
#define COLOR_CYAN      0x03
#define COLOR_RED       0x04
#define COLOR_MAGENTA   0x05
#define COLOR_BROWN     0x06
#define COLOR_WHITE     0x07
#define COLOR_GRAY      0x08
#define COLOR_LIGHT_BLUE    0x09
#define COLOR_LIGHT_GREEN   0x0A
#define COLOR_LIGHT_CYAN    0x0B
#define COLOR_LIGHT_RED     0x0C
#define COLOR_LIGHT_MAGENTA 0x0D
#define COLOR_YELLOW    0x0E
#define COLOR_BRIGHT_WHITE  0x0F

/* Editor buffer */
#define MAX_BUFFER_SIZE 8192
#define MAX_LINES 512

typedef struct {
    char *lines[MAX_LINES];
    int line_count;
    int cursor_x;
    int cursor_y;
    int scroll_offset;
    char filename[256];
} Editor;

Editor editor;

/* IDE state */
typedef enum {
    IDE_EDITOR,
    IDE_REPL,
    IDE_COMPILER,
    IDE_MENU
} IDEMode;

IDEMode current_mode = IDE_EDITOR;

/* VGA operations */
void vid_put_char(int x, int y, char c, uint8_t color) {
    int offset = (y * SCREEN_WIDTH + x) * 2;
    screen[offset] = c;
    screen[offset + 1] = color;
}

void vid_clear(void) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 2; i += 2) {
        screen[i] = ' ';
        screen[i + 1] = (COLOR_BLACK << 4) | COLOR_WHITE;
    }
}

void vid_print(int x, int y, const char *str, uint8_t color) {
    int offset = (y * SCREEN_WIDTH + x) * 2;
    while (*str && offset < SCREEN_WIDTH * SCREEN_HEIGHT * 2) {
        screen[offset] = *str++;
        screen[offset + 1] = color;
        offset += 2;
    }
}

void vid_draw_box(int x, int y, int w, int h, uint8_t color) {
    // Top border
    for (int i = 0; i < w; i++) {
        vid_put_char(x + i, y, '-', color);
    }
    // Bottom border
    for (int i = 0; i < w; i++) {
        vid_put_char(x + i, y + h - 1, '-', color);
    }
    // Left and right borders
    for (int i = 1; i < h - 1; i++) {
        vid_put_char(x, y + i, '|', color);
        vid_put_char(x + w - 1, y + i, '|', color);
    }
    // Corners
    vid_put_char(x, y, '+', color);
    vid_put_char(x + w - 1, y, '+', color);
    vid_put_char(x, y + h - 1, '+', color);
    vid_put_char(x + w - 1, y + h - 1, '+', color);
}

/* IDE Functions */
void ide_update_cursor(void) {
    // Update hardware cursor position
    int cursor_pos = (editor.cursor_y - editor.scroll_offset + 3) * SCREEN_WIDTH + editor.cursor_x + 2;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(cursor_pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((cursor_pos >> 8) & 0xFF));
}

void ide_draw_editor(void) {
    /* Display editor buffer */
    int display_line = 3;
    
    for (int i = editor.scroll_offset; i < editor.line_count && display_line < SCREEN_HEIGHT - 1; i++) {
        if (editor.lines[i]) {
            vid_print(2, display_line, editor.lines[i], 
                      (COLOR_BLACK << 4) | COLOR_WHITE);
        }
        display_line++;
    }
}

void ide_init(void) {
    vid_clear();
    editor.line_count = 0;
    editor.cursor_x = 0;
    editor.cursor_y = 0;
    editor.scroll_offset = 0;
    
    vid_print(0, 0, "VoyageIDE v0.1 - Integrated Development Environment", 
              (COLOR_BLUE << 4) | COLOR_WHITE);
    vid_print(0, 1, "New file | F1:Help | F2:Save | F3:Compile | F4:REPL | ESC:Menu", 
              (COLOR_BLACK << 4) | COLOR_GRAY);
    
    /* Draw main editor area */
    vid_draw_box(0, 2, SCREEN_WIDTH, SCREEN_HEIGHT - 4, 
                 (COLOR_WHITE << 4) | COLOR_BLACK);
    
    /* Draw bottom status bar */
    char status[80];
    char num_buf[16];
    
    strcpy(status, "Ready | VoyageOS | Line: ");
    itoa(editor.cursor_y + 1, num_buf, 10);
    strcat(status, num_buf);
    strcat(status, " Col: ");
    itoa(editor.cursor_x + 1, num_buf, 10);
    strcat(status, num_buf);
    strcat(status, " | Int: ");
    itoa(keyboard_get_interrupt_count(), num_buf, 10);
    strcat(status, num_buf);
    
    vid_print(0, SCREEN_HEIGHT - 1, status, 
              (COLOR_WHITE << 4) | COLOR_GREEN);
              
    ide_update_cursor();
}

void ide_show_menu(void) {
    uint8_t menu_color = (COLOR_BLUE << 4) | COLOR_WHITE;
    
    vid_clear();
    
    vid_print(20, 5, "╔═══════════════════════════╗", menu_color);
    vid_print(20, 6, "║   VoyageIDE Main Menu     ║", menu_color);
    vid_print(20, 7, "╠═══════════════════════════╣", menu_color);
    vid_print(20, 8, "║ 1. New File               ║", menu_color);
    vid_print(20, 9, "║ 2. Open File              ║", menu_color);
    vid_print(20, 10, "║ 3. Compile Code           ║", menu_color);
    vid_print(20, 11, "║ 4. REPL (Read-Eval-Print)║", menu_color);
    vid_print(20, 12, "║ 5. System Info            ║", menu_color);
    vid_print(20, 13, "║ 6. Kernel Debug           ║", menu_color);
    vid_print(20, 14, "║ Q. Quit to Shell          ║", menu_color);
    vid_print(20, 15, "╚═══════════════════════════╝", menu_color);
    
    vid_print(25, 18, "Select option (1-6, Q):", (COLOR_WHITE << 4) | COLOR_BLACK);
}

void ide_show_repl(void) {
    uint8_t repl_color = (COLOR_GREEN << 4) | COLOR_BLACK;
    
    vid_clear();
    
    vid_print(0, 0, "VoyageOS REPL - Interactive Code Evaluation", 
              (COLOR_BLUE << 4) | COLOR_WHITE);
    vid_print(0, 1, "Type Voyage Code, press Enter to evaluate. 'exit' to return.", 
              (COLOR_BLACK << 4) | COLOR_GRAY);
    
    vid_print(0, 2, ">>> ", repl_color);
    
    /* REPL would continue here with input handling */
}

void ide_show_compiler(void) {
    uint8_t compile_color = (COLOR_CYAN << 4) | COLOR_BLACK;
    
    vid_clear();
    
    vid_print(0, 0, "VoyageC Compiler", (COLOR_BLUE << 4) | COLOR_WHITE);
    vid_print(0, 1, "", (COLOR_BLACK << 4) | COLOR_GRAY);
    
    vid_print(2, 3, "Compiling...", compile_color);
    vid_print(2, 5, "✓ Lexical analysis", (COLOR_BLACK << 4) | COLOR_GREEN);
    vid_print(2, 6, "✓ Parse tree generation", (COLOR_BLACK << 4) | COLOR_GREEN);
    vid_print(2, 7, "⊘ Code generation", (COLOR_BLACK << 4) | COLOR_GRAY);
    vid_print(2, 8, "⊘ Assembly generation", (COLOR_BLACK << 4) | COLOR_GRAY);
    vid_print(2, 9, "⊘ Module linking", (COLOR_BLACK << 4) | COLOR_GRAY);
    
    vid_print(2, 11, "Status: In Progress (demo)", (COLOR_BLACK << 4) | COLOR_YELLOW);
}

/* Main IDE loop */
void ide_main(void) {
    ide_init();
    ide_draw_editor();
    
    /* Main input loop */
    while (1) {
        char ch = keyboard_get_ascii();
        if (ch) {
            if (ch == '\n') {
                // Handle enter - move to next line
                if (editor.cursor_y < MAX_LINES - 1) {
                    editor.cursor_y++;
                    editor.cursor_x = 0;
                    if (editor.cursor_y >= editor.line_count) {
                        editor.line_count = editor.cursor_y + 1;
                    }
                }
            } else if (ch == 8) { // Backspace
                if (editor.cursor_x > 0) {
                    editor.cursor_x--;
                } else if (editor.cursor_y > 0) {
                    editor.cursor_y--;
                    // Find end of previous line
                    if (editor.lines[editor.cursor_y]) {
                        editor.cursor_x = strlen(editor.lines[editor.cursor_y]);
                    } else {
                        editor.cursor_x = 0;
                    }
                }
            } else if (ch >= 32 && ch <= 126) { // Printable characters
                // Add character to current line
                if (!editor.lines[editor.cursor_y]) {
                    editor.lines[editor.cursor_y] = (char*)malloc(256);
                    if (editor.lines[editor.cursor_y]) {
                        memset(editor.lines[editor.cursor_y], 0, 256);
                    }
                }
                if (editor.lines[editor.cursor_y]) {
                    int len = strlen(editor.lines[editor.cursor_y]);
                    if (len < 255) {
                        editor.lines[editor.cursor_y][len] = ch;
                        editor.cursor_x++;
                    }
                }
            }
            
            // Redraw editor
            ide_draw_editor();
            ide_update_cursor();
        }
        
        // Small delay to prevent busy waiting
        for (volatile int i = 0; i < 1000; i++);
    }
}

/* Compile user code */
int ide_compile(const char *code) {
    (void)code;  // Mark parameter as unused
    
    vid_clear();
    vid_print(0, 0, "Compiling VoyageC code...", 
              (COLOR_BLUE << 4) | COLOR_WHITE);
    
    /* Lexical analysis, parsing, code generation would happen here */
    
    return 0;  // Success
}

void ide_show_system_info(void) {
    vid_clear();
    
    uint8_t info_color = (COLOR_WHITE << 4) | COLOR_BLACK;
    
    vid_print(5, 3, "═══ VoyageOS System Information ═══", 
              (COLOR_CYAN << 4) | COLOR_BLACK);
    
    vid_print(5, 5, "Operating System: VoyageOS v0.1", info_color);
    vid_print(5, 6, "Architecture: x86-64", info_color);
    vid_print(5, 7, "IDE: VoyageIDE v0.1", info_color);
    vid_print(5, 8, "Compiler: VoyageC", info_color);
    
    vid_print(5, 10, "Total Memory: 2 GB (estimated)", info_color);
    vid_print(5, 11, "Available Memory: 1.8 GB", info_color);
    
    vid_print(5, 13, "Built-in Tools:", (COLOR_YELLOW << 4) | COLOR_BLACK);
    vid_print(5, 14, "  • Text Editor", info_color);
    vid_print(5, 15, "  • VoyageC Compiler", info_color);
    vid_print(5, 16, "  • REPL Interpreter", info_color);
    vid_print(5, 17, "  • Debugger", info_color);
    vid_print(5, 18, "  • File Manager", info_color);
    
    vid_print(5, 20, "Press any key to return...", 
              (COLOR_WHITE << 4) | COLOR_MAGENTA);
}
