# VoyageOS Development Guide

## Overview

VoyageOS is a self-hosted operating system where development happens entirely within the OS itself. The primary interface is **VoyageIDE**, a built-in editor and compiler.

## System Architecture

```
┌─────────────────────────────────────┐
│         VoyageOS Bootloader         │
│   (x86-64 protected/long mode)      │
├─────────────────────────────────────┤
│          VoyageOS Kernel            │
│  (Memory, processes, filesystem)    │
├─────────────────────────────────────┤
│  VoyageIDE | VoyageC | Shell | Apps │
│        (User-space applications)    │
└─────────────────────────────────────┘
```

## Development Workflow

### 1. Bootloader Development
- Located in `bootloader/`
- Written in x86-64 assembly (NASM syntax)
- Handles:
  - Real mode initialization
  - A20 gate enabling
  - GDT setup
  - Protected mode entry
  - Long mode (64-bit) setup

### 2. Kernel Development
- Located in `kernel/`
- Written in C with inline assembly
- Handles:
  - Interrupt handling (IDT)
  - Memory management (paging)
  - Process management
  - VGA text output
  - System calls

### 3. VoyageIDE Development
- Located in `voyageide/`
- The main development interface
- Features:
  - Text editor with syntax highlighting
  - Integrated compiler
  - REPL for runtime evaluation
  - Debugger
  - File manager

### 4. VoyageC Compiler
- Located in `compiler/`
- Custom C-like language with assembly interop
- Compile pipeline:
  1. **Lexical Analysis** - Tokenize source
  2. **Parsing** - Build abstract syntax tree (AST)
  3. **Type Checking** - Validate types
  4. **Code Generation** - Emit x86-64 assembly
  5. **Assembly** - Assemble to machine code
  6. **Linking** - Link object files

## Building VoyageOS

### Quick Start

```bash
cd /home/jasmine/Desktop/VoyageOS

# Build everything
make

# Or use the build script
bash scripts/build.sh all

# Run in QEMU
make run
# or
bash scripts/build.sh run
```

### Building Components Separately

```bash
# Individual builds
make bootloader
make kernel
make voyageide
make compiler

# Or
bash scripts/build.sh bootloader
bash scripts/build.sh kernel
bash scripts/build.sh ide
bash scripts/build.sh compiler
```

### Prerequisites

**Ubuntu/Debian:**
```bash
sudo apt-get install nasm gcc binutils qemu-system-x86
```

**macOS:**
```bash
brew install nasm gcc binutils qemu
```

**Windows (WSL recommended):**
Follow Ubuntu/Debian instructions in WSL

## Writing VoyageC Code

VoyageC is a C-like language with some enhancements:

```c
fn main() {
    // Variables
    num x = 42;
    text msg = "Hello, VoyageOS!";
    
    // Control flow
    if (x > 0) {
        print(msg);
    }
    
    // Functions
    sum = add(10, 20);
    
    // Assembly interop
    asm {
        mov rax, x
        add rax, 1
    }
    
    return 0;
}

fn add(num a, num b) -> num {
    return a + b;
}
```

## VoyageIDE Commands

**Editor Mode:**
- `Ctrl+S` - Save file
- `Ctrl+O` - Open file
- `Ctrl+N` - New file
- `Ctrl+C` - Compile current file
- `F1` - Help
- `ESC` - Main menu

**REPL Mode (F4):**
- Type VoyageC code directly
- Press Enter to evaluate
- `exit` to return to editor

**Compiler Mode (F3):**
- Shows compilation progress
- Displays errors and warnings
- Generates optimized assembly

## Memory Map (Planned)

```
Physical Memory Layout:
0x00000000 - 0x00000FFF   | Real mode IVT / BIOS data
0x00001000 - 0x00007FFF   | Bootloader
0x00008000 - 0x0000FFFF   | Extended BIOS data
0x00010000 - 0x000FFFFF   | Kernel (low 1MB)
0x00100000 - 0x7FFFFFFF   | User space / Heap / Stack
0x80000000 - 0xFFFFFFFF   | Kernel space (high)
```

## Debugging

Enable debug output in kernel:

```c
// In kernel/kernel.c
debug_log("Message: %d\n", value);
```

Run with debugging:
```bash
qemu-system-x86_64 -s -S -fda build/boot.bin
```

Connect GDB in another terminal:
```bash
gdb build/kernel.bin
(gdb) target remote localhost:1234
(gdb) break main
(gdb) continue
```

## Project Structure Details

```
VoyageOS/
├── bootloader/
│   └── bootloader.asm        # x86-64 bootloader
│
├── kernel/
│   ├── kernel.c              # Main kernel
│   ├── interrupts.c          # ISR/IDT handling
│   ├── memory.c              # Paging, allocation
│   └── process.c             # Process management
│
├── voyageide/
│   ├── ide.c                 # Editor + IDE
│   ├── repl.c                # REPL interpreter
│   └── debugger.c            # Integrated debugger
│
├── compiler/
│   ├── voyagec.c             # Main compiler
│   ├── lexer.c               # Tokenizer
│   ├── parser.c              # AST builder
│   └── codegen.c             # Code generator
│
├── tools/
│   ├── shell.c               # Command shell
│   ├── filemanager.c         # File utilities
│   └── utils.c               # System utilities
│
├── scripts/
│   ├── build.sh              # Main build script
│   ├── run.sh                # Run in emulator
│   └── debug.sh              # Debug helper
│
├── docs/
│   ├── ARCHITECTURE.md       # System design
│   ├── CODING_STANDARD.md    # Style guide
│   └── API.md                # System API docs
│
├── Makefile                  # Build configuration
└── README.md                 # Project overview
```

## Coding Standards

### C Code
- Use `snake_case` for functions and variables
- Use `PascalCase` for types and structs
- Prefix kernel internals with `k_` (e.g., `k_alloc`)
- Maximum 80 characters per line
- Use 4-space indentation

### Assembly
- Use NASM syntax
- Prefix labels with section name (e.g., `pm_gdt`)
- Document non-obvious instructions
- Use modern x86-64 registers

## Next Steps

1. **Expand kernel features**: Process scheduling, filesystem support
2. **Improve VoyageIDE**: Better editor, multiple windows, plugins
3. **Enhance compiler**: Optimization passes, better error messages
4. **Add system calls**: File I/O, memory management, process control
5. **Port applications**: Popular tools and languages

## Contributing to VoyageOS Development

When modifying VoyageOS code:

1. Keep bootloader minimal (< 10KB)
2. Kernel should handle its own errors gracefully
3. IDE features should be discoverable via on-screen help
4. Compiler should generate efficient x86-64 code
5. Always test with `make run`

---

**Happy coding on VoyageOS!** 🚀
