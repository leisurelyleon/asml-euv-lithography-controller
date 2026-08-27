//! C-ABI Foreign Function Interface (FFI) to the Bare-Metal RTOS

use crate::droplet_generator::{DropletGenerator, DropletState};
use crate::laser_target_prediction::TargetPredictor;

// Static instances for the FFI bridge
static mut GENERATOR: Option<DropletGenerator> = None;
static mut PREDICTOR: Option<TargetPredictor> = None;

#[no_mangle]
pub extern "C" fn init_rust_droplet_engine() {
    unsafe {
        GENERATOR = Some(DropletGenerator::new());
        // Focal point is exactly 0.05 meters (5 cm) from the ejection nozzle
        PREDICTOR = Some(TargetPredictor::new(0.05));
    }
}

/// Called by the C RTOS to get the exact picosecond timestamp to fire the laser
#[no_mangle]
pub extern "C" fn compute_next_laser_firing_time(current_time_ps: u64) -> u64 {
    unsafe {
        if let (Some(gen), Some(pred)) = (GENERATOR.as_ref(), PREDICTOR.as_ref()) {
            let droplet: DropletState = gen.eject_droplet();
            let travel_time_ps = pred.calculate_intersection_time_ps(&droplet);
            
            // The absolute time the laser must fire
            return current_time_ps + travel_time_ps;
        }
        0 // Fallback/Error state
    }
}
