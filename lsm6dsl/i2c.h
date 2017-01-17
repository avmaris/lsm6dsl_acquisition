//-----------------------------------------------------------------------------
// spi.h - common function declarations for different SPI implementations
//-----------------------------------------------------------------------------
#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

void i2c_initialize(void);
//uint8_t i2c_receive(void);
//uint8_t i2c_send(const uint8_t c);
uint8_t readByte(uint8_t devDdr, uint8_t reg);
uint8_t writeByte(uint8_t devDdr, uint8_t reg, uint8_t data);
uint8_t readMultipleByte(uint8_t devDdr,
		uint8_t reg,
		uint8_t* buf,
		uint8_t num);
#endif /*I2C_H_*/
