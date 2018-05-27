
#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdint.h>

/* Settings */
#define BAUD			9600					// Baud rate (9600 is default)
#define RX_BUFF			10

/* Useful macros */
#define TX_START()		UCSR0B |= _BV(TXEN0)	// Enable TX
#define TX_STOP()		UCSR0B &= ~_BV(TXEN0)	// Disable TX
#define RX_START()		UCSR0B |= _BV(RXEN0)	// Enable RX
#define RX_STOP()		UCSR0B &= ~_BV(RXEN0)	// Disable RX
#define COMM_START()		TX_START(); RX_START()	// Enable communications

/* Interrupt macros; Remember to set the GIE bit in SREG before using (see datasheet) */
#define RX_INTEN()		UCSR0B |= _BV(RXCIE0)	// Enable interrupt on RX complete
#define RX_INTDIS()		UCSR0B &= ~_BV(RXCIE0)	// Disable RX interrupt
#define TX_INTEN()		UCSR0B |= _BV(TXCIE0)	// Enable interrupt on TX complete
#define TX_INTDIS()		UCSR0B &= ~_BV(TXCIE0)	// Disable TX interrupt

/* Prototypes */
void initUART(void);
uint8_t getByte(void);
void putByte(unsigned char data);
void writeString(char *str);
void writeHex(uint8_t byte);

const char* readString(void);

#endif /* UART_H_ */
