#ifndef PICOSECOND_TIMER_H
#define PICOSECOND_TIMER_H

#include <stdint.h>

// ----------------------------------------------------------------------------
// HARDWARE REGISTER MAPPING (Conceptual ASML Custom Silicon)
// ----------------------------------------------------------------------------

// Base address for the high-frequency hardware timer subsystem
#define HW_TIMER_BASE_ADDR 0x40020000

// Volatile pointers to force the compiler to read/write directly to silicon memory
#define TIMER_CONTROL_REG (*(volatile uint32_t*)(HW_TIMER_BASE_ADDR + 0x00))
#define TIMER_COUNTER_REG (*(volatile uint32_t*)(HW_TIMER_BASE_ADDR + 0x04))
#define TIMER_IRQ_ACK_REG (*(volatile uint32_t*)(HW_TIMER_BASE_ADDR + 0x08))

// Hardware-specific bitmasks
#define TIMER_ENABLE_BIT  (1 << 0)
#define TIMER_RESET_BIT   (1 << 1)
#define TIMER_INT_ENABLE  (1 << 2)

// ----------------------------------------------------------------------------
// RTOS MACROS & INTRINSICS
// ----------------------------------------------------------------------------

// A cycle-accurate wait loop to burn CPU cycles for picosecond alignment
static inline void __attribute__((always_inline)) cycle_delay(uint32_t cycles) {
    while (cycles > 0) {
        __asm__ volatile ("nop"); // No-Operation (burns exactly 1 clock cycle)
        cycles--;
    }
}

// Global struct to hold strict deadline requirements
typedef struct {
    uint64_t expected_fire_time_ps;
    uint64_t actual_fire_time_ps;
    int32_t  jitter_margin_ps;
} LaserTimingProfile;

#endif // PICOSECOND_TIMER_H
