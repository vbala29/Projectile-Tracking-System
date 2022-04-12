/**
 * @brief Servo Motor library for the SG90 servo motor.
 * @author Developed by Vikram Bala, C'24 University of Pennsylvania
 * @date April 3rd, 2022
 */

#ifndef SPLAT_CIWS_CUSTOM_SERVO_H
#define SPLAT_CIWS_CUSTOM_SERVO_H

//Pin to connect SG90 motor to for yaw-axis
#define DEFAULT_YAW_PIN DDD5

//Pin to connect SG90 motor to for pitch-axis
#define DEFAULT_PITCH_PIN DDB2

//Timer1/0 prescale for Servo motor pwm control
#define PRESCALE 1024.0

//CPU_FREQ/System clock (prescale of 1/2, so 16Mhz/2 = 8Mhz)
#define CPU_FREQ 8000000.0

//The value at which the motor is at 0º
#define LOWER_PWM_BOUND 0.025

//The value at which the motor is at 180º
#define UPPER_PWM_BOUND 0.121

//Change in pwm value per each degree in SG90 servo range of motion.
#define PWM_SLOPE ((UPPER_PWM_BOUND - LOWER_PWM_BOUND)/180.0)

typedef struct {
    void (*turn_to) (int);
    void (*debug) (float);
} Servo;

void debug_servo_yaw(float duty_cycle);
void debug_servo_pitch(float duty_cycle);

void turn_servo();
Servo* create_servo_yaw();
Servo* create_servo_pitch();



#endif //SPLAT_CIWS_CUSTOM_SERVO_H