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
#include <stdlib.h>
#include <stdio.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

Servo pitch;
Servo yaw;

char str[40];

void initialize() {
    UART_setup();
    //UART_RxInterruptEnable(1);

    CLKPR = (1 << CLKPCE);
    CLKPR = (1<<CLKPS0); //Divide clock by 1/2

    pitch = create_servo_pitch();
    yaw = create_servo_yaw();
}

int main() {
    initialize();

    while(1) {
        char buf_pitch[5] = {'\0', '\0', '\0', '\0', '\0'};
        UART_read(buf_pitch, 4);
        char *end_pitch;
        unsigned int pitch_deg = strtol(buf_pitch, &end_pitch, 10);

        char buf_yaw[5] = {'\0', '\0', '\0', '\0', '\0'};
        UART_read(buf_yaw, 4);
        char *end_yaw;
        unsigned int yaw_deg = strtol(buf_yaw, &end_yaw, 10);


        sprintf(str, "Pitch Angle: %u, Yaw Angle: %u\n", pitch_deg, yaw_deg);
        UART_stringWrite(str);

        pitch.turn_to(pitch_deg);
        yaw.turn_to(yaw_deg);
    }

    return 0;
}



#pragma clang diagnostic pop