; VoyageOS Bootloader - Simple loading animation
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

    mov ax, 0x0003
    int 0x10

    call load_kernel
    call enable_a20
    call setup_paging
    jmp 0x08:pmode_start

print_string:
    push ax
.loop:
    lodsb
    test al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp .loop
.done:
    pop ax
    ret

load_kernel:
    xor ax, ax
    int 0x13
    jc disk_error

    mov cx, 100
    mov ax, 0x1000
    mov es, ax
    xor bx, bx
    mov dh, 0
    mov ch, 0
    mov cl, 2

.read_loop:
    mov ah, 0x02
    mov al, 1
    mov dl, 0
    int 0x13
    jc disk_error

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
    cmp ch, 0
    jne .read_loop
    cmp dh, 0
    jne .read_loop
    cmp cl, 2
    jl .read_loop
    ret

disk_error:
    mov si, disk_err
    call print_string
    hlt

enable_a20:
    mov ax, 0x2401
    int 0x15
    ret

setup_paging:
    mov edi, 0x1000
    mov cr3, edi
    xor eax, eax
    mov ecx, 4096*4
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
.loop:
    mov dword [edi], ebx
    add ebx, 0x1000
    add edi, 8
    loop .loop
    ret

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
    lgdt [gdtd]
    jmp 0x08:lm_start

BITS 64
lm_start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov rsp, 0x7C00
    jmp 0x10000

align 4
gdt:
    dq 0
    dq 0x00209a0000000000
    dq 0x0000920000000000
gdtd:
    dw gdtd - gdt - 1
    dq gdt

boot_msg:  db "VoyageOS [", 0
disk_err:  db "]", 0x0D, 0x0A, "Disk err", 0

times (510 - ($ - $$)) db 0
dw 0xAA55