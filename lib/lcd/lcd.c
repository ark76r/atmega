#include "lcd.h"

#define PULSE()           \
    do                    \
    {                     \
        LCDPORT |= LCDE;  \
        _delay_us(5);     \
        LCDPORT &= ~LCDE; \
    } while (0)

static void lcd_send(uint8_t rs, uint8_t data)
{
    _delay_ms(5);
    if (rs)
        LCDPORT |= LCDRS;
    else
        LCDPORT &= ~LCDRS;

    if (data & 0b10000000)
        LCDPORT |= LCDD7;
    else
        LCDPORT &= ~LCDD7;
    if (data & 0b01000000)
        LCDPORT |= LCDD6;
    else
        LCDPORT &= ~LCDD6;
    if (data & 0b00100000)
        LCDPORT |= LCDD5;
    else
        LCDPORT &= ~LCDD5;
    if (data & 0b00010000)
        LCDPORT |= LCDD4;
    else
        LCDPORT &= ~LCDD4;

    PULSE();

    if (data & 0b00001000)
        LCDPORT |= LCDD7;
    else
        LCDPORT &= ~LCDD7;
    if (data & 0b00000100)
        LCDPORT |= LCDD6;
    else
        LCDPORT &= ~LCDD6;
    if (data & 0b00000010)
        LCDPORT |= LCDD5;
    else
        LCDPORT &= ~LCDD5;
    if (data & 0b00000001)
        LCDPORT |= LCDD4;
    else
        LCDPORT &= ~LCDD4;

    PULSE();
}

void lcd_char(char ch)
{
    lcd_send(1, ch);
}

void lcd_text(char *str)
{
    while (*str != '\0')
    {
        lcd_send(1, *str);
        ++str;
    }
}

void lcd_goto(uint8_t row, uint8_t col)
{
    if (row > 1)
        row = 1;
    lcd_send(0, 0x80 | (0x7f & (row * 0x40 + col)));
}

void lcd_shift(uint8_t cur_or_disp, uint8_t left_or_right)
{
    uint8_t cmd = 0x10;
    if (cur_or_disp)
        cmd |= 1 << 3;
    if (left_or_right)
        cmd |= 1 << 2;
    lcd_send(0, cmd);
}

void lcd_init(void)
{
    LCDDDR = LCDE | LCDRS | LCDD4 | LCDD5 | LCDD6 | LCDD7;

    lcd_send(0, 0x20); // 4 bits
    lcd_send(0, 0x28); // 2 lines
}

void lcd_control(uint8_t display, uint8_t cursor, uint8_t blinking)
{
    uint8_t cmd = 0b00001000;
    if (display)
        cmd |= (1 << 2);
    if (cursor)
        cmd |= (1 << 1);
    if (blinking)
        cmd |= 1;
    lcd_send(0, cmd);
}
void lcd_clear(void)
{
    lcd_send(0, 1); // clear command
}
void lcd_home(void)
{
    lcd_send(0, 2);
}
