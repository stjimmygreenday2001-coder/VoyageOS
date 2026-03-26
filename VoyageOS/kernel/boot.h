#ifndef KERNEL_BOOT_H
#define KERNEL_BOOT_H

#include <stdint.h>

/* Multiboot information structure (GRUB) */
typedef struct {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
} multiboot_info_t;

/* Memory map entry */
typedef struct {
    uint32_t size;
    int64_t addr;
    int64_t len;
    uint32_t type;
} __attribute__((packed)) memory_map_t;

#define MULTIBOOT_FLAG_MEM      0x001
#define MULTIBOOT_FLAG_DEVICE   0x002
#define MULTIBOOT_FLAG_CMDLINE  0x004
#define MULTIBOOT_FLAG_MODS     0x008
#define MULTIBOOT_FLAG_SYMS_ELF 0x020
#define MULTIBOOT_FLAG_MMAP     0x040

#endif /* KERNEL_BOOT_H */
