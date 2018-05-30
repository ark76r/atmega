
#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdint.h>

/* Settings */
#define BAUD			9600					// Baud rate (9600 is default)
#define RX_BUFF			10

#define UART_ISR // to use interrupts

/* Prototypes */
void initUART(void);
uint8_t getByte(void);
void putByte(unsigned char data);
void writeString(char *str);
void writeHex(uint8_t byte);

const char* readString(void);

uint8_t uart_data_ready(void);

#endif /* UART_H_ */
