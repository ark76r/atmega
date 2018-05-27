#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "../lib/uart/uart.h"

//#include <stdio.h>
// int uart_putchar(char c, FILE *stream)
// {
//     putByte(c);
//     return 0;
// }

// int uart_getchar(FILE *stream)
// {
//     return getByte();
// }

// FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

    // stdout = &uart_str;


int main(void)
{
    clock_prescale_set(clock_div_16);

    initUART();

    writeString("\e[0m\e[2J\e[HStart\r\n\e[?25l");


    while (1) //Pętla główna
    {
        _delay_ms(1000);
	putByte('X');
    }
    return 0;
}
