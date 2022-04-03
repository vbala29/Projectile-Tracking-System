/**
 * ATMega328P UART Transmission (Tx) Library
 * @author Developed by Vikram Bala, C'24 University of Pennsylvania
 * @date March, 2022
 */

#ifndef LAB3_UART_H
#define LAB3_UART_H

#include <avr/io.h>
#include <string.h>

/**
 * @brief Used to initialize UART on the Arduino.
 * Params for frame: 8 data bits, 1 stop bits, 0 parity bits. (8-N-1)
 */
void UART_setup();

/**
 * @brief Used to send data byte over UART
 * @param data, the byte to be sent
 */
void UART_write(unsigned char data);


/**
 * @brief Used to send string over UART
 * @param str, the str to be sent
 */
void UART_stringWrite(char *str);

#endif //LAB3_UART_H
