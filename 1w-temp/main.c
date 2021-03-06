#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../lib/timer0/timer0.h"
#include "../lib/dallas1w/dallas1w.h"
#include "../lib/ds18b20/ds18b20.h"
#include "../lib/uart/uart.h"

#define RED "\e[31m"
#define BLUE "\e[34m"
#define RESET "\e[0m"

void print_temp(const char *label, float temp, float diff)
{
    char str[10];

    dtostrf(temp, 3, 2, str);
    writeString(RESET);
    writeString(label);
    if (diff > 0.0)
        writeString(RED);
    else if (diff < 0.0)
        writeString(BLUE);
    writeString(str);
    putByte('\t');
}

int main(void)
{
    float temperature;

    // init cpu freq and timer0
    init_timer0();
    timer0_sleep_ms(100);

    initUART();
    init_ds18b20();

    writeString("\e[0m\e[2J\e[HStart\r\n\e[?25l");

    uint8_t *ds_rom = ds_get_addr(0); // only one is supported now

    if (!ds_rom)
        writeString("No DS18B20\r\n");
    else
    {
        writeString("DS18B20: ");

        for (uint8_t j = 8; j > 0; j--)
        {
            writeHex(ds_rom[j - 1]);
            putByte(' ');
        }
        writeString("\r\n");
    }

    float old_ds = 0.0;

    while (1) //Pętla główna
    {
        writeString("\r\e[0m");

        if (ds_rom)
            ds_convert_t(0); // 0 means first device on the bus

        timer0_sleep_ms(1000);

        if (ds_rom)
        {
            temperature = ds_get_temperature(DS_SKIP_ADDRESS);
            print_temp("DS: ", temperature, temperature - old_ds);
            old_ds = temperature;
        }

        writeString("\e[K");
    }
    return 0;
}
