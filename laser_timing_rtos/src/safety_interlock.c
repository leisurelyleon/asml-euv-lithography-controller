#include <stdint.h>
#include <stdbool.h>

// Hardware sensors
#define THERMAL_SENSOR_BASE  (*(volatile uint32_t*)0x60000000)
#define VACUUM_PRESSURE_REG  (*(volatile uint32_t*)0x60000004)
#define SYSTEM_SHUTDOWN_REG  (*(volatile uint32_t*)0xFFFF0000)

// Safety thresholds
#define MAX_LENS_TEMP_CELSIUS 45
#define MIN_VACUUM_PASCALS    1 // Must be a near-perfect vacuum for EUV light to travel

void trigger_safety_shutdown(void) {
    // Writing 0xDEADBEEF triggers an unmaskable hardware interrupt 
    // that immediately cuts power to the laser amplifiers.
    SYSTEM_SHUTDOWN_REG = 0xDEADBEEF;
}

// Background task checked by the RTOS scheduler
bool verify_hardware_safeties(void) {
    uint32_t current_temp = THERMAL_SENSOR_BASE;
    uint32_t current_pressure = VACUUM_PRESSURE_REG;

    if (current_temp > MAX_LENS_TEMP_CELSIUS) {
        trigger_safety_shutdown();
        return false;
    }

    if (current_pressure > MIN_VACUUM_PASCALS) {
        // Air is leaking into the chamber; EUV light will be absorbed before hitting the wafer.
        trigger_safety_shutdown();
        return false;
    }

    return true;
}

// Mock logger for the RTOS
void log_timing_jitter(int32_t jitter_ps) {
    // In a real system, this writes to a lock-free ring buffer for external diagnostic reading
    (void)jitter_ps; 
}
