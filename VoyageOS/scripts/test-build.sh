#!/bin/bash
# VoyageOS Test and Build Script

set -e

cd /home/jasmine/Desktop/VoyageOS

echo "================================================"
echo "VoyageOS Build and Test"
echo "================================================"
echo ""

echo "Step 1: Cleaning old build artifacts..."
rm -rf build/
echo "✓ Clean complete"
echo ""

echo "Step 2: Building bootloader..."
mkdir -p build
nasm -f bin bootloader/bootloader.asm -o build/boot.bin
BOOT_SIZE=$(wc -c < build/boot.bin)
echo "✓ Bootloader: $BOOT_SIZE bytes"
echo ""

echo "Step 3: Building kernel..."
echo "  - Assembling entry.asm..."
nasm -f elf32 kernel/entry.asm -o build/entry.o
echo "  - Compiling kernel.c..."
gcc -ffreestanding -fno-stack-protector -fno-builtin -m32 -Wall -Wextra -Isrc -c kernel/kernel.c -o build/kernel.o
echo "  - Linking kernel.bin..."
ld -m elf_i386 -nostdlib -Ttext 0x10000 -o build/kernel.bin build/entry.o build/kernel.o
KERNEL_SIZE=$(wc -c < build/kernel.bin)
echo "✓ Kernel: $KERNEL_SIZE bytes"
echo ""

echo "Step 4: Creating bootable floppy image..."
# Create 1.44MB floppy image
dd if=/dev/zero of=build/floppy.img bs=512 count=2880 2>/dev/null
# Write bootloader (sector 0)
dd if=build/boot.bin of=build/floppy.img bs=512 count=1 conv=notrunc 2>/dev/null
# Write kernel (starting at sector 1)
dd if=build/kernel.bin of=build/floppy.img bs=512 seek=1 conv=notrunc 2>/dev/null
FLOPPY_SIZE=$(wc -c < build/floppy.img)
echo "✓ Floppy image: $FLOPPY_SIZE bytes"
echo ""

echo "================================================"
echo "Build Summary:"
echo "================================================"
echo "Bootloader:   $BOOT_SIZE bytes (must be exactly 512)"
echo "Kernel:       $KERNEL_SIZE bytes"
echo "Floppy Image: $FLOPPY_SIZE bytes (1.44 MB)"
echo ""

if [ $BOOT_SIZE -eq 512 ]; then
    echo "✓ Bootloader size: VALID"
else
    echo "✗ Bootloader size: INVALID (expected 512, got $BOOT_SIZE)"
    exit 1
fi

echo ""
echo "================================================"
echo "Build Successful!"
echo "================================================"
echo ""
echo "To test in QEMU, run:"
echo "  qemu-system-x86_64 -fda build/floppy.img -m 256"
echo ""
echo "Or use: make run"
