
//----------------------------------------< Soft I2C >-----------------------------------------------



//=======================================================================
//
//=======================================================================

#include <msp430.h>
#include "../lsm6dsl/softi2c.h"
#include "../board.h"


//=======================================================================
//					       stop generate
//=======================================================================

BYTE i2c_stop(void)
	{
		BYTE error = 0;
		
		NULL_SCL();
		I2C_DELAY();
		NULL_SDA();
		I2C_DELAY();

		ONE_SCL();
		I2C_DELAY();
		ONE_SDA();
		I2C_DELAY();
		
		if((I2C_PIN & (1 << I2C_SDA)) == 0) error = 2;
		if((I2C_PIN & (1 << I2C_SCL)) == 0) error |= 1;
		I2C_DELAY();
		I2C_DELAY();
		I2C_DELAY();
		I2C_DELAY();
		
		return error;
	}
	
//=======================================================================
//					       start generate
//=======================================================================

void i2c_start(void)
	{
		NULL_SDA();
		I2C_DELAY();
		NULL_SCL();
		I2C_DELAY();
	}
	
//=======================================================================
//					       restart generate
//=======================================================================

void i2c_restart(void)
	{
		ONE_SDA();
		I2C_DELAY();
		ONE_SCL();
		I2C_DELAY();
		
		NULL_SDA();
		I2C_DELAY();
		NULL_SCL();
		I2C_DELAY();
	}
	
//=======================================================================
//					       Initialize buss
//=======================================================================

void i2c_init(void)
	{
		ONE_SDA();
		ONE_SCL();
		
		i2c_stop();
	}
	
//=======================================================================
//					       Send byte by buss
//=======================================================================

BYTE i2c_send_byte(BYTE data)
	{
		BYTE i;
		BYTE ask = ACK;
		
		for(i = 0; i < 8; i++)
			{
				if((data & 0x80) == 0)
					{
						NULL_SDA();
					}
				else
					{
						ONE_SDA();
					}
				I2C_DELAY();
				ONE_SCL();
				I2C_DELAY();
				NULL_SCL();
				
				data = (data << 1);
			}
			
		ONE_SDA();
		I2C_DELAY();
		ONE_SCL();
		I2C_DELAY();
		
		if((I2C_PIN & (1 << I2C_SDA)) == (1 << I2C_SDA))
			{
				ask = NACK;
			}
		else
			{
				ask = ACK;
			}
			
		NULL_SCL();

		return ask;
	}
	
//=======================================================================
//					       Read byte
//=======================================================================

BYTE i2c_read_byte(BYTE ask)
	{
		BYTE byte = 0;
		BYTE i;
		
		ONE_SDA();

		for(i = 0; i < 8; i++)
			{
				byte = (byte << 1);
				ONE_SCL();
				I2C_DELAY();
				if((I2C_PIN & (1 << I2C_SDA)) == (1 << I2C_SDA)) byte |= 0x01;
				NULL_SCL();
				I2C_DELAY();
			}
			
		if(ask == ACK)
			{
				NULL_SDA();
			}
		else
			{
				ONE_SDA();
			}
		I2C_DELAY();
		ONE_SCL();
		I2C_DELAY();
		NULL_SCL();
		I2C_DELAY();
		ONE_SDA();
			
		return byte;
	}
