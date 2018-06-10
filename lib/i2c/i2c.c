#include "avr/io.h"
#include <util/twi.h>

#include "i2c.h"

#define F_SCL 100000UL  // SCL Freq
#define I2C_PRESCALER 1 // or 4, 16, 64
#define I2C_BITRATE ((((F_CPU / F_SCL) / I2C_PRESCALER) - 16) / 2)

#if I2C_PRESCALER == 1
#define TWPS10_BITS 0x00
#elif I2C_PRESCALER == 4
#define TWPS10_BITS 0x01
#elif I2C_PRESCALER == 16
#define TWPS10_BITS 0x02
#elif I2C_PRESCALER == 64
#define TWPS10_BITS 0x03
#else
#error Prescaler must be 1 4 16 or 64
#endif

void i2c_init(void)
{
	//setup prescaler in TWSR register
	TWSR = (TWSR & 0xF8) | TWPS10_BITS;
	//setup bitrate
	TWBR = (uint8_t)I2C_BITRATE;
}

uint8_t i2c_start(uint8_t addr)
{
	TWCR = 0; //reset
	// send START
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	loop_until_bit_is_set(TWCR, TWINT);

	if (TW_STATUS != TW_START)
		return 1;

	TWDR = addr;
	TWCR = (1 << TWINT) | (1 << TWEN);
	loop_until_bit_is_set(TWCR, TWINT);

	// acknowledged?
	uint8_t status = TW_STATUS;
	if ((status != TW_MT_SLA_ACK) && (status != TW_MR_SLA_ACK))
		return 2;

	return 0;
}

void i2c_stop(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

uint8_t i2c_write(uint8_t data)
{
	TWDR = data;
	// start transmission
	TWCR = (1 << TWINT) | (1 << TWEN);
	loop_until_bit_is_set(TWCR, TWINT);

	if (TW_STATUS != TW_MT_DATA_ACK)
		return 1;

	return 0;
}

uint8_t i2c_read_ack(void)
{
	// start reading and ack
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	loop_until_bit_is_set(TWCR, TWINT);
	return TWDR;
}

uint8_t i2c_read_nack(void)
{
	// start reading and nack
	TWCR = (1 << TWINT) | (1 << TWEN);
	loop_until_bit_is_set(TWCR, TWINT);
	return TWDR;
}

uint8_t i2c_write_reg_buf(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t length)
{
	addr <<= 1;

	if (i2c_start(addr | I2C_WRITE))
		return 1;

	i2c_write(reg);

	for (uint16_t i = 0; i < length; i++)
		if (i2c_write(data[i]))
		{
			i2c_stop();
			return 1;
		}

	i2c_stop();

	return 0;
}

uint8_t i2c_read_reg_buf(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t length)
{
	addr <<= 1;
	if (i2c_start(addr | I2C_WRITE))
		return 1;

	i2c_write(reg);

	if (i2c_start(addr | I2C_READ))
		return 1;

	for (uint16_t i = 0; i < (length - 1); i++)
		data[i] = i2c_read_ack();

	data[(length - 1)] = i2c_read_nack();

	i2c_stop();

	return 0;
}

uint8_t i2c_write_reg8(uint8_t addr, uint8_t reg, uint8_t data)
{
	addr <<= 1;

	if (i2c_start(addr | I2C_WRITE))
		return 1;

	i2c_write(reg);
	i2c_write(data);

	i2c_stop();

	return 0;
}

uint8_t i2c_read_reg8(uint8_t addr, uint8_t reg, uint8_t *data)
{
	addr <<= 1;
	if (i2c_start(addr | I2C_WRITE))
		return 1;

	i2c_write(reg);

	if (i2c_start(addr | I2C_READ))
		return 1;

	*data = i2c_read_nack();

	i2c_stop();

	return 0;
}
