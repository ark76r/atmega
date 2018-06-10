#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include "../lib/uart/uart.h"
#include "../lib/lcd/lcd.h"

// Pętla główna
int main(void)
{

    initUART();
    writeString("Start\r\n");

    _delay_ms(2000);
    lcd_init();
    writeString("init HC done\r\n");

    lcd_control(1, 1, 0);

    while (1) //Pętla główna
    {
        lcd_clear();
        lcd_home();
        _delay_ms(500); //opóźnienie 1000ms czyli 1s
        lcd_text("Hello!");
        _delay_ms(1000);
    }
    return 0;
}



