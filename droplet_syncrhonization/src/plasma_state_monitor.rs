//! EUV Conversion Efficiency (CE) tracking

pub struct PlasmaStateMonitor {
    target_euv_energy_mj: f64,
}

impl PlasmaStateMonitor {
    pub fn new() -> Self {
        Self {
            target_euv_energy_mj: 2.5, // 2.5 milliJoules per pulse
        }
    }

    /// Analyzes the EUV flash to determine if the droplet was hit perfectly
    pub fn analyze_conversion_efficiency(&self, measured_energy_mj: f64) -> FeedbackAdjustment {
        let efficiency = measured_energy_mj / self.target_euv_energy_mj;

        if efficiency >= 0.95 {
            FeedbackAdjustment::Optimal
        } else if efficiency > 0.80 {
            FeedbackAdjustment::MinorTimingCorrection(15) // Shift next pulse by 15 picoseconds
        } else {
            FeedbackAdjustment::MajorAlignmentWarning
        }
    }
}

pub enum FeedbackAdjustment {
    Optimal,
    MinorTimingCorrection(i32),
    MajorAlignmentWarning,
}
