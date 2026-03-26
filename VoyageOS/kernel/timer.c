#include "timer.h"
#include "io.h"

static volatile uint64_t tick_count = 0;

/* PIT interrupt handler - increments tick counter */
void timer_interrupt(void) {
    tick_count++;
}

void timer_init(uint32_t frequency) {
    uint16_t divisor = 1193180 / frequency;  /* PIT base frequency */
    
    /* Send command byte */
    outb(PIT_CMD, 0x36);  /* Channel 0, both bytes, rate generator */
    
    /* Set divisor (frequency) */
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));
}

uint64_t timer_ticks(void) {
    return tick_count;
}

uint64_t timer_milliseconds(void) {
    /* Assuming 1000 Hz timer = 1 tick per ms */
    return tick_count;
}

void timer_sleep_ms(uint32_t milliseconds) {
    uint64_t target = tick_count + milliseconds;
    while (tick_count < target);
}

void timer_sleep_ticks(uint64_t ticks) {
    uint64_t target = tick_count + ticks;
    while (tick_count < target);
}
