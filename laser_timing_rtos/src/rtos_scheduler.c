#include "picosecond_timer.h"
#include <stdbool.h>

// Forward declarations of external module functions
extern bool execute_plasma_pulse_sequence(LaserTimingProfile* timing_profile);
extern bool verify_hardware_safeties(void);

// 50kHz generation means one droplet every 20 microseconds (20,000,000 picoseconds)
#define DROPLET_INTERVAL_PS 20000000

// Main Entry Point for the Bare-Metal Micro-Kernel
int main(void) {
    
    // 1. Initialize hardware timers
    TIMER_CONTROL_REG = TIMER_RESET_BIT;
    TIMER_CONTROL_REG = TIMER_ENABLE_BIT;
    
    uint64_t next_droplet_time_ps = TIMER_COUNTER_REG + DROPLET_INTERVAL_PS;

    LaserTimingProfile current_target;

    // 2. The Hard Real-Time Infinite Loop
    while (true) {
        
        // A. Verify vacuum pressure and thermal limits
        if (!verify_hardware_safeties()) {
            break; // Kernel halts, safety interlock triggered
        }

        // B. Prepare the timing strictures for the next droplet
        current_target.expected_fire_time_ps = next_droplet_time_ps;

        // C. Execute the dual-pulse laser sequence
        // This function blocks and spin-locks until the exact picosecond is reached
        if (!execute_plasma_pulse_sequence(&current_target)) {
            break; // Jitter exceeded threshold, halt production
        }

        // D. Calculate the timestamp for the NEXT droplet of tin
        next_droplet_time_ps += DROPLET_INTERVAL_PS;
    }

    // System has exited the RTOS loop due to a safety violation.
    // In an embedded environment, returning from main() usually triggers a hard CPU reset.
    return -1; 
}
