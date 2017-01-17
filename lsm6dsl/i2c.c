/**
 * File: i2c.c - msp430 eUSCI I2C implementation
 * Created on: Nov 16, 2016
 * 	Author: Marisov A.V.
 */
#include "driverlib.h"
#include <stdint.h>
#include "../lsm6dsl/i2c.h"
#include "../lsm6dsl/lsm6dsl.h"
#include "../board.h"
#include "../lsm6dsl/softi2c.h"

// Prototypes of TX/RX wait functions
//uint8_t spi_waitTX();
//uint8_t spi_waitRX();
#ifdef HARD_SPI
void trick(){
	uint8_t i = 0;
    GPIO_setAsOutputPin(UCX_I2C_PORT, UCX_I2C_SDA_PIN + UCX_I2C_SCL_PIN);

    for(i = 0; i < 10; i++){
		 __delay_cycles(100);
		if(i&1)
			GPIO_toggleOutputOnPin(UCX_I2C_PORT,UCX_I2C_SDA_PIN);
		if(i&2)
			GPIO_toggleOutputOnPin(UCX_I2C_PORT,UCX_I2C_SCL_PIN);
    }
    GPIO_setOutputHighOnPin(UCX_I2C_PORT,UCX_I2C_SDA_PIN);
    GPIO_setOutputHighOnPin(UCX_I2C_PORT,UCX_I2C_SCL_PIN);

}
#endif

void i2c_initialize(void) {

#ifdef HARD_SPI
	trick(); // stuck i2c trick from forum
	   //Assign I2C pins to USCI_B0
	GPIO_setAsPeripheralModuleFunctionInputPin(
			UCX_I2C_PORT,
			UCX_I2C_SDA_PIN + UCX_I2C_SCL_PIN,
			GPIO_SECONDARY_MODULE_FUNCTION
		 );
	PMM_unlockLPM5();
	//Initialize Master
	EUSCI_B_I2C_initMasterParam param = {0};
	param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
	param.i2cClk = CS_getSMCLK();
	param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_100KBPS;
	param.byteCounterThreshold = 1;
	param.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP;
	EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);

	//Specify slave address
	EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE, LSM6DSL_SLAVE_ADDRESS);

	//Set in transmit mode
	EUSCI_B_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_MODE);

	//Enable I2C Module to start operations
	EUSCI_B_I2C_enable(EUSCI_B0_BASE);

    EUSCI_B_I2C_clearInterrupt(EUSCI_B0_BASE,
    		EUSCI_B_I2C_RECEIVE_INTERRUPT0 + EUSCI_B_I2C_BYTE_COUNTER_INTERRUPT
                               );
    //Wait for slave to initialize
#else
    i2c_init();
#endif
}

#ifdef HARD_SPI
uint8_t i2c_sendBusyWait(void)
{
#ifdef HARD_SPI
    //Delay until transmission completes
    while(EUSCI_B_I2C_isBusBusy(EUSCI_B0_BASE))
    {
    	// TODO: Timeout
    }
#endif
	return 0;
}
#endif

///**
// * spi_send() - send a byte and recv response
// * Logic of sending token from an example and could be improved.
// */
//uint8_t i2c_send(uint8_t c) {
//	uint8_t RXDataAnsfer = 0;
//    //Send single byte data.
//#ifdef HARD_SPI
//	EUSCI_B_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
//    EUSCI_B_I2C_masterSendSingleByte(EUSCI_B0_BASE, c);
//    i2c_sendBusyWait();
//#else
//    return i2c_send_byte(c);
//#endif
//    return RXDataAnsfer; // read data
//}
//
///**
// * spi_receive() - send dummy btye then recv response
// *
// */
//uint8_t i2c_receive(void) {
//	uint8_t RXData = 0;
//#ifdef HARD_SPI
//	EUSCI_B_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_RECEIVE_MODE);
//	RXData = EUSCI_B_I2C_masterReceiveSingleByte(EUSCI_B0_BASE);
//#endif
//    return RXData;
//}



uint8_t readMultipleByte(uint8_t devDdr,
		uint8_t reg,
		uint8_t* buf,
		uint8_t num)
{
	uint8_t retCode = 0xFF;
	uint8_t i;

	i2c_start();
	retCode = i2c_send_byte((devDdr << 1));
	retCode =  i2c_send_byte(reg);
	retCode =  i2c_stop();
	i2c_start();
	retCode = i2c_send_byte((devDdr << 1) | 0x01);
	for(i = 0; i < num; i++)
		if((i + 1) < num)
			*(buf++) =  i2c_read_byte(ACK);
		else
			*buf =  i2c_read_byte(NACK);

	i2c_stop();
	return retCode;
}

uint8_t readByte(uint8_t devDdr, uint8_t reg){
	uint8_t retCode = 0xFF;
	i2c_start();
	retCode = i2c_send_byte((devDdr << 1));
	retCode =  i2c_send_byte(reg);
	retCode =  i2c_stop();
	i2c_start();
	retCode = i2c_send_byte((devDdr << 1) | 0x01);
	retCode =  i2c_read_byte(NACK);
	i2c_stop();
	return retCode;
}

uint8_t writeByte(uint8_t devDdr, uint8_t reg, uint8_t data){
	uint8_t retCode = 0xFF;
	i2c_start();
	retCode = i2c_send_byte((devDdr << 1));
	if(!retCode)
		retCode =  i2c_send_byte(reg);
	if(!retCode)
		retCode =  i2c_send_byte(data);
	if(!retCode)
		retCode =  i2c_stop();

	return retCode;
}

#ifndef HARD_SPI

#endif
