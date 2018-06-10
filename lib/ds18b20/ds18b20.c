#include <avr/io.h>
#include <stdlib.h>
#include <math.h>

#include "ds18b20.h"
#include "../dallas1w/dallas1w.h"

#define DS_CMD_CONVERT_T 0x44
#define DS_CMD_READ_SCRATCHPAD 0xBE

static uint8_t *roms[DS_MAX_DEVICES_ON_BUS];

int8_t init_ds18b20(void)
{
    for (uint8_t i = 0; i < DS_MAX_DEVICES_ON_BUS; i++)
        roms[i] = NULL;

    uint8_t *rom = p1w_search_rom();

    if (!rom || rom[0] != 0x28) // familly of ds12b20
        rom = NULL;

    roms[0] = rom;

    if (rom == NULL)
        return -1;
    else
        return 0;
}

uint8_t *ds_get_addr(const uint8_t deviceno)
{
    if (deviceno < DS_MAX_DEVICES_ON_BUS)
        return roms[deviceno];
    return NULL;
}

static int8_t transmit_address(const uint8_t deviceno)
{
    uint8_t present = p1w_reset();
    if (!present)
        return -1;
    if (deviceno == DS_SKIP_ADDRESS)
    {
        p1w_write_byte(ONEW_SKIP_ROM);
    }
    else if (deviceno < DS_MAX_DEVICES_ON_BUS)
    {
        uint8_t *rom = roms[deviceno];
        if (rom == NULL)
            return -3;
        p1w_write_byte(ONEW_MATCH_ROM); // MATCH ROM
        for (uint8_t j = 0; j < 8; j++)
            p1w_write_byte(rom[j]);
    }
    else
    {
        return -2;
    }
    return 0;
}

int8_t ds_convert_t(const uint8_t deviceno)
{
    int8_t res = transmit_address(deviceno);
    if (res != 0)
        return res;
    p1w_write_byte(DS_CMD_CONVERT_T);
    p1w_power(1);
    return 0;
}

float ds_get_temperature(const uint8_t deviceno)
{
    int8_t res = transmit_address(deviceno);
    if (res != 0)
        return NAN;
    p1w_write_byte(DS_CMD_READ_SCRATCHPAD);
    uint8_t t1 = p1w_read_byte();
    uint8_t t2 = p1w_read_byte();
    p1w_reset();  // end of reading
    p1w_power(0); // low power on the bus

    return (float)(t1 + (t2 << 8)) / 16.0;
}
