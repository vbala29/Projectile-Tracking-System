#include "Custom_Servo.h"
#include "uart.h"
#include <stdio.h>

#define DEBUG 1

char str[20];
int ocr_max = (int) (0.020 / (1.0/(CPU_FREQ/PRESCALE)));

void debug_servo_yaw(float duty_cycle) {
    OCR0B = ocr_max * duty_cycle;
}

void debug_servo_pitch(float duty_cycle) {
    OCR1B = ocr_max * duty_cycle;
}

void turn_servo_yaw(int deg) {
    if (deg > 180) {
        return;
    } else if (deg < 0) {
        return;
    }

    float duty_cycle = LOWER_PWM_BOUND + ((float) deg) * PWM_SLOPE;
    if (DEBUG) sprintf(str, "YAW mapped deg: %i to float: %i\n", deg, (int) (duty_cycle * 1000));
    if (DEBUG) UART_stringWrite(str);

    OCR0B = ocr_max * duty_cycle;
}


void turn_servo_pitch(int deg) {
    if (deg > 180) {
        return;
    } else if (deg < 0) {
        return;
    }

    float duty_cycle = LOWER_PWM_BOUND + ((float) deg) * PWM_SLOPE;
    if (DEBUG) sprintf(str, "PITCH mapped deg: %i to float: %i\n", deg, (int) (duty_cycle * 1000));
    if (DEBUG) UART_stringWrite(str);

    OCR1B = ocr_max * duty_cycle;
}

Servo create_servo_yaw() {
    Servo servo;
    servo.turn_to = &turn_servo_yaw;
    servo.debug = &debug_servo_yaw;

    DDRD |= (1 << DEFAULT_YAW_PIN); //Initialize pin D5/OC0B to be output

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

    if (DEBUG) sprintf(str,"OCR Timer 0 MAX: %u\n", ocr_max);
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
    servo.turn_to = &turn_servo_pitch;
    servo.debug = &debug_servo_pitch;

    DDRB |= (1 << DEFAULT_PITCH_PIN); //Initialize pin 10/OC1B to be output

    //Set timer one to have internal div by 1024;
    TCCR1B |= (1<<CS10);
    TCCR1B &= ~(1<<CS11);
    TCCR1B |= (1<<CS12);

    //Set timer to fast PWM mode. Table 14-9 In AT328 Datasheet. OCR1A is top.
    TCCR1A |= (1<<WGM10);
    TCCR1A |= (1<<WGM11);
    TCCR1B |= (1<<WGM12);
    TCCR1B |= (1<<WGM13);

    //TOP value, period set to 20ms as per datasheet
    OCR1A = ocr_max;

    if (DEBUG) sprintf(str,"OCR Timer 1 MAX: %u\n", ocr_max);
    if (DEBUG) UART_stringWrite(str);
    if (DEBUG) sprintf(str, "CLKPR: 0x%X, OCR1A; 0x%X\n", CLKPR, OCR1A);
    if (DEBUG) UART_stringWrite(str);

    //Compare value, sets the duty cycle
    OCR1B = OCR1A * 0.5;

    //Set OC1B to be in non inverting mode.
    TCCR1A |= (1<<COM1B1);

    return servo;
}


