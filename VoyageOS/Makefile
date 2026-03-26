NASM = nasm
GCC = gcc
LD = ld
QEMU = qemu-system-x86_64

CFLAGS = -ffreestanding -fno-stack-protector -fno-builtin -m64 -Wall -Wextra -Ikernel
LDFLAGS = -m elf_x86_64 -nostdlib -Ttext 0x10000 -z noexecstack

BUILD_DIR = build

all: $(BUILD_DIR) boot kernel ide
	@echo "VoyageOS + VoyageIDE built successfully!"

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

boot: $(BUILD_DIR)/boot.bin

$(BUILD_DIR)/boot.bin: bootloader/bootloader.asm
	$(NASM) -f bin bootloader/bootloader.asm -o $(BUILD_DIR)/boot.bin

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/entry.o: kernel/entry.asm
	$(NASM) -f elf64 kernel/entry.asm -o $(BUILD_DIR)/entry.o

$(BUILD_DIR)/isr.o: kernel/isr.asm
	$(NASM) -f elf64 kernel/isr.asm -o $(BUILD_DIR)/isr.o

$(BUILD_DIR)/kernel.o: kernel/kernel.c
	$(GCC) $(CFLAGS) -c kernel/kernel.c -o $(BUILD_DIR)/kernel.o

$(BUILD_DIR)/stdlib.o: kernel/stdlib.c
	$(GCC) $(CFLAGS) -c kernel/stdlib.c -o $(BUILD_DIR)/stdlib.o

$(BUILD_DIR)/io.o: kernel/io.c
	$(GCC) $(CFLAGS) -c kernel/io.c -o $(BUILD_DIR)/io.o

$(BUILD_DIR)/timer.o: kernel/timer.c
	$(GCC) $(CFLAGS) -c kernel/timer.c -o $(BUILD_DIR)/timer.o

$(BUILD_DIR)/init.o: kernel/init.c
	$(GCC) $(CFLAGS) -c kernel/init.c -o $(BUILD_DIR)/init.o

$(BUILD_DIR)/ide.o: voyageide/ide.c
	$(GCC) $(CFLAGS) -c voyageide/ide.c -o $(BUILD_DIR)/ide.o

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/entry.o $(BUILD_DIR)/isr.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/stdlib.o $(BUILD_DIR)/io.o $(BUILD_DIR)/timer.o $(BUILD_DIR)/init.o $(BUILD_DIR)/ide.o
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.elf
	objcopy -O binary $< $@

ide: $(BUILD_DIR)/ide.o

floppy: all
	dd if=/dev/zero of=$(BUILD_DIR)/floppy.img bs=1024 count=1440 2>/dev/null
	dd if=$(BUILD_DIR)/boot.bin of=$(BUILD_DIR)/floppy.img bs=512 count=1 conv=notrunc
	dd if=$(BUILD_DIR)/kernel.bin of=$(BUILD_DIR)/floppy.img bs=512 seek=1 conv=notrunc

run: floppy
	$(QEMU) -fda $(BUILD_DIR)/floppy.img -m 256

clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.bin $(BUILD_DIR)/*.img

.PHONY: all boot kernel ide floppy run clean