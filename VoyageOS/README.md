# VoyageOS

A modernized TempleOS-like operating system.

## Features

- 64-bit kernel
- Keyboard and mouse support
- HolyC-like compiler and interpreter
- Graphics and sound capabilities
- Command shell
- File system

## Building

To build the OS, you need a cross-compiler for x86_64.

Install binutils and GCC for x86_64-elf.

Then, run `make` in the root directory.

## Running

Use QEMU to run the OS image.

`qemu-system-x86_64 -kernel VoyageOS.bin`

This will boot the bare metal OS, display the welcome message and shell prompt, and wait for keyboard input.

## Structure

- `src/`: Source code
  - `kernel/`: Kernel code
  - `drivers/`: Device drivers
  - `shell/`: Shell implementation
  - `compiler/`: HolyC compiler
  - `interpreter/`: Interpreter for scripts
  - `fs/`: File system
- `include/`: Header files
- `boot/`: Bootloader
- `scripts/`: Build scripts
- `docs/`: Documentation