#ifndef _mpl3115a2_h
#define mpl3115a2
#include <stdint.h>

uint8_t mpl_check(uint8_t reinit_i2c);
uint8_t mpl_calc_temp();
float mpl_read_temp();
#endif
