# Makefile for VoyageOS

CC = gcc
AS = nasm
LD = ld

CFLAGS = -ffreestanding -m32 -O2 -Wall -Wextra -Iinclude
LDFLAGS = -T linker.ld -m elf_i386

OBJ = boot.o kernel.o vga.o keyboard.o mouse.o shell.o string.o sound.o compiler.o interpreter.o

all: VoyageOS.bin

VoyageOS.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

boot.o: boot/boot.asm
	$(AS) -f elf32 -o $@ $<

kernel.o: src/kernel/kernel.c
	$(CC) $(CFLAGS) -c -o $@ $<

vga.o: src/kernel/vga.c
	$(CC) $(CFLAGS) -c -o $@ $<

keyboard.o: src/drivers/keyboard.c
	$(CC) $(CFLAGS) -c -o $@ $<

mouse.o: src/drivers/mouse.c
	$(CC) $(CFLAGS) -c -o $@ $<

shell.o: src/shell/shell.c
	$(CC) $(CFLAGS) -c -o $@ $<

string.o: src/kernel/string.c
	$(CC) $(CFLAGS) -c -o $@ $<

sound.o: src/kernel/sound.c
	$(CC) $(CFLAGS) -c -o $@ $<

compiler.o: src/compiler/compiler.c
	$(CC) $(CFLAGS) -c -o $@ $<

interpreter.o: src/interpreter/interpreter.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o VoyageOS.bin

run: VoyageOS.bin
	qemu-system-x86_64 -kernel VoyageOS.bin

.PHONY: all clean run