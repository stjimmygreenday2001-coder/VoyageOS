#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#include <stdint.h>

/* PIT (Programmable Interval Timer) */
#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_CMD      0x43

/* Initialize system timer */
void timer_init(uint32_t frequency);

/* Get system uptime */
uint64_t timer_ticks(void);
uint64_t timer_milliseconds(void);

/* Sleep functions */
void timer_sleep_ms(uint32_t milliseconds);
void timer_sleep_ticks(uint64_t ticks);

#endif /* KERNEL_TIMER_H */
