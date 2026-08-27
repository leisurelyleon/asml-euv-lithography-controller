#pragma once
#include <iostream>
#include <cmath>

namespace asml::optics {

struct Position3D {
    double x_nm;
    double y_nm;
    double z_nm;
};

class WaferStageStepper {
private:
    Position3D current_position_;
    bool is_levitating_;

public:
    WaferStageStepper() : current_position_{0.0, 0.0, 0.0}, is_levitating_(false) {}

    void EngageMagneticLevitation() {
        std::cout << "[WAFER-STAGE] Engaging planar magnetic levitation bearings...\n";
        is_levitating_ = true;
    }

    // Moves the stage to the next exposure field with nanometer precision
    bool StepToNextField(double target_x_nm, double target_y_nm) {
        if (!is_levitating_) {
            std::cout << "[WAFER-STAGE] ERROR: Cannot move stage; levitation offline.\n";
            return false;
        }

        // Simulate the extreme acceleration and deceleration curve
        current_position_.x_nm = target_x_nm;
        current_position_.y_nm = target_y_nm;

        // In a real system, interferometers verify this position down to the picometer
        std::cout << "[WAFER-STAGE] Stepped to field position: X=" 
                  << current_position_.x_nm << "nm, Y=" 
                  << current_position_.y_nm << "nm. Ready for exposure.\n";
                  
        return true;
    }

    Position3D GetCurrentPosition() const {
        return current_position_;
    }
};

} // namespace asml::optics
