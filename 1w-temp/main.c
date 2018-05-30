#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
//#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "../lib/lcd/lcd.h"
#include "../lib/dallas1w/dallas1w.h"
#include "../lib/uart/uart.h"

//#include "../lib/console/console.h"

volatile uint16_t timer0 = 0;

void init_timer0()
{
    OCR0A |= 99; // match each 100 cycles
    TCCR0A |= (1<< WGM01);
    TIMSK0 |= (1<<OCIE0A);

    TCCR0B |= (1 << CS00); // turn on timer - no prescaler, 1MHz clock.
}

void timer0_sleep_ms(uint16_t m)
{
    // timer0 tics: each 100 us
    for(; m>0; m--)
    {
        timer0 = 0;
        while (timer0 < 10) // 100us * 10 == 1ms
        {
            sleep_cpu();
        }
    }
}

int main(void)
{
    char str[10];
    float temperature;
    clock_prescale_set(clock_div_16);
    //########### I/O ###########

    initUART();
    init_timer0();
    sei();
    sleep_enable();
    //init_console();

    writeString("\e[0m\e[2J\e[HStart\r\n\e[?25l");
    timer0_sleep_ms(1000);

    lcd_init();
    lcd_control(1, 0, 0);
    lcd_clear();
    lcd_home();

    uint8_t *rom = p1w_search_rom();

    if (!rom)
    {
        writeString("No ROM\r\n");
    }
    else
    {
        writeString("ROM: ");

        for (uint8_t j = 8; j > 0; j--)
        {
            writeHex(rom[j - 1]);
            putByte(' ');
        }
        writeString("\r\n");
    }


    while (1) //Pętla główna
    {
        uint8_t present = p1w_reset();
        if (!present)
        {
            writeString("\rno sensor on 1-wire                ");
            lcd_clear();
            lcd_home();
            lcd_text("no sensor");
            continue;
        }
        lcd_goto(0, 0);
        lcd_text("Temp:     ");

        p1w_write_byte(0xCC); // SKIP ROM
        p1w_write_byte(0x44); // CONVERT T
        p1w_power(1);         // high power
        timer0_sleep_ms(750);
        p1w_reset();
        p1w_write_byte(0xCC); // SKIP ROM
        //p1w_write_byte(0x55); // MATCH ROM
        //for (uint8_t j = 0; j < 8; j++)
        //    p1w_write_byte(rom[j]);

        p1w_write_byte(0xBE); // READ SCRATCHPAD
        uint8_t t1 = p1w_read_byte();
        uint8_t t2 = p1w_read_byte();
        p1w_reset();
        p1w_power(0);
        temperature = (float)(t1 + (t2 << 8)) / 16.0;
        dtostrf(temperature, 1, 1, str);
        lcd_goto(1, 0);
        lcd_text(str);
        writeString("\rTemperature: ");
        writeString(str);
        writeString("   ");
        writeString("              ");

        // if (uart_data_ready())
        //     putByte(getByte());
    }
    return 0;
}

ISR(TIMER0_COMPA_vect)
{
    timer0++;
}
