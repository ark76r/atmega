#include "console.h"

int uart_putchar(char c, FILE *stream)
{
    putByte(c);
    return 0;
}

int uart_getchar(FILE *stream)
{
    return getByte();
}

static FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

void init_console(void)
{
    stdout = &uart_str;
}
