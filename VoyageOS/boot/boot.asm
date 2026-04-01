; Bootloader for VoyageOS
; Multiboot compliant

BITS 32

section .multiboot_header
align 4
header_start:
    dd 0x1BADB002                ; magic number
    dd 0                         ; flags
    dd - (0x1BADB002 + 0)        ; checksum
header_end:

extern kernel_main

section .text
global start

start:
    ; Set up stack
    mov esp, stack_top

    ; Call kernel main
    call kernel_main

    ; Halt
    hlt

section .bss
stack_bottom:
    resb 4096
stack_top: