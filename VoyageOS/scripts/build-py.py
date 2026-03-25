#!/usr/bin/env python3
"""
VoyageOS Build System
Compiles bootloader, kernel, and creates bootable floppy image
"""

import os
import subprocess
import sys

def run_command(cmd, description):
    """Run a shell command and return success/failure"""
    print(f"  {description}...", end=" ", flush=True)
    try:
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True, timeout=30)
        if result.returncode == 0:
            print("✓")
            return True
        else:
            print("✗")
            print(f"    Error: {result.stderr}")
            return False
    except Exception as e:
        print(f"✗\n    Error: {e}")
        return False

def get_file_size(path):
    """Get file size in bytes"""
    try:
        return os.path.getsize(path)
    except:
        return 0

def main():
    os.chdir("/home/jasmine/Desktop/VoyageOS")
    
    print("╔════════════════════════════════════════╗")
    print("║      VoyageOS Build System             ║")
    print("╚════════════════════════════════════════╝")
    print()
    
    # Clean
    print("Cleaning old build artifacts...")
    subprocess.run("rm -rf build", shell=True)
    subprocess.run("mkdir -p build", shell=True)
    print()
    
    # Build bootloader
    print("Step 1: Building Bootloader")
    if not run_command("nasm -f bin bootloader/bootloader.asm -o build/boot.bin", "Assembling"):
        print("BUILD FAILED: Bootloader assembly error")
        return 1
    
    boot_size = get_file_size("build/boot.bin")
    print(f"       Size: {boot_size} bytes")
    if boot_size != 512:
        print(f"ERROR: Bootloader must be exactly 512 bytes (got {boot_size})")
        return 1
    print()
    
    # Build kernel
    print("Step 2: Building Kernel")
    if not run_command("nasm -f elf32 kernel/entry.asm -o build/entry.o", "Assembling entry"):
        print("BUILD FAILED: Kernel entry assembly error")
        return 1
    
    if not run_command("gcc -ffreestanding -fno-stack-protector -fno-builtin -m32 -Wall -Wextra -Isrc -c kernel/kernel.c -o build/kernel.o", "Compiling kernel.c"):
        print("BUILD FAILED: Kernel C compilation error")
        return 1
    
    if not run_command("ld -m elf_i386 -nostdlib -Ttext 0x10000 -o build/kernel.bin build/entry.o build/kernel.o", "Linking kernel"):
        print("BUILD FAILED: Kernel linking error")
        return 1
    
    kernel_size = get_file_size("build/kernel.bin")
    print(f"       Size: {kernel_size} bytes")
    print()
    
    # Create floppy image
    print("Step 3: Creating Bootable Floppy Image")
    if not run_command("dd if=/dev/zero of=build/floppy.img bs=512 count=2880 2>/dev/null", "Creating 1.44MB image"):
        print("BUILD FAILED: Floppy creation error")
        return 1
    
    if not run_command("dd if=build/boot.bin of=build/floppy.img bs=512 count=1 conv=notrunc 2>/dev/null", "Writing bootloader"):
        print("BUILD FAILED: Bootloader write error")
        return 1
    
    if not run_command("dd if=build/kernel.bin of=build/floppy.img bs=512 seek=1 conv=notrunc 2>/dev/null", "Writing kernel"):
        print("BUILD FAILED: Kernel write error")
        return 1
    
    floppy_size = get_file_size("build/floppy.img")
    print(f"       Size: {floppy_size} bytes")
    print()
    
    # Summary
    print("╔════════════════════════════════════════╗")
    print("║        BUILD SUCCESSFUL! ✓            ║")
    print("╚════════════════════════════════════════╝")
    print()
    print("Files created:")
    print(f"  - build/boot.bin       {boot_size} bytes (bootloader)")
    print(f"  - build/kernel.bin     {kernel_size} bytes (kernel)")
    print(f"  - build/floppy.img     {floppy_size} bytes (bootable image)")
    print()
    print("To test in QEMU, run:")
    print("  qemu-system-x86_64 -fda build/floppy.img -m 256")
    print()
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
