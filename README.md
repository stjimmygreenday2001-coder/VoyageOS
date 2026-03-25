# VoyageOS
Voyage Operating System 

A unique, from-scratch x86-64 operating system with an internal development environment (VoyageIDE). Taking inspiration from TempleOS but with its own identity and randomized theme.

## Vision

VoyageOS is built with the philosophy of:
- **Self-hosted development**: Code and compile within the OS itself
- **Minimalist design**: Focused on core functionality
- **Creative freedom**: Randomized theme/aesthetics for unpredictability
- **Learning journey**: Understanding OS internals from bootloader to userland

## Project Structure

```
VoyageOS/
├── bootloader/      # x86-64 bootloader (stage1/stage2)
├── kernel/          # Core kernel implementation
├── voyageide/       # Built-in IDE & development environment
├── compiler/        # VoyageC compiler (C/Assembly hybrid)
├── tools/           # Utility tools and system utilities
├── scripts/         # Build and automation scripts
└── docs/            # Documentation
```

## Building VoyageOS

```bash
make         # Build everything (requires GCC, NASM, ld)
make clean   # Clean build artifacts
make run     # Run in QEMU emulator
make iso     # Create bootable ISO
```

## System Architecture

### Bootloader
- 512-byte MBR bootloader
- Stage 2 bootloader for long mode setup
- Memory detection and GDT initialization

### Kernel
- Interrupt handling (IDT)
- Memory management (paging, heap allocation)
- Process/task management
- Virtual filesystem

### VoyageIDE
- Text editor with syntax highlighting
- Integrated compiler
- Debugger interface
- Real-time code compilation

### Compiler (VoyageC)
- C-like syntax with assembly interop
- Fast compilation to native code
- REPL for interactive development

## Development

The primary way to develop VoyageOS is through VoyageIDE, which runs within the OS itself. This allows for:
- Writing kernel code in VoyageC
- Immediate compilation and testing
- Hot-reloading of modules (future)
- Iterative OS development

## Current Status

🚀 Early development phase
- [ ] Basic bootloader
- [ ] Long mode (64-bit) support
- [ ] Memory management
- [ ] VoyageIDE (text editor + REPL)
- [ ] Basic syscalls
- [ ] Filesystem
- [ ] Networking (future)

## Building on macOS/Linux Host

Requirements:
- `nasm` - Assembler
- `gcc` - C compiler
- `ld` - Linker
- `qemu` - Emulator (optional, for testing)
- `xorriso` - ISO creation (optional)

```bash
# Ubuntu/Debian
sudo apt-get install nasm gcc qemu-system-x86 xorriso

# macOS
brew install nasm gcc qemu xorriso
```

## License

This is a personal hobby project. Use and modify freely.

---

*VoyageOS: Journey through your own operating system.*
