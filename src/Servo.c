#include "Servo.h"

struct Servo {
    int pin;
    void (*turn_to) (int);
};

void turn_servo(int deg) {
    if (deg >= 180) {
        return;
    } else if (deg < 0) {
        return;
    }

    float duty_cycle = LOWER_PWM_BOUND + deg/180 * UPPER_PWM_BOUND;
    OCR0B = OCR0A * duty_cycle;
}

Servo createServo() {
    Servo servo;
    servo.pin = DEFAULT_PIN;
    servo.turn_to = &turn_servo;

    DDRD |= (1 << PIND3); //Initialize pin D3

    //Set timer zero to have internal div by 256;
    TCCR0B &= ~(1<<CS00);
    TCCR0B &= ~(1<<CS01);
    TCCR0B |= (1<<CS02);

    //Set timer to fast PWM mode. Table 14-9 In AT328 Datasheet. OCRA0 is top.
    TCCR0A |= (1<<WGM00);
    TCCR0A |= (1<<WGM01);
    TCCR0B |= (1<<WGM02);

    //TOP value, period set to 20ms as per datasheet.
    OCR0A = (uint8_t) BASE_VALUE_PWM;

    //Compare value, sets the duty cycle
    OCR0B = OCR0A * 0.5;

    //Set OC0A to be in non inverting mode.
    TCCR0A |= (1<<COM0B1);
}

