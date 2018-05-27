#include "uart.h"
#include <util/setbaud.h>

static char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void initUART(void)
{
	UBRR0H = UBRRH_VALUE; /* defined in setbaud.h */
	UBRR0L = UBRRL_VALUE;
#if USE_2X
	UCSR0A |= (1 << U2X0);
#else
	UCSR0A &= ~(1 << U2X0);
#endif
	/* Enable USART transmitter/receiver */
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); /* 8 data bits, 1 stop bit */
}

uint8_t getByte(void)
{
	while (!(UCSR0A & _BV(RXC0)))
		;
	return (uint8_t)UDR0;
}

void putByte(unsigned char data)
{
	while (!(UCSR0A & _BV(UDRE0)))
		;
	UDR0 = (unsigned char)data;
}

void writeHex(uint8_t byte)
{
	putByte(digits[(byte >> 4)]);
	putByte(digits[byte & 0x0f]);
}

void writeString(char *str)
{
	while (*str)
	{
		putByte(*str);
		str++;
	}
}

const char *readString(void)
{
	static char buffer[RX_BUFF];
	char *temp = buffer;

	while ((*temp = getByte()) != '\n')
		++temp;

	return buffer;
}
