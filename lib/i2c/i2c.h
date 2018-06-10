#ifndef _I2C_H
#define _I2C_H

#include <stdint.h>

#define I2C_READ 0x01
#define I2C_WRITE 0x00

void i2c_init(void);
uint8_t i2c_start(uint8_t addr);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);

uint8_t i2c_write_reg_buf(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t length);
uint8_t i2c_read_reg_buf(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t length);

uint8_t i2c_read_reg8(uint8_t addr, uint8_t reg, uint8_t* data);
uint8_t i2c_write_reg8(uint8_t addr, uint8_t reg, uint8_t data);

#endif
