#ifndef SPLAT_CIWS_CUSTOM_SERVO_H
#define SPLAT_CIWS_CUSTOM_SERVO_H

#include <avr/io.h>

#define DEFAULT_YAW_PIN DDD5
#define DEFAULT_PITCH_PIN DDB2
#define PRESCALE 1024.0
#define CPU_FREQ 8000000.0

#define LOWER_PWM_BOUND 0.05
#define UPPER_PWM_BOUND 0.1
#define PWM_SLOPE ((UPPER_PWM_BOUND - LOWER_PWM_BOUND)/180.0)

typedef struct Servo {
    void (*turn_to) (int);
    void (*debug) (float);
} Servo;

void turn_servo();
Servo create_servo_yaw();
Servo create_servo_pitch();



#endif //SPLAT_CIWS_CUSTOM_SERVO_H