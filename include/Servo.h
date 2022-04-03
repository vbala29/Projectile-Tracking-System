#ifndef SPLAT_CIWS_SERVO_H
#define SPLAT_CIWS_SERVO_H

#define DEFAULT_PIN 3
#define PRESCALE 256.0
#define CPU_FREQ 16000000.0
#define BASE_VALUE_PWM 0.020 / (1.0/(CPU_FREQ/PRESCALE))

#define LOWER_PWM_BOUND 0.075
#define UPPER_PWM_BOUND 0.1

#include <avr/io.h>

typedef struct Servo Servo;



#endif //SPLAT_CIWS_SERVO_H