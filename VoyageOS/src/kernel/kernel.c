#include "kernel.h"
#include "vga.h"
#include "drivers.h"
#include "io.h"
#include "shell.h"

void kernel_main() {
    serial_init();
    vga_init();
    keyboard_init();
    mouse_init();
    vga_puts("Welcome to VoyageOS\n");
    shell_run();
}