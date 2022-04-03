#include "Custom_Servo.h"
#include "uart.h"
#include <stdio.h>

#define DEBUG 1

char str[20];
int ocr_max = (int) (0.020 / (1.0/(CPU_FREQ/PRESCALE)));

void debug_servo(float duty_cycle) {
    OCR0B = ocr_max * duty_cycle;
}

void turn_servo(int deg) {
    if (deg > 180) {
        return;
    } else if (deg < 0) {
        return;
    }

    float duty_cycle = LOWER_PWM_BOUND + ((float) deg) * PWM_SLOPE;
    if (DEBUG) sprintf(str, "mapped deg: %i to float: %i\n", deg, (int) (duty_cycle * 1000));
    if (DEBUG) UART_stringWrite(str);

    OCR0B = ocr_max * duty_cycle;
}

Servo create_servo_yaw() {
    Servo servo;
    servo.pin = DEFAULT_YAW_PIN;
    servo.turn_to = &turn_servo;
    servo.debug = &debug_servo;

    DDRD |= (1 << DDD5); //Initialize pin D6/OCB0 to be output

    //Set timer zero to have internal div by 1024;
    TCCR0B |= (1<<CS00);
    TCCR0B &= ~(1<<CS01);
    TCCR0B |= (1<<CS02);

    //Set timer to fast PWM mode. Table 14-9 In AT328 Datasheet. OCR0A is top.
    TCCR0A |= (1<<WGM00);
    TCCR0A |= (1<<WGM01);
    TCCR0B |= (1<<WGM02);

    //TOP value, period set to 20ms as per datasheet
    OCR0A = ocr_max;

    if (DEBUG) sprintf(str,"OCR MAX: %u\n", ocr_max);
    if (DEBUG) UART_stringWrite(str);
    if (DEBUG) sprintf(str, "CLKPR: 0x%X, OCR0A; 0x%X\n", CLKPR, OCR0A);
    if (DEBUG) UART_stringWrite(str);

    //Compare value, sets the duty cycle
    OCR0B = OCR0A * 0.5;

    //Set OCB0 to be in non inverting mode.
    TCCR0A |= (1<<COM0B1);

    return servo;
}


Servo create_servo_pitch() {
    Servo servo;
    servo.pin = DEFAULT_PITCH_PIN;
    servo.turn_to = &turn_servo;
    servo.debug = &debug_servo;

    DDRD |= (1 << DDD5); //Initialize pin D6/OCB0 to be output

    //Set timer zero to have internal div by 1024;
    TCCR0B |= (1<<CS00);
    TCCR0B &= ~(1<<CS01);
    TCCR0B |= (1<<CS02);

    //Set timer to fast PWM mode. Table 14-9 In AT328 Datasheet. OCR0A is top.
    TCCR0A |= (1<<WGM00);
    TCCR0A |= (1<<WGM01);
    TCCR0B |= (1<<WGM02);

    //TOP value, period set to 20ms as per datasheet
    OCR0A = ocr_max;

    if (DEBUG) sprintf(str,"OCR MAX: %u\n", ocr_max);
    if (DEBUG) UART_stringWrite(str);
    if (DEBUG) sprintf(str, "CLKPR: 0x%X, OCR0A; 0x%X\n", CLKPR, OCR0A);
    if (DEBUG) UART_stringWrite(str);

    //Compare value, sets the duty cycle
    OCR0B = OCR0A * 0.5;

    //Set OCB0 to be in non inverting mode.
    TCCR0A |= (1<<COM0B1);

    return servo;
}


