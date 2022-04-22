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
char buf [5] = {'\0', '\0', '\0', '\0', '\0'};

unsigned int pitch_deg = 0;
unsigned int yaw_deg = 0;

uint32_t rate_limiter = 0;

void initialize() {
    cli();
    UART_setup();
    UART_RxInterruptEnable(1);

    CLKPR = (1 << CLKPCE);
    CLKPR = (1<<CLKPS0); //Divide clock by 1/2

    DDRD &= ~(1 << DDD7); //Initialize pin D7 to be an input
    DDRD |= (1 << DDD2); //Initialize pin D2 to be an output

    pitch = create_servo_pitch();
    yaw = create_servo_yaw();

    sei();
}

void destruct() {
    free(pitch);
    free(yaw);
}

int main() {
    initialize();

    while(1) {
        if (PIND & (1 << PIND7)) {
            PORTD |= (1 << PORTD2);
        } else {
            PORTD &= ~(1 << PORTD2);
        }

        if (rate_limiter == 500000) {
            //Send location data to ESP8266 to be used by the web server.
            sprintf(str, "%u,%u.\n", pitch_deg, yaw_deg);
            UART_stringWrite(str);
            rate_limiter = 0;
        } else {
            rate_limiter++;
        }
    }

    destruct();

    return 0;
}

/**
 * @brief Triggered when USART0 RX is complete
 * USART RXC0 bit is high = RX is complete.
 */
ISR(USART_RX_vect) {
    char *end;
    UART_read(buf, 4);
    if (buf[0] == 'P') {
        pitch_deg = strtol(buf + 1, &end, 10);
        serial_state = 1;
    } else if (buf[0] == 'Y') {
        yaw_deg = strtol(buf + 1, &end, 10);
        serial_state = 2;
    }

    if (serial_state == 2) {
        pitch->turn_to(pitch_deg);
        yaw->turn_to(yaw_deg);

        sprintf(str, "Pitch Angle: %u, Yaw Angle: %u\n", pitch_deg, yaw_deg);
        UART_stringWrite(str);

        //Clear buffers
        for (int i = 0; i < 5; i++) {
            buf[i] = '\0';
        }

        serial_state = 0;
    }


}



#pragma clang diagnostic pop

