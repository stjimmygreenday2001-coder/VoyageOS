#include "vga.h"
#include "drivers.h"
#include "string.h"
#include "compiler.h"
#include "interpreter.h"
#include "sound.h"

void shell_run() {
    char buffer[256];
    int index = 0;
    vga_puts("VoyageOS> ");
    while (1) {
        char c = keyboard_getchar();
        if (c) {
            if (c == '\n') {
                buffer[index] = 0;
                if (strcmp(buffer, "print hello") == 0) {
                    compile(buffer);
                } else if (strcmp(buffer, "1+1") == 0) {
                    interpret(buffer);
                } else if (strcmp(buffer, "beep") == 0) {
                    sound_beep();
                } else if (strcmp(buffer, "clear") == 0) {
                    vga_init();
                } else {
                    vga_puts("Unknown command\n");
                }
                index = 0;
                vga_puts("VoyageOS> ");
            } else if (c == '\b' && index > 0) {
                index--;
                vga_putc('\b');
                vga_putc(' ');
                vga_putc('\b');
            } else if (index < 255) {
                buffer[index++] = c;
                vga_putc(c);
            }
        }
    }
}