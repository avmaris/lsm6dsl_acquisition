
//-------------------------------------------------< Библиотека для работы с шиной I2C >----------------------------------------------------



#ifndef _SOFT_I2C_H_
#define _SOFT_I2C_H_

#include "../board.h"

#define BYTE unsigned char

#define I2C_DDR		I2C_DIR	//  DDR which correspond to port which connect to I2C pins
#define I2C_PIN		I2C_PINC	//

#define I2C_SCL		I2C_SCL_PIN	// SCL pin
#define I2C_SDA		I2C_SDA_PIN	// SDA pin

#define ACK 0		// ACK defines
#define NACK 1		// NACK defines

#define I2C_DELAY()	delayus(10)	// delay

#define ONE_SCL()	{I2C_DDR &= ~(1 << I2C_SCL); I2C_PORT  |=  (1 << I2C_SCL);}
#define NULL_SCL()	{I2C_DDR |= (1 << I2C_SCL) ;  I2C_PORT &= ~(1 << I2C_SCL);}
#define ONE_SDA()	{I2C_DDR &= ~(1 << I2C_SDA); I2C_PORT  |=  (1 << I2C_SDA);}
#define NULL_SDA()	{I2C_DDR |= (1 << I2C_SDA) ;  I2C_PORT &= ~(1 << I2C_SDA);}
	

// pPrototypes

BYTE i2c_stop(void);			//
void i2c_start(void);			//
void i2c_restart(void);			//
void i2c_init(void);			//
BYTE i2c_send_byte(BYTE data);	//
BYTE i2c_read_byte(BYTE ask);	//

#endif /* I2C_H_ */
