//! Trajectory calculus for molten tin in a vacuum

use crate::droplet_generator::DropletState;

pub struct TargetPredictor {
    focal_point_distance_m: f64,
}

impl TargetPredictor {
    pub fn new(focal_distance: f64) -> Self {
        Self {
            focal_point_distance_m: focal_distance,
        }
    }

    /// Calculates the exact time the droplet will intersect the laser beam
    pub fn calculate_intersection_time_ps(&self, droplet: &DropletState) -> u64 {
        // Time = Distance / Velocity
        let time_seconds = self.focal_point_distance_m / droplet.velocity_m_s;
        
        // Convert seconds to picoseconds (1 second = 1 trillion picoseconds)
        let time_picoseconds = time_seconds * 1_000_000_000_000.0;
        
        time_picoseconds as u64
    }
}
