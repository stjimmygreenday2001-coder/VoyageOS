# VoyageOS Architecture Guide

## System Overview

VoyageOS is a monolithic x86-64 kernel-based operating system with integrated development tools. The architecture prioritizes minimalism, self-hosting, and iterative development.

## CPU Architecture: x86-64

### Memory Modes

```
Real Mode (16-bit) 
    ↓ (A20 gate enabled)
Protected Mode (32-bit, GDT)
    ↓ (PAE enabled, page tables setup)
Long Mode (64-bit, IA-32e)
```

### Register Usage

```
RAX, RBX, RCX, RDX  | General purpose
RSI, RDI            | Source/Destination
RBP                 | Base pointer (frame)
RSP                 | Stack pointer
R8-R15              | Additional registers

RIP                 | Instruction pointer
RFLAGS              | Status flags
```

### x86-64 Specifics

- **64-bit pointers** (up to 48-bit in practice with current CPUs)
- **Large address space** (canonical addresses: 0x0000_0000_0000_0000 to 0x7FFF_FFFF_FFFF_FFFF for user, 0xFFFF_8000_0000_0000+ for kernel)
- **Per-CPU TSS** (Task State Segment) for syscall entry
- **GDT-based segmentation** (minimal in long mode)
- **Paging** (mandatory in long mode, 4KB/2MB/1GB pages)

## Memory Layout

### Virtual Address Space

```
0x0000_0000_0000_0000 - 0x7FFF_FFFF_FFFF_FFFF │
    User Space                                  │ Canonical range
    (code, data, heap, stack)                   │ 48-bit addressable
0xFFFF_8000_0000_0000 - 0xFFFF_FFFF_FFFF_FFFF │
    Kernel Space
    (kernel code, page tables, memory-mapped I/O)
```

### Kernel Virtual Memory Map

```
0xFFFF_FFFF_E000_0000   Kernel code/data (identity mapped)
0xFFFF_FFFF_F000_0000   Direct mapping of all physical RAM
0xFFFF_FFFF_FF00_0000   Memory-mapped I/O (VGA, APIC, etc.)
0xFFFF_FFFF_FFFF_0000   Kernel stacks
0xFFFF_FFFF_FFFF_F000   Kernel page tables
```

## Boot Sequence

```
1. BIOS/EFI Firmware
   └─→ Loads MBR (512 bytes) at 0x7C00

2. Bootloader Stage 1 (bootloader.asm - 512 bytes)
   ├─ Enable A20 gate
   ├─ Load GDT (protected mode)
   ├─ Clear CR0.PG, set CR0.PE (protected mode)
   └─ Jump to Stage 2

3. Bootloader Stage 2 (32-bit)
   ├─ Setup paging structures
   ├─ Load 64-bit GDT
   ├─ Set CR4.PAE, clear CR4.PSE
   ├─ Load CR3 (PDPT address)
   ├─ Set EFER.LME (long mode enable)
   ├─ Set CR0.PG (paging enable)
   └─ Jump to kernel entry (0x10000)

4. Kernel (kernel.c - 64-bit)
   ├─ Initialize BSS (zero initialized data)
   ├─ Setup IDT (interrupt descriptor table)
   ├─ Initialize memory allocator
   ├─ Setup TSS (task state segment)
   ├─ Initialize VGA driver
   ├─ Load VoyageIDE
   └─ Enter main kernel loop
```

## Privilige Levels

### Ring 0 (Kernel)
- Full CPU control
- Can execute privileged instructions
- Runs bootloader and kernel code
- Handles interrupts and exceptions

### Ring 3 (User)
- Limited CPU access
- Cannot execute privileged instructions
- Runs applications and VoyageIDE
- Accesses kernel via syscalls

## Interrupt Handling

### Exception Vectors (0-31)
```
0x00    Divide by Zero
0x01    Debug
0x02    NMI (Non-Maskable Interrupt)
0x03    Breakpoint
0x04    Overflow
0x05    Bound Range
0x06    Invalid Opcode
0x07    Device Not Available
0x08    Double Fault
0x0A    Invalid TSS
0x0B    Segment Not Present
0x0C    Stack Segment Fault
0x0D    General Protection Fault
0x0E    Page Fault
0x10    Floating Point Error
0x11    Alignment Check
0x12    Machine Check
```

### Hardware Interrupts (32-255)
- IRQ 0: Timer
- IRQ 1: Keyboard
- IRQ 2: Cascade (PIC)
- IRQ 3-7: Legacy devices
- IRQ 8-15: Secondary PIC
- 32+: User-defined

## Memory Management

### Paging Structures (4-level page table)

```
                                    ┌─────────┐
Virtual Address                    │   TLB   │
↓                                   └─────────┘
┌─────────────────────────────────────┐
│ Sign Extension (63-48) │ Index (47-0)│
└──────────┬──────────────────┬────────┘
           │                  │
      [39-47-12] bytes    ┌──→ PML4 (Page Map Level 4)
      to create offset        ├── 512 entries × 8 bytes
                              └─→ PDPT (Page Directive Ptr Table)
                                  ├── 512 entries × 8 bytes
                                  └─→ PDT (Page Directory Table)
                                      ├── 512 entries × 8 bytes
                                      └─→ PT (Page Table)
                                          ├── 512 entries × 8 bytes
                                          └─→ Physical Page (4KB)
```

### Page Table Entry Format

```
┌───────────────────────────────────────────────────────────┐
│ 63 62  59     48     40     32     20         12  11   0  │
│ N  RZO Address       Address       Address     Flags Page │
│ X          (upper)   (middle)      (lower)            Offs │
└───────────────────────────────────────────────────────────┘

Flags:
  P   (0)   - Present
  R/W (1)   - Read/Write
  U/S (2)   - User/Supervisor
  PWT (3)   - Page Write-Through
  PCD (4)   - Page Cache Disable
  A   (5)   - Accessed
  D   (6)   - Dirty (for PTEs only)
  PAT (7)   - Page Attribute Table
  G   (8)   - Global (for PTEs only)
```

## Process/Task Model (Planned)

### Task Control Block (TCB)

```c
struct TCB {
    uint64_t rip;           // Instruction pointer
    uint64_t rsp;           // Stack pointer
    uint64_t rax, rbx, etc; // Registers
    void *page_table;       // CR3 value
    int priority;           // Scheduling priority
    state pid;              // Process ID
    state *next;            // Next in queue
}
```

### Context Switching

```
Task A (Running)
  ↓ (Timer interrupt or yield)
Save A's context (registers) to TCB_A
Load B's context (registers) from TCB_B
Load B's page table (CR3)
Load B's RIP (into RIP)
  ↓
Task B (Running)
```

## Syscall Interface

### x86-64 Syscall Mechanism

```
User Mode (Ring 3)
  mov rax, SYSCALL_NUMBER
  mov rdi, arg1
  mov rsi, arg2
  ... (more args in r8-r11)
  syscall           ← Enter Ring 0
  
  (kernel handles in syscall handler)
  
Kernel Mode (Ring 0)
  return to user with RAX = result
```

## VGA Text Mode (0x03)

### Memory Layout (0xB8000)

```
Each character cell = 2 bytes
Byte 0: ASCII character
Byte 1: Attributes (foreground/background color)

Color Code:
  0 = Black       8 = Dark Gray
  1 = Blue        9 = Light Blue
  2 = Green       A = Light Green
  3 = Cyan        B = Light Cyan
  4 = Red         C = Light Red
  5 = Magenta     D = Light Magenta
  6 = Brown       E = Yellow
  7 = White       F = White (bright)

Screen: 80 columns × 25 rows = 2000 cells
Offset to row Y, col X: (y * 80 + x) * 2
```

## Kernel Module Architecture

### Built-in Modules

```
kernel.c
├── Memory Management (k_alloc, k_free)
├── Page Fault Handler (do_page_fault)
├── Interrupt Dispatcher (dispatch_irq)
├── Syscall Handler (handle_syscall)
└── Device Drivers
    ├── VGA Text Driver
    ├── Keyboard Driver
    └── Timer Driver
```

### Access Control

- **Kernel only**: Memory allocation, paging, interrupts, I/O ports
- **User callable**: Syscalls (read, write, open, close, exec, etc.)
- **Kernel + IDE**: File system operations, process management

## IDE Integration

### VoyageIDE Memory Map

```
0x00000000 - User code/data
0x08000000 - IDE text buffer
0x09000000 - IDE heap
0x0A000000 - Compiler state
0x0B000000 - REPL state
```

### Editor Buffer
- Variable-length lines
- Up to 512 lines loaded
- Efficient string operations

### Compiler Integration
- Real-time syntax checking
- Background compilation
- Error highlighting
- Code completion (planned)

## Performance Considerations

### Optimization Points

1. **Bootloader**: Minimal, < 10 KB
2. **Kernel**: Lean I/O, efficient memory management
3. **IDE**: Responsive editing, cached syntax trees
4. **Compiler**: Single-pass code generation, minimal optimization passes

### Cache Efficiency

- Align kernel data structures to cache lines (64 bytes)
- Minimize TLB misses with strategic virtual addresses
- Use large pages (2MB) for kernel code section

---

*This document describes the current and planned architecture of VoyageOS.*
