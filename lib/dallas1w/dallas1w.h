#ifndef _DALLAS1W
#define _DALLAS1W
#include <avr/io.h>

#define ONEW_PORT PORTB
#define ONEW_DDR DDRB
#define ONEW_PIN PINB
#define ONEW (1 << PB1)

#define ONEW_SKIP_ROM 0xCC
#define ONEW_MATCH_ROM 0x55

#define ONEW_OUTPUT() do { ONEW_DDR |= ONEW; } while(0)
#define ONEW_INPUT() do { ONEW_DDR &= ~ONEW; } while(0)
#define ONEW_LOW() do { ONEW_PORT &= ~ONEW; } while(0)
#define ONEW_HIGH() do {ONEW_PORT |= ONEW; } while(0)

#define ONEW_VALUE (ONEW_PIN & ONEW)

uint8_t p1w_reset(void);
void p1w_write_slot(uint8_t bit);
uint8_t p1w_read_slot(void);
void p1w_power(uint8_t poweron);
void p1w_write_byte(uint8_t b);
uint8_t p1w_read_byte();

uint8_t *p1w_search_rom(void); // TODO, now it works only with one ROM on the bus

#endif
