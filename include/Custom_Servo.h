#ifndef SPLAT_CIWS_CUSTOM_SERVO_H
#define SPLAT_CIWS_CUSTOM_SERVO_H

#include <avr/io.h>

#define DEFAULT_YAW_PIN 5
#define DEFAULT_PITCH_PIN 5
#define PRESCALE 1024.0
#define CPU_FREQ 8000000.0

#define LOWER_PWM_BOUND 0.05
#define UPPER_PWM_BOUND 0.1
#define PWM_SLOPE ((UPPER_PWM_BOUND - LOWER_PWM_BOUND)/180.0)

typedef struct Servo {
    int pin;
    void (*turn_to) (int);
    void (*debug) (float);
} Servo;

void turn_servo();
Servo create_servo();



#endif //SPLAT_CIWS_CUSTOM_SERVO_H