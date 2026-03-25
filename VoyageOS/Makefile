# VoyageOS Makefile
# Main build script for the entire OS

.PHONY: all clean run iso help boot kernel voyageide

# Build variables
NASM = nasm
GCC = gcc
LD = ld
QEMU = qemu-system-x86_64

CFLAGS = -ffreestanding -fno-stack-protector -fno-builtin -m64 -Wall -Wextra -Isrc
LDFLAGS = -m elf_x86_64 -nostdlib -Ttext 0x10000

BUILD_DIR = build
BOOT_DIR = bootloader
KERNEL_DIR = kernel
IDE_DIR = voyageide
COMPILER_DIR = compiler

# Default target
all: boot kernel
	@echo "✓ VoyageOS built successfully!"
	@echo "Run 'make run' to test in QEMU"

help:
	@echo "VoyageOS Build System"
	@echo "====================\n"
	@echo "Available targets:"
	@echo "  make boot       - Build bootloader only"
	@echo "  make kernel     - Build kernel only"
	@echo "  make all        - Build everything"
	@echo "  make run        - Run OS in QEMU"
	@echo "  make clean      - Clean build artifacts"
	@echo "  make help       - Show this message"

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Bootloader targets
boot: $(BUILD_DIR) $(BUILD_DIR)/boot.bin
	@echo "✓ Bootloader compiled"

$(BUILD_DIR)/boot.bin: $(BOOT_DIR)/bootloader.asm
	@echo "Assembling bootloader..."
	$(NASM) -f bin $(BOOT_DIR)/bootloader.asm -o $(BUILD_DIR)/boot.bin
	@echo "  Size: $$(wc -c < $(BUILD_DIR)/boot.bin) bytes"

# Kernel targets
kernel: $(BUILD_DIR) $(BUILD_DIR)/kernel.bin
	@echo "✓ Kernel compiled"

$(BUILD_DIR)/entry.o: $(KERNEL_DIR)/entry.asm
	@echo "Assembling kernel entry..."
	$(NASM) -f elf64 $(KERNEL_DIR)/entry.asm -o $(BUILD_DIR)/entry.o

$(BUILD_DIR)/isr.o: $(KERNEL_DIR)/isr.asm
	@echo "Assembling ISRs..."
	$(NASM) -f elf64 $(KERNEL_DIR)/isr.asm -o $(BUILD_DIR)/isr.o

$(BUILD_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c
	@echo "Compiling kernel C code..."
	$(GCC) $(CFLAGS) -c $(KERNEL_DIR)/kernel.c -o $(BUILD_DIR)/kernel.o

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/entry.o $(BUILD_DIR)/isr.o $(BUILD_DIR)/kernel.o
	@echo "Linking kernel to ELF..."
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/entry.o $(BUILD_DIR)/isr.o $(BUILD_DIR)/kernel.o
	@echo "  Size ELF: $$(wc -c < $(BUILD_DIR)/kernel.elf) bytes"

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.elf
	@echo "Generating flat binary kernel..."
	objcopy -O binary $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.bin
	@echo "  Size BIN: $$(wc -c < $(BUILD_DIR)/kernel.bin) bytes"

# Run in QEMU with separate window and audio
run: floppy
	@echo "Launching VoyageOS in QEMU (separate window)..."
	@echo "Close the QEMU window to exit"
	@qemu-system-x86_64 \
		-fda $(BUILD_DIR)/floppy.img \
		-m 256 \
		-display gtk \
		-audiodev pa,id=audio0 \
		-machine pcspk-audiodev=audio0 \
		-k en-us \
		-name "VoyageOS" \
		2>/dev/null || \
	qemu-system-x86_64 \
		-fda $(BUILD_DIR)/floppy.img \
		-m 256 \
		-display sdl \
		-audiodev pulse,id=audio0 \
		-machine pcspk-audiodev=audio0 \
		2>/dev/null || \
	qemu-system-x86_64 \
		-fda $(BUILD_DIR)/floppy.img \
		-m 256 \
		2>/dev/null || true

# Create floppy image
floppy: all
	@echo "Creating floppy image..."
	@dd if=/dev/zero of=$(BUILD_DIR)/floppy.img bs=1024 count=1440 2>/dev/null
	@dd if=$(BUILD_DIR)/boot.bin of=$(BUILD_DIR)/floppy.img bs=512 count=1 conv=notrunc 2>/dev/null
	@dd if=$(BUILD_DIR)/kernel.bin of=$(BUILD_DIR)/floppy.img bs=512 seek=1 conv=notrunc 2>/dev/null
	@echo "✓ Floppy image created: $(BUILD_DIR)/floppy.img"

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
	@echo "✓ Clean complete"

# Debug info
info:
	@echo "VoyageOS Build Configuration"
	@echo "============================"
	@echo "Assembler: $(NASM)"
	@echo "Compiler: $(GCC)"
	@echo "Linker: $(LD)"
	@echo "Emulator: $(QEMU)"
	@echo "Build Dir: $(BUILD_DIR)"
