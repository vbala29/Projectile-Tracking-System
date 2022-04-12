/**
 * ATMega328P USART Transmission/Receiving (Tx/RX) Library
 * @brief for USART0 on ATMega328P
 * @author Developed by Vikram Bala, C'24 University of Pennsylvania
 * @date March, 2022
 */

#include "uart.h"
#include <stdio.h>

#define CPU_FREQ 8000000UL
#define BAUD 9600
#define BAUD_REGISTER (((CPU_FREQ / (BAUD * 16UL))) - 1)

void UART_setup() {
    //Setting baud rate to 9600
    UBRR0H = (unsigned char) (BAUD_REGISTER >> 8);
    UBRR0L = (unsigned char) BAUD_REGISTER;

    //Enable rx and tx lines, disable RxD0 and TxD0 normal operation.
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    //1 stop bits and 8 data bits (8-N-1 mode)
    UCSR0C = (UCSZ01 << 1) | (UCSZ00 << 1);
    UCSR0C &= ~(1<<USBS0);

}

void UART_write(unsigned char data) {
    //Either Transmit not complete or USART Data register not ready for more data
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;

}

/**
 * Keep the print statements commented unless necessary for testing, as they cause
 * consecutively sent bytes to be dropped due to latency. If you want to use them to debug,
 * only send one byte at a time.
 */
void UART_read(char* buffer, uint8_t size) {
    unsigned int index = 0;

    while (index < size) {
        while (!(UCSR0A & (1 << RXC0))); //RXC0 indicates when data is available in the receive buffer.

        buffer[index] = UDR0; //Set value at index in buffer to currently received byte. Reading UDR0 cleared RXC0.
        if (DEBUG) sprintf(str, "Read: %c \n", buffer[index]);
        if (DEBUG) UART_stringWrite(str);

        if (buffer[index] == '\n' || buffer[index] == '\r') {

            buffer[index] = '\0';
            if (DEBUG) sprintf(str, "Read: %u \n", buffer[index]);
            if (DEBUG) UART_stringWrite(str);

            return;
        } else {
            index++;
        }
    }
}

void UART_RxInterruptEnable(int enable) {
    if (enable) {
        UCSR0B |= (1 << RXCIE0);
    } else {
        UCSR0B &= ~(1 << RXCIE0);
    }
}


void UART_TxInterruptEnable(int enable) {
    if (enable) {
        UCSR0B |= (1 << TXCIE0);
    } else {
        UCSR0B &= ~(1 << TXCIE0);
    }
}

void UART_stringWrite(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        UART_write(str[i]);
    }
}



