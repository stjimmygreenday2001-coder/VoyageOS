#include "vga.h"

void interpret(const char* code) {
    // Simple expression evaluator for + and numbers
    int result = 0;
    char op = '+';
    const char* ptr = code;
    while (*ptr) {
        if (*ptr >= '0' && *ptr <= '9') {
            int num = 0;
            while (*ptr >= '0' && *ptr <= '9') {
                num = num * 10 + (*ptr - '0');
                ptr++;
            }
            if (op == '+') result += num;
            else if (op == '-') result -= num;
        } else if (*ptr == '+') {
            op = '+';
            ptr++;
        } else if (*ptr == '-') {
            op = '-';
            ptr++;
        } else {
            ptr++;
        }
    }
    // Output result
    if (result == 0) vga_putc('0');
    else {
        int temp = result;
        if (temp < 0) { vga_putc('-'); temp = -temp; }
        int digits[10];
        int d = 0;
        while (temp) {
            digits[d++] = temp % 10;
            temp /= 10;
        }
        for (int j = d-1; j >= 0; j--) {
            vga_putc('0' + digits[j]);
        }
    }
    vga_putc('\n');
}