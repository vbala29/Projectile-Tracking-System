

/**
 * Simple Projectile Locating and Aiming Tower - Close in Weapons System
 * @category SPLAT - CIWS
 * ESE350 Spring 2022 Final Project - Vikram Bala and Andy Liu
 * @author Developed by Vikram Bala, Andy Liu C'24 University of Pennsylvania
 * @date April, 2022
 */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

#include "LCD_GFX.h"
#include "ST7735.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>

//Macro max function
#define max(a, b) ((a) < (b) ? b : a)


int serial_state = 0; //0 = no new data, 1 = pitch received, 2 = yaw/pitch received.
char buf[5] = {'\0', '\0', '\0', '\0', '\0'};



unsigned int x_val = 0; //Current radar primary track position horizontal location
unsigned int y_val = 0; //Current radar primary track position distance
unsigned int x_old_val = 0; //Used for clearing old primary radar tracks on the LCD.
unsigned int y_old_val = 0; //Used for clearing old primary radar tracks on the LCD.

float x_scaler = LCD_WIDTH/50;
float y_scaler = 1;


void Initialize()
{
    cli(); //disable global interrupts

    CLKPR = (1 << CLKPCE);
    CLKPR = (1<<CLKPS0); //Divide clock by 1/2

    lcd_init();
    UART_setup();
    UART_RxInterruptEnable(1);

    sei();
}

/**
 * @brief Triggered when USART0 RX is complete
 * USART RXC0 bit is high = RX is complete.
 */
ISR(USART_RX_vect) {
    char *end;
    UART_read(buf, 4);
    if (buf[0] == 'X') {
        x_old_val = x_val;
        x_val = strtol(buf + 1, &end, 10);
        serial_state = 1;
    } else if (buf[0] == 'D') {
        y_old_val = y_val;
        y_val = strtol(buf + 1, &end, 10);
        serial_state = 2;
    }


    if (serial_state == 2) {
        sprintf(str, "X val: %u, Y Val: %u\n", x_val, y_val);
        UART_stringWrite(str);

        //Clear old radar track
        LCD_drawCircle(x_old_val * x_scaler, LCD_HEIGHT - y_scaler * y_old_val, 8, rgb565(255, 255, 255));

        //Draw new radar track
        LCD_drawCircle(x_val * x_scaler, LCD_HEIGHT - y_scaler * y_val, 5, rgb565(0, 0, 255));

        //Clear buffers
        for (int i = 0; i < 5; i++) {
            buf[i] = '\0';
        }

        serial_state = 0;
    }
}

int main() {
    Initialize();
    LCD_setScreen(rgb565(255, 255, 255));
    sprintf(str, "Radar Display\n");
    UART_stringWrite(str);
    sprintf(str, "Radar Display");
    LCD_drawString(10, 10, str, rgb565(0, 0, 0), rgb565(255, 255, 255));
    LCD_drawCircle(LCD_WIDTH/2 - 6, LCD_HEIGHT - 6, 3, rgb565(255, 0, 0));

    while(1); //Let the ISR trigger and update LCD

}

#pragma clang diagnostic pop