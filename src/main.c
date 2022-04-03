/**
 * Simple Projectile Locating and Aiming Tower - Close in Weapons System
 * @category SPLAT - CIWS
 * ESE350 Spring 2022 Final Project - Vikram Bala and Andy Liu
 * @author Developed by Vikram Bala, Andy Liu C'24 University of Pennsylvania
 * @date April, 2022
 */

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "Custom_Servo.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main() {
    CLKPR = (1 << CLKPCE);
    CLKPR = (1<<CLKPS0); //Divide clock by 1/2

    UART_setup();
    Servo servo = create_servo_pitch();

    while(1) {
        _delay_ms(500);
        servo.turn_to(0);

        _delay_ms(500);
        servo.turn_to(30);

        _delay_ms(500);
        servo.turn_to(60);

        _delay_ms(500);
        servo.turn_to(90);

        _delay_ms(500);
        servo.turn_to(120);

        _delay_ms(500);
        servo.turn_to(150);

        _delay_ms(500);
        servo.turn_to(180);
    }

    return 0;
}

#pragma clang diagnostic pop