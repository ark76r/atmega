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
#include "../lib/uart/uart.h"
#include "../lib/dallas1w/dallas1w.h"
//#include "../lib/console/console.h"

int main(void)
{
    char str[10];
    float temperature;
    clock_prescale_set(clock_div_16);
    //########### I/O ###########

    initUART();
    //init_console();

    writeString("\e[0m\e[2J\e[HStart\r\n\e[?25l");
    _delay_ms(1000);

    lcd_init();
    lcd_control(1, 1, 0);

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

    lcd_clear();
    lcd_home();
    lcd_text("Temperature:");

    while (1) //Pętla główna
    {
        _delay_ms(1000);
        uint8_t present = p1w_reset();
        if (!present)
        {
            writeString("\rno sensor on 1-wire                ");
            lcd_goto(1, 0);
            lcd_text("no sensor");
            continue;
        }

        p1w_write_byte(0xCC); // SKIP ROM
        p1w_write_byte(0x44); // CONVERT T
        p1w_power(1);         // high power
        _delay_ms(750);
        p1w_reset();
        //p1w_write_byte(0xCC); // SKIP ROM
        p1w_write_byte(0x55); // MATCH ROM
        for (uint8_t j = 0; j < 8; j++)
            p1w_write_byte(rom[j]);

        p1w_write_byte(0xBE); // READ SCRATCHPAD
        uint8_t t1 = p1w_read_byte();
        uint8_t t2 = p1w_read_byte();
        p1w_reset();
        p1w_power(0);
        //printf("t1:%u t2:%u\r\n", t1, t2);
        temperature = (float)(t1 + (t2 << 8)) / 16.0;
        dtostrf(temperature, 1, 1, str);
        writeString("\rTemperatura: ");
        writeString(str);
        writeString("              ");
        lcd_goto(1, 0);
        lcd_text(str);
    }
    return 0;
}
