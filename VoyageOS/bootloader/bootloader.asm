; VoyageOS Bootloader - Improved version with better debugging
BITS 16
ORG 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov ax, 0x0003      ; Clear screen
    int 0x10

    mov si, boot_msg
    call print_string

    mov al, 'B'         ; Bootloader started
    mov ah, 0x0E
    int 0x10

    call load_kernel    ; Try to load kernel

    ; If we reach here, loading succeeded
    mov al, 'D'
    mov ah, 0x0E
    int 0x10

    call enable_a20
    call setup_paging

    jmp 0x08:pmode_start

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

enable_a20:
    mov ax, 0x2401
    int 0x15
    ret

setup_paging:
    mov edi, 0x1000
    mov cr3, edi
    xor eax, eax
    mov ecx, 4096*4         ; Clear more for safety
    rep stosd
    mov edi, cr3

    mov dword [edi], 0x2003
    add edi, 0x1000
    mov dword [edi], 0x3003
    add edi, 0x1000
    mov dword [edi], 0x4003
    add edi, 0x1000

    mov ebx, 0x00000003
    mov ecx, 512
.set_entry:
    mov dword [edi], ebx
    add ebx, 0x1000
    add edi, 8
    loop .set_entry
    ret

; Improved kernel loader - loads up to 60 sectors (30KB) with per-sector debug
load_kernel:
    xor ax, ax
    int 0x13                ; Reset drive
    jc .disk_fail

    mov cx, 60              ; Increased to 60 sectors (should be enough for now)
    mov ax, 0x1000
    mov es, ax
    xor bx, bx

    mov dh, 0               ; head 0
    mov ch, 0               ; cylinder 0
    mov cl, 2               ; start at sector 2

.read_loop:
    mov ah, 0x02
    mov al, 1
    mov dl, 0
    int 0x13
    jc .disk_fail           ; If any sector fails, show error

    ; Show progress dot for each successful sector
    mov al, '.'
    mov ah, 0x0E
    int 0x10

    add bx, 512
    inc cl

    cmp cl, 19
    jne .no_wrap
    mov cl, 1
    inc dh
    cmp dh, 2
    jne .no_wrap
    mov dh, 0
    inc ch
.no_wrap:
    loop .read_loop
    ret

.disk_fail:
    mov si, disk_err_msg
    call print_string
    hlt
    jmp $

; 32-bit protected mode
BITS 32
pmode_start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x7C00

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    lgdt [gdt64_descriptor]
    jmp 0x08:long_mode_start

; 64-bit long mode
BITS 64
long_mode_start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov rsp, 0x7C00

    jmp 0x10000             ; Jump to kernel

; GDT
align 4
gdt:
    dq 0
    dq 0x00209a0000000000   ; 64-bit code
    dq 0x0000920000000000   ; 64-bit data

gdt64_descriptor:
    dw gdt64_descriptor - gdt - 1
    dq gdt

; Messages
boot_msg:     db "VoyageOS Bootloader v2", 0x0D, 0x0A, 0
disk_err_msg: db 0x0D, 0x0A, "Disk read failed! Cannot load kernel.", 0x0D, 0x0A, 0

times (510 - ($ - $$)) db 0
dw 0xAA55