#include <math.h>

#include "../i2c/i2c.h"
#include "../timer0/timer0.h"
#include "mpl3115a2.h"

#define MPL_ADDR 0x60
#define MPL_WHO_AM_I 0x0C
#define MPL_CTRL_REG1 0x26

uint8_t mpl_check(uint8_t reinit_i2c)
{
    if (reinit_i2c)
        i2c_init();
    uint8_t buffer;
    uint8_t res = i2c_read_reg8(MPL_ADDR, MPL_WHO_AM_I, &buffer);
    if (res)
        return 1;
    // in WHO_AM_I should be always 0xc4
    if (buffer != 0xC4)
        return 2;
    return 0;
}

uint8_t mpl_calc_temp()
{
    // 0x3A...
    // ALT = 1
    // RAW = 0
    // OS[2:0] = 111
    // RST = 0
    // OST = 1
    // SBYB = 0
    uint8_t val = 0b00111010;

    return i2c_write_reg8(MPL_ADDR, 0x26, val);
}

float mpl_read_temp()
{
    static uint8_t buffer[2];
    //uint8_t CMD1 = 0x3A;
    uint8_t val = 0xff;

    // wait for data ready
    while (val != 0x38)
    {
        timer0_sleep_ms(10);
        if (i2c_read_reg8(MPL_ADDR, 0x26, &val))
            return NAN;
    }

    if (i2c_read_reg_buf(MPL_ADDR, 0x04, &buffer[0], 2))
        return NAN;

    return (float)buffer[0] + (float)(buffer[1]) / 256.0;
}
