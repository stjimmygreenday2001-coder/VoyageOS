; VoyageOS Bootloader (MBR - 512 bytes)
; Minimal bootloader for QEMU floppy/disk boot
; Works with both real mode and protected mode environments

BITS 16
ORG 0x7C00

; ================================
; Boot Sector Entry Point
; ================================

start:
    cli                        ; Disable interrupts
    
    ; Setup segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00            ; Stack just below bootloader
    sti                        ; Re-enable for BIOS calls
    
    ; Clear screen
    mov ax, 0x0003            ; 80x25 text mode
    int 0x10
    
    ; Display boot message
    mov si, boot_msg
    call print_string

    ; Debug: show we're starting kernel load
    mov al, 'L'
    mov ah, 0x0E
    mov bh, 0
    int 0x10

    ; Load kernel from disk to 0x10000
    call load_kernel

    ; Debug: show kernel load completed
    mov al, 'D'
    mov ah, 0x0E
    mov bh, 0
    int 0x10

    ; Enable A20 gate
    call enable_a20

    ; Setup paging for long mode
    call setup_paging

    ; Far jump to protected mode
    jmp 0x08:pmode_start

; ================================
; Real Mode Helper Functions
; ================================

; Print string to screen
; Input: SI = pointer to null-terminated string
print_string:
    push ax
    push bx
.loop:
    lodsb
    test al, al
    jz .done
    mov ah, 0x0E
    mov bh, 0
    int 0x10
    jmp .loop
.done:
    pop bx
    pop ax
    ret

; Enable A20 gate
enable_a20:
    mov ax, 0x2401
    int 0x15
    ret

; Setup paging structures for long mode
setup_paging:
    ; Clear page tables
    mov edi, 0x1000
    mov cr3, edi
    xor eax, eax
    mov ecx, 4096
    rep stosd
    mov edi, cr3

    ; PML4[0] -> PDP
    mov dword [edi], 0x2003
    add edi, 0x1000
    ; PDP[0] -> PD
    mov dword [edi], 0x3003
    add edi, 0x1000
    ; PD[0] -> PT
    mov dword [edi], 0x4003
    add edi, 0x1000

    ; Identity map first 2MB
    mov ebx, 0x00000003
    mov ecx, 512
.set_entry:
    mov dword [edi], ebx
    add ebx, 0x1000
    add edi, 8
    loop .set_entry

    ret

; Load kernel from disk into memory at 0x10000
load_kernel:
    ; Reset disk controller first
    mov ah, 0x00
    mov dl, 0x00
    int 0x13
    jc disk_error

    ; Use LBA addressing for simpler disk reading
    ; Disk Address Packet
    mov si, dap
    mov word [si], 0x0010     ; packet size
    mov word [si+2], 26       ; sectors to read
    mov word [si+4], 0x0000   ; destination offset
    mov word [si+6], 0x1000   ; destination segment
    mov dword [si+8], 1       ; LBA sector 1 (start of kernel)
    mov dword [si+12], 0      ; LBA high (0 for floppy)

    mov ah, 0x42              ; extended read
    mov dl, 0x00              ; drive 0
    int 0x13
    jc disk_error

    ; Success - kernel loaded
    ret

; Disk Address Packet (16 bytes)
dap:
    db 0x10                   ; packet size
    db 0                      ; reserved
    dw 26                     ; sectors to read
    dw 0x0000                 ; destination offset
    dw 0x1000                 ; destination segment
    dd 1                      ; LBA low
    dd 0                      ; LBA high

; Disk read error handler
disk_error:
    ; Debug: show which sector failed
    mov al, 'E'
    mov ah, 0x0E
    mov bh, 0
    int 0x10
    mov al, cl
    add al, '0'
    int 0x10

    mov si, disk_err_msg
    call print_string
    hlt
    jmp $

; ================================
; Protected Mode (32-bit)
; ================================

BITS 32
align 4

pmode_start:
    ; Load data segment register (0x10 = data segment)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Setup stack
    mov esp, 0x7C00

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Set long mode bit in EFER
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; Load 64-bit GDT
    lgdt [gdt64_descriptor]

    ; Jump to 64-bit code
    jmp 0x08:long_mode_start

; ================================
; Long Mode (64-bit)
; ================================

BITS 64

long_mode_start:
    ; Clear segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Setup stack
    mov rsp, 0x7C00

    ; Jump to kernel at 0x10000
    jmp 0x10000

; ================================
; Global Descriptor Table (GDT)
; ================================

align 4
gdt:
    ; Null descriptor
    dq 0

    ; 64-bit code descriptor (0x08)
    dq 0x00209a0000000000

    ; 64-bit data descriptor (0x10)
    dq 0x0000920000000000

gdt_descriptor:
    dw gdt_descriptor - gdt - 1
    dd gdt

; 64-bit GDT descriptor
gdt64_descriptor:
    dw gdt64_descriptor - gdt - 1
    dq gdt

; ================================
; Messages
; ================================

boot_msg: db "VoyageOS Bootloader", 0x0D, 0x0A, 0

disk_err_msg: db "Disk read error! Cannot load kernel.", 0x0D, 0x0A, 0

; ================================
; Padding and Boot Signature
; ================================

; Pad to 510 bytes, then add boot signature
times (510 - ($ - $$)) db 0
dw 0xAA55
