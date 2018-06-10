#include <util/delay.h>
#include <avr/interrupt.h>
#include "dallas1w.h"

uint8_t p1w_reset(void)
{
    uint8_t counter = 125;
    uint8_t present = 0;
    ONEW_INPUT();
    while (--counter > 0 && !ONEW_VALUE)
        _delay_us(2);
    cli();
    ONEW_OUTPUT();
    ONEW_LOW();
    sei();
    _delay_us(480);
    cli();
    ONEW_INPUT();
    _delay_us(70);
    present = ONEW_VALUE;
    sei();
    _delay_us(410);
    return !present;
}

void p1w_write_slot(uint8_t bit)
{
    cli();
    ONEW_OUTPUT();
    ONEW_LOW();
    if (bit)
    {
        _delay_us(10);
        ONEW_HIGH();
        _delay_us(55);
    }
    else
    {
        _delay_us(65);
        ONEW_HIGH();
        _delay_us(5);
    }
    sei();
}
uint8_t p1w_read_slot(void)
{
    uint8_t r;
    cli();
    ONEW_OUTPUT();
    ONEW_LOW();
    _delay_us(3);
    ONEW_INPUT();
    _delay_us(10);
    r = ONEW_VALUE;
    sei();
    _delay_us(53);
    return r;
}

void p1w_power(uint8_t poweron)
{
    if (poweron)
    {
        ONEW_OUTPUT();
        ONEW_HIGH(); // high power
    }
    else
    {
        ONEW_INPUT(); // tri-state
    }
}

void p1w_write_byte(uint8_t b)
{
    uint8_t mask;
    for (mask = 0x01; mask; mask <<= 1)
        p1w_write_slot((b & mask) ? 1 : 0);
}

uint8_t p1w_read_byte()
{
    uint8_t r = 0;
    uint8_t mask;

    for (mask = 0x01; mask; mask <<= 1)
        if (p1w_read_slot())
            r |= mask;

    return r;
}

uint8_t *p1w_search_rom(void)
{
    static uint8_t rom[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t i, j;
    uint8_t present = p1w_reset();
    if (!present)
    {
        return (uint8_t *)0;
    }
    p1w_write_byte(0xF0); // search_rom

    for (j = 0; j < 8; j++)
    {
        for (i = 0x01; i; i <<= 1)
        {
            uint8_t tval = p1w_read_slot();
            uint8_t fval = p1w_read_slot();
            if (tval == 0 && fval == 0)
                tval = 1;
            p1w_write_slot(tval);
            rom[j] |= tval ? i : 0;
        }
    }
    return rom;
}
