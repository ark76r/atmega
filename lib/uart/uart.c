#include "uart.h"
#include <util/setbaud.h>
#include <avr/sleep.h>
#ifdef UART_ISR
#include <avr/interrupt.h>
#endif
static char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

volatile uint8_t cbuff[16]; // cyclic buffer

struct cb_inticies {
	uint8_t front : 4;
	uint8_t end: 4;
};

volatile struct cb_inticies idxs;

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

#ifdef UART_ISR
	UCSR0B |= (1 << RXCIE0); // enable interrput
#endif
}

#ifdef UART_ISR
static uint8_t cb_get_byte(void);
#endif

uint8_t getByte(void)
{
#ifdef UART_ISR
	return cb_get_byte();
#else
	while (!(UCSR0A & (1 << RXC0)))
		;
	return (uint8_t)UDR0;
#endif
}

void putByte(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)))
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


// cyclic buffer implementation

uint8_t uart_data_ready(void)
{
#ifdef UART_ISR
	return !(idxs.front == idxs.end);

#else
	return (UCSR0A & (1 << UDRE0));
#endif
}
#ifdef UART_ISR

static void cb_put_byte(uint8_t b)
{
	cbuff[ (++idxs.end) & (sizeof(cbuff)-1)] = b;
}

static uint8_t cb_get_byte(void)
{
	while (idxs.front == idxs.end)
	{
		sleep_cpu();
	}
	return cbuff[(++idxs.front) & (sizeof(cbuff)-1)];
}

ISR(USART_RX_vect)
{
	cb_put_byte((uint8_t)UDR0);
}
#endif
