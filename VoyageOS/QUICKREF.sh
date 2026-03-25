#!/bin/bash
# VoyageOS Quick Reference Card
# Enhanced with window display and audio support

cat << 'EOF'
╔════════════════════════════════════════════════════════════════════╗
║                      VoyageOS Quick Reference                     ║
║              Building a Self-Hosted x86-64 Operating System        ║
║             (With Separate Window Display & Audio Support)         ║
╚════════════════════════════════════════════════════════════════════╝

━━━━━━━━━━━━━━━━━━━━━ QUICK START COMMANDS ━━━━━━━━━━━━━━━━━━━━━

  cd /home/jasmine/Desktop/VoyageOS
  make all         # Build everything  
  make run         # Launch in QEMU (GTK window + PulseAudio)

━━━━━━━━━━━━━━━━━━━━━ BUILD TARGETS ━━━━━━━━━━━━━━━━━━━━━━━━━

  make boot        ← Build bootloader (boot.bin, 512 bytes)
  make kernel      ← Build kernel (kernel.bin, ~3 KB)
  make all         ← Build both
  make floppy      ← Create bootable image (1.44 MB)
  make run         ← Launch QEMU with GTK + audio
  make clean       ← Remove all artifacts
  make help        ← Show targets
  make info        ← Show config

━━━━━━━━━━━━━━━━━━━ BUILD OUTPUT FILES ━━━━━━━━━━━━━━━━━━━━

  build/boot.bin       512 bytes   (MBR bootloader)
  build/kernel.bin     ~3 KB       (32-bit kernel)
  build/floppy.img     1.44 MB     (Bootable disk)
  build/entry.o                    (Kernel entry object)
  build/kernel.o                   (Kernel C object)

━━━━━━━━━━━━━━━ QEMU EXECUTION MODES ━━━━━━━━━━━━━━━━━

  GTK Window (Default)
  ├─ Display: Separate window (best for x11)
  ├─ Audio:   PulseAudio (PC speaker routed to speakers)
  └─ Command: make run

  SDL Window (Fallback 1)
  ├─ Display: SDL graphics
  ├─ Audio:   PulseAudio
  └─ Used if GTK not available

  Text Mode (Fallback 2)
  ├─ Display: Terminal/curses
  └─ Audio:   None
  └─ Used if GUI unavailable

  Manual Launch:
  $ qemu-system-x86_64 -fda build/floppy.img -m 256 \
      -display gtk -audiodev pa,id=audio0 \
      -machine pcspk-audiodev=audio0

  Exit QEMU: Ctrl+Alt+Q (or close window)

━━━━━━━━━━━━━━━━━━━ SOURCE FILES ━━━━━━━━━━━━━━━━━━━━

  bootloader/bootloader.asm   Real mode → Protected mode
  kernel/entry.asm            32-bit protected mode entry
  kernel/kernel.c             Main kernel (VGA, kmain)
  voyageide/ide.c             IDE framework
  compiler/voyagec.c          VoyageC compiler
  docs/DEVELOPMENT.md         Development guide
  docs/ARCHITECTURE.md        System design
  docs/CODING_STANDARD.md     Code style
  Makefile                    Build system
  BUILD_GUIDE.md              Comprehensive build docs

━━━━━━━━━━━━━━━━━━ MEMORY LAYOUT ━━━━━━━━━━━━━━━━━━━━━

  Boot Sequence:
  0x7C00 → Bootloader (real mode, 512 bytes)
    ↓ (enable A20, setup GDT)
  0x10000 → Kernel (32-bit protected mode)
    ↓ (initialize, call kmain)
  0x1F000 → Kernel Stack
  0xB8000 → VGA Memory (80×25 text)

━━━━━━━━━━━━━━━━━━ FIRST TIME SETUP ━━━━━━━━━━━━━━━━━━━

  $ cd /home/jasmine/Desktop/VoyageOS
  $ make all          # Build everything
  $ make run          # Launch QEMU (GTK + audio)

━━━━━━━━━━━━━━━━━━ NEXT TIME ━━━━━━━━━━━━━━━━━━━━━

  Edit → Build → Test
  $ vim kernel/kernel.c       # Edit kernel
  $ make                       # Rebuild
  $ make run                   # Test

BUILD COMMANDS
═════════════════════════════════════════════════════════════════════

Using Make:
  make              - Build all (bootloader, kernel, IDE)
  make boot         - Bootloader only
  make kernel       - Kernel only
  make voyageide    - VoyageIDE only
  make run          - Build and run in QEMU
  make iso          - Create bootable ISO
  make clean        - Remove build artifacts
  make help         - Show help

Using Scripts:
  bash scripts/build.sh all       - Build everything
  bash scripts/build.sh run       - Build and run
  bash scripts/build.sh compiler  - Build compiler
  bash scripts/build.sh clean     - Clean artifacts
  bash scripts/setup.sh           - Installation wizard

═════════════════════════════════════════════════════════════════════

━━━━━━━━━━━━━━━ SYSTEM REQUIREMENTS ━━━━━━━━━━━━━━━

  Build Tools:
  ✓ nasm          – Assembler (check: nasm -v)
  ✓ gcc -m32      – C compiler with 32-bit (gcc -v)
  ✓ ld            – Linker (ld -v)
  ✓ make          – Build tool (make --version)

  Emulation:
  ✓ qemu-system-x86_64  – QEMU (qemu-system-x86_64 -version)

  Graphics/Audio:
  ✓ PulseAudio (pa) – For audio support (pactl --version)
  ✓ GTK or SDL    – For window display

  Install on Linux:
  $ sudo apt-get install nasm gcc gcc-multilib make \
      binutils qemu-system-x86 pulseaudio

━━━━━━━━━━━━━━━ TROUBLESHOOTING ━━━━━━━━━━━━━━━

  make: command not found
  → Install GNU Make: sudo apt-get install make

  nasm: command not found
  → Install NASM: sudo apt-get install nasm

  gcc: error: unrecognized command line option '-m32'
  → Install 32-bit support: sudo apt-get install gcc-multilib

  QEMU won't open separate window
  → make run auto-tries fallbacks (GTK → SDL → basic)
  → Or install: sudo apt-get install qemu-system-x86 libsdl2-2.0

  No audio heard from VM
  → Check PulseAudio: pactl list sinks
  → Kernel might not have PC speaker code
  → Try basic mode: qemu-system-x86_64 -fda build/floppy.img

  Boot shows blank screen
  → Check boot signature: hexdump -C build/boot.bin | tail
  → Should end with: 0000200  aa55
  → Review: docs/BOOT_FIXES_DETAILED.md

═════════════════════════════════════════════════════════════════════

COMPONENTS
═════════════════════════════════════════════════════════════════════

BOOTLOADER (bootloader/bootloader.asm):
  - Real mode initialization
  - A20 gate enabling
  - GDT setup (protected mode)
  - Paging setup
  - Long mode (64-bit) entry
  
  Status: Functional
  Size: ~512 bytes (MBR)

KERNEL (kernel/kernel.c, kernel/entry.asm):
  - 64-bit kernel entry point
  - VGA text output (80x25)
  - Basic exception handling
  - Memory management (basics)
  - System initialization
  - Interrupt dispatcher (stub)
  
  Status: Early stage
  In progress: Interrupt handling, memory management

VOYAGEIDE (voyageide/ide.c):
  - Text editor interface
  - Syntax highlighting preparation
  - REPL support structure
  - Compiler integration
  - File management interface
  
  Status: UI framework built
  In progress: Interactive editing

VOYAGEC COMPILER (compiler/voyagec.c):
  - Lexer/tokenizer structure
  - AST definition
  - Parser framework
  - Code generator design
  
  Status: Early stage
  In progress: Full parser, code generation

═════════════════════════════════════════════════════════════════════

FILE FORMATS
═════════════════════════════════════════════════════════════════════

Bootloader:  NASM assembly (.asm) → binary
Kernel:      C (.c) + NASM assembly (.asm) → object → binary
IDE:         C (.c) → compiled object
Compiler:    C (.c) → standalone executable
Programs:    VoyageC (.voyagec) → assembly → binary
Docs:        Markdown (.md), VoyageC (.voyagec)

═════════════════════════════════════════════════════════════════════

CODING STYLE (See docs/CODING_STANDARD.md)
═════════════════════════════════════════════════════════════════════

C Code:
  - snake_case for functions/variables
  - PascalCase for types/structs
  - 4-space indentation
  - K&R brace style
  - Max 100 chars per line
  - // for single-line comments
  - /* */ for multi-line comments

Assembly:
  - lower_snake_case for labels
  - 4-space indentation for instructions
  - Meaningful comments

VoyageC Code:
  - fn function_name() syntax
  - Similar conventions to C
  - More readable than system C

═════════════════════════════════════════════════════════════════════

COMMON TASKS
═════════════════════════════════════════════════════════════════════

Add a new ISR (Interrupt Service Routine):
  1. Declare in kernel/interrupts.c (not yet created)
  2. Add handler function
  3. Register in IDT
  4. Test with interrupt trigger

Implement a syscall:
  1. Add syscall number to kernel/syscall.h
  2. Write handler in kernel/syscall.c
  3. Test from user space

Create a library function:
  1. Implement in kernel/ or lib/
  2. Add to public header file
  3. Document in docs/API.md
  4. Add examples to docs/EXAMPLES.voyagec

═════════════════════════════════════════════════════════════════════

EMULATION
═════════════════════════════════════════════════════════════════════

Run VoyageOS:
  make run          - Launches QEMU with built boot image
  
In QEMU:
  Ctrl+Alt+2        - Switch to monitor console
  quit              - Exit QEMU (from monitor)
  Ctrl+A, X         - Kill QEMU (from main window)

Debug VoyageOS:
  Terminal 1:
    qemu-system-x86_64 -s -S -fda build/boot.bin
  
  Terminal 2:
    gdb build/kernel.bin
    (gdb) target remote localhost:1234
    (gdb) break main
    (gdb) continue

═════════════════════════════════════════════════════════════════════

DEPENDENCIES TARGET PLATFORMS
═════════════════════════════════════════════════════════════════════

Linux (Ubuntu/Debian):
  nasm gcc binutils make qemu-system-x86

Linux (Fedora/CentOS):
  nasm gcc binutils make qemu-kvm

macOS:
  nasm gcc binutils make qemu (via Homebrew)

Windows:
  Use WSL (Windows Subsystem for Linux) and follow Linux steps
  Or: Use MSYS2 with GCC/NASM installed

═════════════════════════════════════════════════════════════════════

DOCUMENTATION
═════════════════════════════════════════════════════════════════════

Overview:
  README.md             - Project introduction & building
  
Getting Started:
  docs/DEVELOPMENT.md   - How to develop for VoyageOS
  
Technical Reference:
  docs/ARCHITECTURE.md  - System design & internals
  
Code:
  docs/CODING_STANDARD.md   - Style guidelines
  docs/EXAMPLES.voyagec     - Sample VoyageC programs
  
Planning:
  ROADMAP.md           - Development timeline & roadmap

═════════════════════════════════════════════════════════════════════

KEYBOARD SHORTCUTS (When IDE is implemented)
═════════════════════════════════════════════════════════════════════

Editor Mode:
  Ctrl+S      - Save current file
  Ctrl+O      - Open file
  Ctrl+N      - New file
  Ctrl+C      - Compile file
  F1          - Help/documentation
  F2          - Save
  F3          - Compile
  F4          - Toggle REPL
  ESC         - Show main menu

REPL Mode:
  Enter       - Evaluate expression
  Ctrl+C      - Clear current line
  exit        - Return to editor

═════════════════════════════════════════════════════════════════════

TROUBLESHOOTING
═════════════════════════════════════════════════════════════════════

Build fails with "nasm not found":
  → Install NASM: sudo apt-get install nasm (Linux)
                  brew install nasm (macOS)

Build fails with "gcc not found":
  → Install GCC: sudo apt-get install gcc
                 brew install gcc

QEMU won't start:
  → Install QEMU: sudo apt-get install qemu-system-x86
                  brew install qemu

Can't write to bootloader area:
  → You likely have permission issues
  → Ensure you're in the correct directory
  → Check that build/ directory is writable

═════════════════════════════════════════════════════════════════════

NEXT STEPS
═════════════════════════════════════════════════════════════════════

1. Read docs/DEVELOPMENT.md for detailed development guide
2. Understand docs/ARCHITECTURE.md for system design
3. Build and run: make run
4. Examine the generated code in build/
5. Start modifying kernel/kernel.c to add features
6. Work on compiler in compiler/voyagec.c
7. Implement IDE interactivity in voyageide/ide.c

═════════════════════════════════════════════════════════════════════

Random Inspiration:
  "Write your own operating system. Understand the machine."
  → VoyageOS is your journey into systems programming.

═════════════════════════════════════════════════════════════════════

For more information, see README.md and the docs/ folder.
Happy coding! 🚀

EOF
