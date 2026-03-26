#ifndef KERNEL_SYSTEM_H
#define KERNEL_SYSTEM_H

#include <stdint.h>

/* CPU information structure */
typedef struct {
    uint32_t cpu_id;
    uint32_t features;
    char vendor[13];
    char brand[49];
} cpu_info_t;

/* System info structure */
typedef struct {
    cpu_info_t cpu;
    uint64_t mem_total;
    uint64_t mem_available;
    uint32_t uptime;
} system_info_t;

extern system_info_t system_info;

/* CPUID instruction */
void cpuid(uint32_t leaf, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx);

/* CPU detection */
void cpu_detect(void);

/* System halt/reboot */
void system_halt(void);
void system_reboot(void);

#endif /* KERNEL_SYSTEM_H */
