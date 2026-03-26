#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

#include <stdint.h>
#include <stddef.h>

/* Memory management structures */
typedef struct {
    uint64_t base;
    uint64_t size;
    int type;
} memory_region_t;

/* Memory info - populated by bootloader */
typedef struct {
    uint32_t mem_lower;     /* Memory below 1MB in KB */
    uint32_t mem_upper;     /* Memory above 1MB in KB */
    uint32_t num_regions;
    memory_region_t regions[16];
} memory_info_t;

extern memory_info_t memory_info;

/* Virtual memory management */
void memory_init(void);
void* vmalloc(size_t size);
void vfree(void* ptr);

/* Physical page management */
typedef struct {
    uint64_t physical_addr;
    uint32_t flags;
    uint16_t ref_count;
} page_frame_t;

void page_allocate(uint64_t addr);
void page_free(uint64_t addr);
uint64_t get_free_page(void);

#endif /* KERNEL_MEMORY_H */
