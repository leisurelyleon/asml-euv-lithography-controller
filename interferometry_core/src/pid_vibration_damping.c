#include <stdint.h>

typedef struct {
    double kp; // Proportional gain
    double ki; // Integral gain
    double kd; // Derivative gain
    
    double previous_error;
    double integral_accumulator;
    double max_output;
} PID_Controller;

void pid_init(PID_Controller* pid, double kp, double ki, double kd, double max_out) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->previous_error = 0.0;
    pid->integral_accumulator = 0.0;
    pid->max_output = max_out;
}

// Calculates the required Lorentz coil current to correct the mirror's position
double pid_compute(PID_Controller* pid, double target_position_pm, double current_position_pm, double dt_seconds) {
    double error = target_position_pm - current_position_pm;
    
    // Proportional
    double p_term = pid->kp * error;
    
    // Integral
    pid->integral_accumulator += error * dt_seconds;
    double i_term = pid->ki * pid->integral_accumulator;
    
    // Derivative
    double derivative = (error - pid->previous_error) / dt_seconds;
    double d_term = pid->kd * derivative;
    
    pid->previous_error = error;
    
    double output = p_term + i_term + d_term;
    
    // Clamp the output to prevent hardware overcurrent
    if (output > pid->max_output) return pid->max_output;
    if (output < -pid->max_output) return -pid->max_output;
    
    return output;
}
