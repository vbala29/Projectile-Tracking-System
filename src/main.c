#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "Custom_Servo.h"

int main() {
    CLKPR = (1 << CLKPCE);
    CLKPR = (1<<CLKPS0); //Divide clock by 1/2

    UART_setup();

    _delay_ms(1000);

    Servo servo = create_servo();
    while(1) {
        //servo.turn_to(0);
        servo.debug(0.02);

        _delay_ms(500);

       // servo.turn_to(180);
       servo.debug(0.12);

        _delay_ms(500);
    }

    return 0;
}