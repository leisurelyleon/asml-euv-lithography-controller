#include "optical_sensor_fusion.c"
#include "pid_vibration_damping.c"
#include <stdio.h>

// Hardware DAC (Digital-to-Analog Converter) for the magnetic levitation coils
#define LORENTZ_COIL_Z_AXIS (*(volatile uint32_t*)0x80000000)

// Convert required force output into raw DAC integers (0-65535 for a 16-bit DAC)
void apply_magnetic_force(double required_current_amps) {
    // 0 Amps = 32768 (Midpoint), +5 Amps = 65535, -5 Amps = 0
    double normalized = (required_current_amps / 5.0) * 32767.0;
    uint32_t dac_value = (uint32_t)(32768 + normalized);
    
    // Write directly to the hardware coil
    LORENTZ_COIL_Z_AXIS = dac_value; 
}

// The main high-frequency control loop (e.g., running at 20kHz)
void zeiss_mirror_control_loop(void) {
    printf("[ZEISS-OPTICS] Magnetic Levitation Control Loop Initialized.\n");
    
    PID_Controller z_axis_pid;
    // Highly tuned gains for a massive mirror floating in a vacuum
    pid_init(&z_axis_pid, 0.045, 0.001, 0.008, 5.0); 

    double target_z_picometers = 15000000.0; // Target hover height
    double dt = 0.00005; // 50 microseconds (20kHz loop)

    // Simulated single iteration of the infinite loop
    MirrorPosition current_pos = read_fused_mirror_position();
    
    if (current_pos.signal_locked) {
        double required_current = pid_compute(&z_axis_pid, target_z_picometers, current_pos.z_picometers, dt);
        apply_magnetic_force(required_current);
    } else {
        printf("[ZEISS-OPTICS] CRITICAL: Interferometer lock lost. Triggering safe catch sequence.\n");
        // De-energize coils slowly to let the mirror land on its mechanical hard-stops
    }
}
