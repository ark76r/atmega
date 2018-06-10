#ifndef _DS18B20
#define _DS18B20
#include <stdint.h>

// now only one device is supported
#define DS_MAX_DEVICES_ON_BUS 1
#define DS_SKIP_ADDRESS ((uint8_t) DS_MAX_DEVICES_ON_BUS)

int8_t init_ds18b20(void);
uint8_t *ds_get_addr(const uint8_t deviceno);
int8_t ds_convert_t(const uint8_t deviceno);
float ds_get_temperature(const uint8_t deviceno);

#endif
