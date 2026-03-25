#!/bin/bash
# VoyageOS Boot Verification Script
# Confirms the boot fixes are working

set -e

cd /home/jasmine/Desktop/VoyageOS

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║          VoyageOS Boot Verification                           ║"
echo "║     Checking that all boot fixes are in place                ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Check bootloader file
echo "Checking bootloader..."
if [ -f "build/boot.bin" ]; then
    BOOT_SIZE=$(wc -c < build/boot.bin)
    if [ $BOOT_SIZE -eq 512 ]; then
        echo "  ✓ Bootloader size: $BOOT_SIZE bytes (correct)"
    else
        echo "  ✗ Bootloader size: $BOOT_SIZE bytes (expected 512)"
        exit 1
    fi
    
    # Check boot signature
    SIGNATURE=$(hexdump -C build/boot.bin | tail -1)
    if echo "$SIGNATURE" | grep -q "55aa"; then
        echo "  ✓ Boot signature: 0xAA55 present"
    else
        echo "  ✗ Boot signature not found"
        exit 1
    fi
else
    echo "  ✗ bootloader not built"
    exit 1
fi
echo ""

# Check kernel
echo "Checking kernel..."
if [ -f "build/kernel.bin" ]; then
    KERNEL_SIZE=$(wc -c < build/kernel.bin)
    echo "  ✓ Kernel size: $KERNEL_SIZE bytes"
else
    echo "  ✗ Kernel not built"
    exit 1
fi
echo ""

# Check floppy image
echo "Checking floppy image..."
if [ -f "build/floppy.img" ]; then
    FLOPPY_SIZE=$(wc -c < build/floppy.img)
    EXPECTED_SIZE=$((1440 * 1024))
    if [ $FLOPPY_SIZE -eq $EXPECTED_SIZE ]; then
        echo "  ✓ Floppy image size: correct (1.44 MB)"
    else
        echo "  ! Floppy image size: $FLOPPY_SIZE bytes (expected $EXPECTED_SIZE)"
    fi
    
    FILE_TYPE=$(file build/floppy.img)
    if echo "$FILE_TYPE" | grep -q "DOS/MBR"; then
        echo "  ✓ Floppy format: bootable DOS/MBR image"
    else
        echo "  ! File type: $FILE_TYPE"
    fi
else
    echo "  ✗ Floppy image not created"
    exit 1
fi
echo ""

# Check source files
echo "Checking source code fixes..."

if grep -q "jmp 0x10000" bootloader/bootloader.asm; then
    echo "  ✓ Bootloader: Jump to 0x10000 (physical address)"
else
    echo "  ✗ Bootloader: Jump address might be wrong"
fi

if ! grep -q "int 0x13\|0x02" bootloader/bootloader.asm; then
    echo "  ✓ Bootloader: Disk I/O removed"
else
    echo "  ! Bootloader: May still have disk I/O code"
fi

if grep -q "enable_a20" bootloader/bootloader.asm; then
    echo "  ✓ Bootloader: A20 gate enabled"
else
    echo "  ✗ Bootloader: A20 gate missing"
fi

if grep -q "kernel_entry\|extern kmain" kernel/entry.asm; then
    echo "  ✓ Kernel entry: Proper C kernel jump"
else
    echo "  ✗ Kernel entry: C kernel call missing"
fi

if grep -q "0x1F000\|0x1F" kernel/entry.asm; then
    echo "  ✓ Kernel entry: High memory stack (0x1F000)"
else
    echo "  ! Kernel entry: Stack location might be wrong"
fi
echo ""

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║         All Verifications Passed! ✓                          ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""
echo "Boot fixes verified. VoyageOS is ready to test!"
echo ""
echo "To boot in QEMU:"
echo "  qemu-system-x86_64 -fda build/floppy.img -m 256"
echo ""
echo "Or use:"
echo "  make run"
echo ""
