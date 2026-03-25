# VoyageOS Build & Run Guide

## Quick Start

```bash
cd /home/jasmine/Desktop/VoyageOS

# Build everything
make all

# Create bootable floppy image
make floppy

# Run in QEMU with separate window and audio
make run
```

## Build System Overview

### What Gets Built

| Component | Output | Size | Purpose |
|-----------|--------|------|---------|
| **Bootloader** | `build/boot.bin` | 512 bytes | MBR, real→protected mode transition |
| **Kernel** | `build/kernel.bin` | ~3KB | 32-bit protected mode kernel |
| **Floppy Image** | `build/floppy.img` | 1.44 MB | Bootable disk image for QEMU |

### Build Architecture

```
bootloader.asm (NASM)
    ↓
boot.bin (512 bytes)
    ↓
    ├─→ Sector 0 of floppy.img (MBR with boot signature)
    │
kernel/
    ├─→ entry.asm (NASM 32-bit)
    │   ↓
    │   entry.o (ELF32)
    │
    ├─→ kernel.c (GCC with -m32)
    │   ↓
    │   kernel.o (ELF32)
    │
    └─→ Linked with LD at 0x10000
        ↓
        kernel.bin (~3KB)
        ↓
        Sectors 1+ of floppy.img
```

### Memory Layout

```
0x00000000 ┌─────────────────────────┐
           │  IVT (Interrupt Vectors)│ 1 KB
0x00000400 ├─────────────────────────┤
           │  BIOS Data Area         │ 2 KB
0x00000C00 ├─────────────────────────┤
           │  Unused                 │
0x00007C00 │  Bootloader (MBR)       │ 512 bytes
0x00007E00 ├─────────────────────────┤
           │  Boot Stack & Data      │
0x00100000 ├─────────────────────────┤  ← 1 MB boundary
           │  (Protected Mode Area)  │
0x00010000 │  Kernel @ 0x10000       │
           │  (Linked address)       │
0x00010C00 │  Kernel Stack @ 0x1F000│
           └─────────────────────────┘
```

### Makefile Targets

```bash
make boot          # Build bootloader only
make kernel        # Build kernel only
make all           # Build bootloader and kernel
make floppy        # Create bootable floppy image
make run           # Launch in QEMU (GTK window + audio)
make clean         # Remove all build artifacts
make help          # Display help message
make info          # Show build configuration
```

## QEMU Execution Modes

The `make run` target uses intelligent fallback chains to ensure compatibility:

### Primary Mode (Best)
```bash
qemu-system-x86_64 \
    -fda build/floppy.img \
    -m 256 \
    -display gtk \
    -audiodev pa,id=audio0 \
    -machine pcspk-audiodev=audio0
```
- **Display**: GTK (separate window)
- **Audio**: PulseAudio (PC speaker routed to speakers)

### Fallback 1 (Alt Linux)
```bash
qemu-system-x86_64 \
    -fda build/floppy.img \
    -m 256 \
    -display sdl \
    -audiodev pulse,id=audio0 \
    -machine pcspk-audiodev=audio0
```
- **Display**: SDL (separate window)
- **Audio**: PulseAudio

### Fallback 2 (Minimal)
```bash
qemu-system-x86_64 \
    -fda build/floppy.img \
    -m 256
```
- **Display**: Default (usually curses/text)
- **Audio**: None

## Manual QEMU Launch

If you prefer manual control:

```bash
# With all features
qemu-system-x86_64 \
    -fda build/floppy.img \
    -m 256 \
    -display gtk \
    -audiodev pa,id=audio0 \
    -machine pcspk-audiodev=audio0 \
    -name "VoyageOS" \
    -serial stdio

# Exit: Ctrl+Alt+Q or close window
```

## Build Configuration

### Compiler Flags
```makefile
NASM    = nasm                                    # Assembler
GCC     = gcc                                     # C Compiler
LD      = ld                                      # Linker
CFLAGS  = -ffreestanding -fno-stack-protector -m32  # GCC flags
LDFLAGS = -m elf_i386 -nostdlib -Ttext 0x10000  # Linker flags
```

### Key Options Explained
- `-ffreestanding`: No standard library, bare metal
- `-fno-stack-protector`: No stack canary checks
- `-m32`: Compile for 32-bit (even on 64-bit system)
- `-nostdlib`: Don't link standard library
- `-Ttext 0x10000`: Link kernel at physical address 0x10000

## Troubleshooting

### Build Fails: "NASM not found"
```bash
sudo apt-get install nasm
```

### Build Fails: "gcc: command not found" or wrong architecture
```bash
sudo apt-get install gcc-multilib  # For 32-bit support on 64-bit system
```

### QEMU won't start
- Try `make run` (uses fallback chain)
- Or run manually with basic mode: `qemu-system-x86_64 -fda build/floppy.img`

### No audio in VM
- Check PulseAudio: `pactl list sinks`
- Try fallback: `make run` will auto-retry with SDL/none

### Blank VGA display
- Bootloader/kernel communication issue
- Check `scripts/verify-boot.sh` for boot signature
- Review [BOOT_FIXES_DETAILED.md](BOOT_FIXES_DETAILED.md)

## Verification Scripts

```bash
./scripts/verify-boot.sh      # Check boot signature
./scripts/test-build.sh       # Test compilation
```

## Development Workflow

### 1. Edit Source
```bash
vim bootloader/bootloader.asm    # Edit bootloader
vim kernel/kernel.c              # Edit kernel
vim kernel/entry.asm             # Edit kernel entry
```

### 2. Rebuild
```bash
make all    # Clean build if source changed significantly
make run    # Test in QEMU
```

### 3. Debug
```bash
# Check build output
ls -lh build/
file build/floppy.img

# View binary contents
hexdump -C build/boot.bin | head
strings build/kernel.bin

# Check build logs in terminal
```

## Performance Notes

- **Build Time**: ~1-2 seconds (very fast)
- **Boot Time**: ~1-2 seconds from QEMU launch
- **RAM Usage**: 256 MB allocated to VM
- **Disk Image**: 1.44 MB (standard floppy size)

## Next Steps for Development

1. **Add Interrupt Handling**: Implement IDT and exception handlers
2. **Memory Management**: Set up paging and virtual memory
3. **VoyageIDE**: Complete interactive text editor interface
4. **Compiler**: Finish VoyageC compiler implementation
5. **Filesystem**: Implement simple block device and FAT12
6. **System Calls**: Add syscall interface for user programs

---

**Last Updated**: After implementing separate window display and audio support
**Build Status**: ✅ Complete and functional
**Boot Status**: ✅ Bootable floppy image verified
