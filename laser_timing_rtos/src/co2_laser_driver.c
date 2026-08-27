#include "picosecond_timer.h"
#include <stdint.h>
#include <stdbool.h>

// Memory-mapped I/O for the optical laser amplifiers
#define LASER_AMP_CTRL_REG (*(volatile uint32_t*)0x50010000)
#define LASER_TRIGGER_PRE  (1 << 0)
#define LASER_TRIGGER_MAIN (1 << 1)

// External function hooks
extern void trigger_safety_shutdown(void);
extern void log_timing_jitter(int32_t jitter_ps);

// Fires the dual-pulse sequence required to generate EUV plasma
bool execute_plasma_pulse_sequence(LaserTimingProfile* timing_profile) {
    // 1. Await exact hardware counter match for the PRE-PULSE
    while (TIMER_COUNTER_REG < timing_profile->expected_fire_time_ps) {
        // Spin-lock wait (Hard Real-Time polling)
    }
    
    // FIRE PRE-PULSE: Flattens the molten tin droplet into a disk
    LASER_AMP_CTRL_REG |= LASER_TRIGGER_PRE; 
    
    // Calculate precision jitter
    timing_profile->actual_fire_time_ps = TIMER_COUNTER_REG;
    timing_profile->jitter_margin_ps = (int32_t)(timing_profile->actual_fire_time_ps - timing_profile->expected_fire_time_ps);

    // If jitter exceeds 500 picoseconds, the tin disk is unstable. Abort main pulse.
    if (timing_profile->jitter_margin_ps > 500) {
        trigger_safety_shutdown();
        return false; 
    }

    // 2. Wait exactly 3 nanoseconds (3000 picoseconds) for the tin droplet to expand
    // Assuming a 1GHz CPU for the conceptual math: 1 cycle = 1 nanosecond
    cycle_delay(3); 

    // FIRE MAIN PULSE: Vaporizes the flattened tin into EUV-emitting plasma
    LASER_AMP_CTRL_REG |= LASER_TRIGGER_MAIN;

    // Reset amplifiers for the next iteration
    LASER_AMP_CTRL_REG &= ~(LASER_TRIGGER_PRE | LASER_TRIGGER_MAIN);

    log_timing_jitter(timing_profile->jitter_margin_ps);
    return true;
}
