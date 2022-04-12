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
#include <avr/interrupt.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

Servo* pitch;
Servo* yaw;

int serial_state = 0; //0 = no new data, 1 = pitch received, 2 = yaw/pitch received.
char buf_pitch[5] = {'\0', '\0', '\0', '\0', '\0'};
char buf_yaw[5] = {'\0', '\0', '\0', '\0', '\0'};

unsigned int pitch_deg = 0;
unsigned int yaw_deg = 0;

void initialize() {
    cli();
    UART_setup();
    UART_RxInterruptEnable(1);

    CLKPR = (1 << CLKPCE);
    CLKPR = (1<<CLKPS0); //Divide clock by 1/2

    pitch = create_servo_pitch();
    yaw = create_servo_yaw();

    sei();
}

int main() {
    initialize();

    while(1) {

    }

    return 0;
}

/**
 * @brief Triggered when USART0 RX is complete
 * USART RXC0 bit is high = RX is complete.
 */
ISR(USART_RX_vect) {
    char *end;
    if (serial_state == 0) {
        UART_read(buf_pitch, 4);
        pitch_deg = strtol(buf_pitch, &end, 10);
        serial_state = 1;
    } else {
        UART_read(buf_yaw, 4);
        yaw_deg = strtol(buf_yaw, &end, 10);
        serial_state = 2;
    }

    if (serial_state == 2) {
        pitch->turn_to(pitch_deg);
        yaw->turn_to(yaw_deg);

        sprintf(str, "Pitch Angle: %u, Yaw Angle: %u\n", pitch_deg, yaw_deg);
        UART_stringWrite(str);

        //Clear buffers
        for (int i = 0; i < 5; i++) {
            buf_pitch[i] = '\0';
            buf_yaw[i] = '\0';
        }

        serial_state = 0;
    }


}



#pragma clang diagnostic pop