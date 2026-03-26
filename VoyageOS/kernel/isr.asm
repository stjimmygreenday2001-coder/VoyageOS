; VoyageOS ISR / Exception Handler Stub
BITS 64

extern exception_handler   ; Now properly declared below

global isr_common_stub

%macro ISR_NOERRCODE 1
    global isr%1
isr%1:
    push 0
    push %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
    global isr%1
isr%1:
    push %1
    jmp isr_common_stub
%endmacro

; Common ISR stub
isr_common_stub:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp          ; Pass stack pointer to handler
    call exception_handler

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16           ; Remove error code + interrupt number
    iretq

; Define common exceptions (you can expand later)
ISR_NOERRCODE 0   ; Division by zero
ISR_NOERRCODE 1   ; Debug
ISR_NOERRCODE 2   ; NMI
ISR_NOERRCODE 3   ; Breakpoint
ISR_NOERRCODE 4   ; Overflow
ISR_NOERRCODE 5   ; Bound range
ISR_NOERRCODE 6   ; Invalid opcode
ISR_NOERRCODE 7   ; Device not available
ISR_ERRCODE   8   ; Double fault
ISR_NOERRCODE 9   ; Coprocessor segment overrun
ISR_ERRCODE   10  ; Invalid TSS
ISR_ERRCODE   11  ; Segment not present
ISR_ERRCODE   12  ; Stack-segment fault
ISR_ERRCODE   13  ; General protection fault
ISR_ERRCODE   14  ; Page fault

; Hardware interrupts (IRQs)
ISR_NOERRCODE 32  ; Timer (IRQ 0)
ISR_NOERRCODE 33  ; Keyboard (IRQ 1)