#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#ifndef F_OSC
#define F_OSC 8000000
#endif

#if (F_OSC/F_CPU == 1)
#define CPU_DIV clock_div_1
#elif (F_OSC/F_CPU == 2)
#define CPU_DIV clock_div_2
#elif (F_OSC/F_CPU == 4)
#define CPU_DIV clock_div_4
#elif (F_OSC/F_CPU == 8)
#define CPU_DIV clock_div_8
#elif (F_OSC/F_CPU == 16)
#define CPU_DIV clock_div_16
#elif (F_OSC/F_CPU == 32)
#define CPU_DIV clock_div_32
#elif (F_OSC/F_CPU == 64)
#define CPU_DIV clock_div_64
#elif (F_OSC/F_CPU == 128)
#define CPU_DIV clock_div_128
#elif (F_OSC/F_CPU == 256)
#define CPU_DIV clock_div_256
#else
#error "Wrong F_OSC/F_CPU combination"
#endif



#define TIMER0_PS_1  (1 << CS00)
#define TIMER0_PS_8  (1 << CS01)
#define TIMER0_PS_64  ((1 << CS00) |  (1 << CS01))

#if   (F_CPU == 20000000)
#define TIMER0_PS TIMER0_PS_8
#define TIMER0_COMP 250
#elif (F_CPU == 16000000)
#define TIMER0_PS TIMER0_PS_64
#define TIMER0_COMP 25
#elif (F_CPU == 12000000)
#define TIMER0_PS TIMER0_PS_8
#define TIMER0_COMP 150
#elif (F_CPU ==  8000000)
#define TIMER0_PS TIMER0_PS_8
#define TIMER0_COMP 100
#elif (F_CPU ==  4000000)
#define TIMER0_PS TIMER0_PS_8
#define TIMER0_COMP 50
#elif (F_CPU ==  2000000)
#define TIMER0_PS TIMER0_PS_8
#define TIMER0_COMP 25
#elif (F_CPU ==  1000000)
#define TIMER0_PS TIMER0_PS_1
#define TIMER0_COMP 100
#else
#define TIMER0_PS TIMER0_PS_1
#define TIMER0_COMP 100
#error "Timer0: No defaults for this F_CPU"
#endif

static volatile uint8_t timer0; // changes each 100us

void init_timer0(void)
{
    clock_prescale_set(CPU_DIV);
    sleep_enable();

    timer0 = 0;
    OCR0A |= TIMER0_COMP - 1; // match each 100 cycles
    TCCR0A |= (1<< WGM01);
    TIMSK0 |= (1<<OCIE0A);
    //TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
    TCCR0B |= TIMER0_PS; // turn on timer
    sei();
}

void timer0_sleep_ms(uint16_t m)
{
    for(; m>0; m--)
    {
        uint8_t old_t0 = timer0;
        while ((uint8_t) (timer0 - old_t0) < 10) // timer0 tics: each 100 us hece we wait 10 ticks
        {
            // writeHex(old_t0);
            // putByte(' ');
            // writeHex(timer0);
            // putByte(' ');
            // writeHex(timer0 - old_t0);
            // writeString("\r\n");
            sleep_cpu();
        }
    }
}

// fires each 100us
ISR(TIMER0_COMPA_vect)
{
    timer0++;
}

