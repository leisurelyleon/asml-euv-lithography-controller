#include "WaferStageStepper.cpp"
#include "ReticleMaskAlignment.cpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace asml::optics;

// Simulates the burst of 13.5nm EUV photons hitting the wafer
void ExposeField(int field_id) {
    std::cout << "  -> [EXPOSURE] Firing 13.5nm EUV plasma pulse for Field " << field_id << "...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Simulation delay
    std::cout << "  -> [EXPOSURE] Photoresist chemically altered. Transistors patterned.\n";
}

int main() {
    std::cout << "=========================================================\n";
    std::cout << "      ASML EUV LITHOGRAPHY: OPTICS & STAGE SIMULATOR     \n";
    std::cout << "=========================================================\n";

    ReticleMaskAlignment reticle;
    WaferStageStepper stage;

    // 1. Setup the optical blueprint
    reticle.LoadReticleMask();
    if (!reticle.PerformGlobalAlignment()) {
        std::cout << "[SYSTEM] CRITICAL: Blueprint alignment failed.\n";
        return -1;
    }

    // 2. Setup the silicon wafer
    stage.EngageMagneticLevitation();

    std::cout << "\n[SYSTEM] Commencing Step-and-Scan Wafer Exposure...\n";

    // 3. Expose a 2x2 grid of microchip fields on the wafer
    int field_count = 1;
    for (double x = 0.0; x <= 26000000.0; x += 26000000.0) { // 26mm step
        for (double y = 0.0; y <= 33000000.0; y += 33000000.0) { // 33mm step
            
            // Move the stage with nanometer precision
            stage.StepToNextField(x, y);
            
            // Expose the silicon to the EUV light
            ExposeField(field_count);
            field_count++;
            
            std::cout << "\n";
        }
    }

    std::cout << "=========================================================\n";
    std::cout << " WAFER PROCESSING COMPLETE. READY FOR ETCHING.\n";
    std::cout << "=========================================================\n";

    return 0;
}
