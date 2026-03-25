; VoyageOS Kernel Entry (Long Mode 64-bit)
; Called from bootloader at 0x10000
; Sets up kernel environment and jumps to C code

BITS 64

; Entry point - bootloader jumps here (0x10000)
global kernel_entry

section .text
    kernel_entry:
        ; Clear frame pointer for proper stack traces
        xor rbp, rbp

        ; Setup kernel stack (grows downward from high address)
        mov rsp, 0x1F000      ; Stack pointer at 0x1F000

        ; Clear RFLAGS register
        xor rax, rax
        push rax
        popf

        ; Disable interrupts since no IDT is setup
        cli

        ; Call C kernel main
        extern kmain
        call kmain

        ; Halt if kmain returns (shouldn't happen)
        cli
        hlt
        jmp $
