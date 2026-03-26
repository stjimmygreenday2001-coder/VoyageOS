/*
 * VoyageOS Kernel
 * Main kernel entry point in protected mode (32-bit)
 */

#include <stdint.h>
#include <stddef.h>

/* VGA buffer for text output */
#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile uint8_t *vga_buffer = (uint8_t *)VGA_ADDRESS;
int cursor_x = 0;
int cursor_y = 0;

/* Kernel color codes */
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_WHITE 7

/* IDT structures */
struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

#define IDT_ENTRIES 256
struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idt_ptr;

/* ISR handlers */
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void isr32(void);  // Keyboard interrupt
extern void isr32(void);  // Keyboard interrupt

/* Keyboard handling */
#define KEYBOARD_BUFFER_SIZE 256
char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
int keyboard_read_pos = 0;
int keyboard_write_pos = 0;

/* Keyboard scancode to ASCII mapping (US layout) */
char scancode_to_ascii[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboard_handler(void) {
    uint8_t scancode = 0;
    
    // Read scancode from keyboard
    __asm__ volatile("inb $0x60, %0" : "=a"(scancode));
    
    // Convert scancode to ASCII
    if (scancode < 128) {
        char ascii = scancode_to_ascii[scancode];
        if (ascii != 0) {
            // Add to buffer
            keyboard_buffer[keyboard_write_pos] = ascii;
            keyboard_write_pos = (keyboard_write_pos + 1) % KEYBOARD_BUFFER_SIZE;
        }
    }
    
    // Send EOI to PIC
    __asm__ volatile("movb $0x20, %al; outb %al, $0x20");
}

char keyboard_getchar(void) {
    if (keyboard_read_pos != keyboard_write_pos) {
        char c = keyboard_buffer[keyboard_read_pos];
        keyboard_read_pos = (keyboard_read_pos + 1) % KEYBOARD_BUFFER_SIZE;
        return c;
    }
    return 0;
}

/* PIC initialization */
void pic_init(void) {
    // ICW1: Start initialization
    __asm__ volatile("movb $0x11, %al; outb %al, $0x20");
    __asm__ volatile("movb $0x11, %al; outb %al, $0xA0");
    
    // ICW2: Set interrupt vectors (0x20-0x27 for master, 0x28-0x2F for slave)
    __asm__ volatile("movb $0x20, %al; outb %al, $0x21");
    __asm__ volatile("movb $0x28, %al; outb %al, $0xA1");
    
    // ICW3: Tell master about slave
    __asm__ volatile("movb $0x04, %al; outb %al, $0x21");
    __asm__ volatile("movb $0x02, %al; outb %al, $0xA1");
    
    // ICW4: Set 8086 mode
    __asm__ volatile("movb $0x01, %al; outb %al, $0x21");
    __asm__ volatile("movb $0x01, %al; outb %al, $0xA1");
    
    // Mask all interrupts except keyboard (IRQ 1)
    __asm__ volatile("movb $0xFD, %al; outb %al, $0x21");
    __asm__ volatile("movb $0xFF, %al; outb %al, $0xA1");
}

void vga_clear(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i++) {
        vga_buffer[i] = 0;
    }
    cursor_x = 0;
    cursor_y = 0;
}

void vga_putchar(char c, uint8_t color) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) {
            cursor_y = 0;
        }
        return;
    }
    
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = 0;
    }
    
    int offset = (cursor_y * VGA_WIDTH + cursor_x) * 2;
    vga_buffer[offset] = c;
    vga_buffer[offset + 1] = color;
    
    cursor_x++;
}

void vga_print(const char *str, uint8_t color) {
    while (*str) {
        vga_putchar(*str++, color);
    }
}

void print_hex(uint32_t value, uint8_t color) {
    char hex[] = "0123456789ABCDEF";
    vga_print("0x", color);
    for (int i = 7; i >= 0; i--) {
        char c = hex[(value >> (i * 4)) & 0xF];
        vga_putchar(c, color);
    }
}

/* IDT functions */
void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {
    idt[num].offset_low = base & 0xFFFF;
    idt[num].offset_mid = (base >> 16) & 0xFFFF;
    idt[num].offset_high = (base >> 32) & 0xFFFFFFFF;
    idt[num].selector = sel;
    idt[num].ist = 0;
    idt[num].type_attr = flags;
    idt[num].zero = 0;
}

void idt_init(void) {
    idt_ptr.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idt_ptr.base = (uint64_t)&idt;

    // Clear IDT
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt[i].offset_low = 0;
        idt[i].offset_mid = 0;
        idt[i].offset_high = 0;
        idt[i].selector = 0;
        idt[i].ist = 0;
        idt[i].type_attr = 0;
        idt[i].zero = 0;
    }

    // Set up ISRs (0-31 are exceptions)
    idt_set_gate(0, (uint64_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint64_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint64_t)isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint64_t)isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint64_t)isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint64_t)isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint64_t)isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint64_t)isr7, 0x08, 0x8E);
    idt_set_gate(8, (uint64_t)isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint64_t)isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint64_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint64_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint64_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint64_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint64_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint64_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint64_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint64_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint64_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint64_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint64_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint64_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint64_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint64_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint64_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint64_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint64_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint64_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint64_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint64_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint64_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint64_t)isr31, 0x08, 0x8E);
    idt_set_gate(32, (uint64_t)isr32, 0x08, 0x8E);  // Keyboard

    // Load IDT
    __asm__ volatile("lidt %0" : : "m"(idt_ptr));
}

/* Exception handlers */
void exception_handler(uint64_t vector, uint64_t error_code) {
    if (vector == 32) {
        // Keyboard interrupt
        keyboard_handler();
        return;
    }
    
    vga_clear();
    vga_print("VoyageOS Exception Handler\n", (COLOR_RED << 4) | COLOR_WHITE);
    vga_print("Exception: ", (COLOR_RED << 4) | COLOR_WHITE);
    print_hex(vector, (COLOR_RED << 4) | COLOR_WHITE);
    vga_print("\nError Code: ", (COLOR_RED << 4) | COLOR_WHITE);
    print_hex(error_code, (COLOR_RED << 4) | COLOR_WHITE);
    vga_print("\n\nSystem halted.", (COLOR_RED << 4) | COLOR_WHITE);
    
    while (1) {
        __asm__("hlt");
    }
}

/* Kernel entry point */
void ide_main(void) {
    vga_clear();
    
    uint8_t title_color = (COLOR_BLUE << 4) | COLOR_WHITE;
    uint8_t text_color = (COLOR_BLACK << 4) | COLOR_WHITE;
    uint8_t menu_color = (COLOR_CYAN << 4) | COLOR_BLACK;
    
    vga_print("╔════════════════════════════════════════════════════════════════════╗\n", title_color);
    vga_print("║                     VoyageIDE - Integrated Development Environment ║\n", title_color);
    vga_print("║                         A Journey Through Code                      ║\n", title_color);
    vga_print("╚════════════════════════════════════════════════════════════════════╝\n", title_color);
    vga_print("\n", text_color);
    
    vga_print("Welcome to VoyageIDE!\n\n", menu_color);
    vga_print("Available commands:\n", text_color);
    vga_print("  E - Editor\n", text_color);
    vga_print("  C - Compiler\n", text_color);
    vga_print("  R - REPL\n", text_color);
    vga_print("  S - System Info\n", text_color);
    vga_print("  Q - Quit to kernel\n\n", text_color);
    vga_print("Press a key: ", menu_color);
    
    while (1) {
        char c = keyboard_getchar();
        if (c) {
            vga_putchar(c, text_color);
            vga_print("\n\n", text_color);
            
            if (c == 'E' || c == 'e') {
                vga_print("Editor not implemented yet. Press Q to return.\n", (COLOR_RED << 4) | COLOR_WHITE);
            } else if (c == 'C' || c == 'c') {
                vga_print("Compiler not implemented yet. Press Q to return.\n", (COLOR_RED << 4) | COLOR_WHITE);
            } else if (c == 'R' || c == 'r') {
                ide_repl();
                // Return to menu
                vga_clear();
                vga_print("╔════════════════════════════════════════════════════════════════════╗\n", title_color);
                vga_print("║                     VoyageIDE - Integrated Development Environment ║\n", title_color);
                vga_print("║                         A Journey Through Code                      ║\n", title_color);
                vga_print("╚════════════════════════════════════════════════════════════════════╝\n", title_color);
                vga_print("\n", text_color);
                vga_print("Welcome back to VoyageIDE!\n\n", menu_color);
                vga_print("Available commands:\n", text_color);
                vga_print("  E - Editor\n", text_color);
                vga_print("  C - Compiler\n", text_color);
                vga_print("  R - REPL\n", text_color);
                vga_print("  S - System Info\n", text_color);
                vga_print("  Q - Quit to kernel\n\n", text_color);
                vga_print("Press a key: ", menu_color);
            } else if (c == 'S' || c == 's') {
                vga_print("System Information:\n", menu_color);
                vga_print("  OS: VoyageOS v0.1\n", text_color);
                vga_print("  Architecture: x86-64\n", text_color);
                vga_print("  Memory: 256 MB\n", text_color);
                vga_print("  IDE: VoyageIDE (basic)\n", text_color);
                vga_print("\nPress Q to return.\n", text_color);
            } else if (c == 'Q' || c == 'q') {
                return;
            } else {
                vga_print("Invalid command. Try again: ", menu_color);
                continue;
            }
            
            if (c != 'S' && c != 's') {
                vga_print("\nPress Q to return to menu: ", menu_color);
            }
        }
        __asm__("hlt");
    }
}

void ide_repl(void) {
    vga_clear();
    
    uint8_t title_color = (COLOR_GREEN << 4) | COLOR_BLACK;
    uint8_t prompt_color = (COLOR_WHITE << 4) | COLOR_BLACK;
    uint8_t text_color = (COLOR_BLACK << 4) | COLOR_WHITE;
    uint8_t error_color = (COLOR_RED << 4) | COLOR_WHITE;
    
    vga_print("╔════════════════════════════════════════════════════════════════════╗\n", title_color);
    vga_print("║                        VoyageC REPL - Interactive Mode            ║\n", title_color);
    vga_print("╚════════════════════════════════════════════════════════════════════╝\n", title_color);
    vga_print("\n", text_color);
    
    vga_print("Welcome to VoyageC REPL!\n", prompt_color);
    vga_print("Type expressions or 'exit' to quit.\n\n", text_color);
    
    char input_buffer[256];
    int buffer_pos = 0;
    
    vga_print(">>> ", prompt_color);
    
    while (1) {
        char c = keyboard_getchar();
        if (c) {
            if (c == '\n') {
                // Process the command
                input_buffer[buffer_pos] = 0;
                
                if (strcmp(input_buffer, "exit") == 0) {
                    return;
                }
                
                // Simple expression evaluation (just echo for now)
                vga_print("\nResult: ", prompt_color);
                vga_print(input_buffer, text_color);
                vga_print("\n\n>>> ", prompt_color);
                
                buffer_pos = 0;
            } else if (c == '\b') {
                if (buffer_pos > 0) {
                    buffer_pos--;
                    // Move cursor back
                    vga_print("\b \b", text_color);
                }
            } else if (buffer_pos < 255) {
                input_buffer[buffer_pos++] = c;
                vga_putchar(c, text_color);
            }
        }
        __asm__("hlt");
    }
}

// Simple string comparison
int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) {
            return *s1 - *s2;
        }
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

/* Kernel entry point */
void kmain(void) {
    vga_clear();
    
    uint8_t title_color = (COLOR_BLUE << 4) | COLOR_WHITE;
    uint8_t text_color = (COLOR_BLACK << 4) | COLOR_WHITE;
    uint8_t info_color = (COLOR_BLACK << 4) | COLOR_GREEN;
    
    vga_print("╔════════════════════════════════════════════════════════════════════╗\n", title_color);
    vga_print("║                    Welcome to VoyageOS                             ║\n", title_color);
    vga_print("║                   A Journey Through Code                           ║\n", title_color);
    vga_print("║                         v0.1 (Beta)                                ║\n", title_color);
    vga_print("╚════════════════════════════════════════════════════════════════════╝\n", title_color);
    vga_print("\n", text_color);
    
    vga_print("kernel: ", info_color);
    vga_print("Kernel initialized in long mode (64-bit)\n", text_color);
    
    vga_print("kernel: ", info_color);
    vga_print("Bootloader jumped to 0x10000\n", text_color);
    
    vga_print("kernel: ", info_color);
    vga_print("Paging structures initialized\n", text_color);
    
    vga_print("kernel: ", info_color);
    vga_print("VGA text output enabled (80x25)\n", text_color);
    
    // Initialize IDT
    idt_init();
    vga_print("kernel: ", info_color);
    vga_print("Interrupt Descriptor Table initialized\n", text_color);
    
    // Initialize PIC
    pic_init();
    vga_print("kernel: ", info_color);
    vga_print("Programmable Interrupt Controller initialized\n", text_color);
    
    // Enable interrupts
    __asm__ volatile("sti");
    vga_print("kernel: ", info_color);
    vga_print("Interrupts enabled\n", text_color);
    
    vga_print("\n", text_color);
    vga_print("kernel: ", info_color);
    vga_print("Next phase: Long mode and VoyageIDE initialization\n", text_color);
    
    vga_print("\n", text_color);
    vga_print("kernel: ", info_color);
    vga_print("System ready. Press 'I' for VoyageIDE or type commands.\n", text_color);
    
    /* Main loop - check for keyboard input */
    while (1) {
        char c = keyboard_getchar();
        if (c) {
            if (c == 'I' || c == 'i') {
                // Launch VoyageIDE
                ide_main();
            } else if (c == '\n') {
                vga_print("\nkernel: ", info_color);
            } else {
                vga_putchar(c, text_color);
            }
        }
        __asm__("hlt");
    }
}
