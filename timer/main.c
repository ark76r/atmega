#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "../lib/uart/uart.h"

#define TRIGGER PB1
#define TRIGGER_PORT PORTB
#define TRIGGER_DDR DDRB

volatile uint16_t echo_width;

int uart_putchar(char c, FILE *stream)
{
    putByte(c);
    return 0;
}

int uart_getchar(FILE *stream)
{
    return getByte();
}

FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

void hc_trigger(void)
{
    echo_width = 0;
    TRIGGER_PORT |= (1 << TRIGGER);
    _delay_us(10);
    TRIGGER_PORT &= ~(1 << TRIGGER);
}

// Pętla główna
int main(void)
{
    clock_prescale_set(clock_div_1);
    //########### I/O ###########
    TRIGGER_DDR |= (1 << TRIGGER);

    initUART();
    stdout = &uart_str;

    TCCR1B |= (1 << ICES1);
    TCCR1B |= (1 << CS11); // clock/8
    TIMSK1 |= (1 << ICIE1);

    sleep_enable();
    sei();

    printf("\e[0m\e[2JStart\r\n\e[?25l");

    while (1) //Pętla główna
    {
        _delay_ms(100);
        hc_trigger();
        while (!echo_width) {
            sleep_cpu();
        }
        if (echo_width < 32000u)
            printf("\r\e[34;47mTime: %5uus Distance: %u cm  \e[0m\e[K", echo_width, (echo_width+29u)/58u);
        else
            printf("\r\e[34;47mTime: infinity Distance: infinity  \e[0m\e[K");
    }
    return 0;
}



ISR(TIMER1_CAPT_vect)
{
    static uint16_t last_capture;

    if (!(TCCR1B & (1 << ICES1)))
    {
        echo_width = ICR1 - last_capture;
    }
    last_capture = ICR1;
    TCCR1B ^= (1<<ICES1);
}
