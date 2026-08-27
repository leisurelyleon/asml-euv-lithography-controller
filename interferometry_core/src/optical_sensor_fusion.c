#include <stdint.h>
#include <stdbool.h>

// Memory-Mapped Sensor Registers
#define INTERFEROMETER_RAW_X (*(volatile uint64_t*)0x70000000)
#define INTERFEROMETER_RAW_Y (*(volatile uint64_t*)0x70000008)
#define INTERFEROMETER_RAW_Z (*(volatile uint64_t*)0x70000010)

typedef struct {
    double x_picometers;
    double y_picometers;
    double z_picometers;
    bool   signal_locked;
} MirrorPosition;

// Convert raw optical phase shifts into physical distances (picometers)
double calculate_phase_to_distance(uint64_t raw_phase_data) {
    // A highly simplified mock of HeNe (Helium-Neon) laser phase unwrapping
    // Wavelength / 4096 interpolation scale
    const double wavelength_pm = 632816.0; // 632.8 nm in picometers
    return (double)(raw_phase_data % 4096) * (wavelength_pm / 4096.0);
}

MirrorPosition read_fused_mirror_position(void) {
    MirrorPosition pos;
    
    // Read directly from the hardware FPGA registers
    pos.x_picometers = calculate_phase_to_distance(INTERFEROMETER_RAW_X);
    pos.y_picometers = calculate_phase_to_distance(INTERFEROMETER_RAW_Y);
    pos.z_picometers = calculate_phase_to_distance(INTERFEROMETER_RAW_Z);
    
    // In reality, if the laser beam breaks, signal_locked becomes false
    pos.signal_locked = true; 
    
    return pos;
}
