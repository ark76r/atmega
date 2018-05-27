#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>

#define LCDPORT PORTC
#define LCDDDR DDRC
#define LCDD4 (1 << PC0)
#define LCDD5 (1 << PC1)
#define LCDD6 (1 << PC2)
#define LCDD7 (1 << PC3)
#define LCDRS (1 << PC4)
#define LCDE (1 << PC5)

#define SHIFT_LEFT 0
#define SHIFT_RIGH 1
#define SHIFT_CURSOR 0
#define SHIFT_DISPLAY 1

#define DISPLAY_ON 1
#define DISPLAY_OFF 0
#define CURSOR_ON 1
#define CURSOR_OFF 0
#define BLINKING_ON 1
#define BLINKING_OFF 0

void lcd_init(void);
void lcd_control(uint8_t display, uint8_t cursor, uint8_t blinking);
void lcd_clear(void);
void lcd_home(void);
void lcd_goto(uint8_t row, uint8_t col);
void lcd_shift(uint8_t cur_or_disp, uint8_t left_or_right);
void lcd_text(char* str);
void lcd_char(char ch);



#endif
