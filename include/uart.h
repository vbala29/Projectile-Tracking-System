/**
 * ATMega328P UART Transmission (Tx) Library
 * @author Developed by Vikram Bala, C'24 University of Pennsylvania
 * @date April 12th, 2022
 * @brief Uses USART0 on Arduino in syncrhonous mode at a specified Baud rate of 9600 bits/second.
 */

#ifndef LAB3_UART_H
#define LAB3_UART_H

#include <avr/io.h>
#include <string.h>

#define DEBUG 0 //Boolean turns on/off debugging mode print statements

//Used for UART printing
char str[100];

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
 * @brief Used to receive UART data.
 * @param buffer to store the received data
 * @param size of the buffer
 */
void UART_read(char* buffer, uint8_t size);

/**
 * @brief Used to enable and disabled interrupts when Serial Rx data is received
 * @param enable - boolean for whether to enable or disable RXCIE0
 */
void UART_RxInterruptEnable(int enable);

/**
 * @brief Used to enable and disabled interrupts when Serial Tx data is finished sending
 * @param enable - boolean for whether to enable or disable TXCIE0
 */
void UART_TxInterruptEnable(int enable);


/**
 * @brief Used to send string over UART
 * @param str, the str to be sent
 */
void UART_stringWrite(char *str);


#endif //LAB3_UART_H
