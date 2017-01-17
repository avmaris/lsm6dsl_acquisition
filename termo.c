/*
 * termo.c
 *
 *  Created on: Nov 29, 2016
 *      Author: marisov
 */
#include "driverlib.h"
#include <stdio.h>
#include "timeradc.h"
#include "board.h"

int mv2DC(int mV)
{
	//float temper;
	//temper = (212.009 - (float)mV * 0.193);

	return (21200 - 19*mV);
};

void termoPon(){
	GPIO_setOutputHighOnPin(TEMP_PWR_PORT, TEMP_PWR_PIN);
	GPIO_setOutputHighOnPin(TEMP_TON_PORT, TEMP_TON_PIN);
	delayms(1);
};

void termoPoff(){
    GPIO_setOutputLowOnPin(TEMP_TON_PORT, TEMP_TON_PIN);
    GPIO_setOutputLowOnPin(TEMP_PWR_PORT, TEMP_PWR_PIN);
};


void termoInit(void){
	/*
	 * Control pin init
	 * */
    GPIO_setOutputLowOnPin(TEMP_TON_PORT, TEMP_TON_PIN);
    GPIO_setOutputLowOnPin(TEMP_PWR_PORT, TEMP_PWR_PIN);
    GPIO_setAsOutputPin(TEMP_TON_PORT, TEMP_TON_PIN);
    GPIO_setAsOutputPin(TEMP_PWR_PORT, TEMP_PWR_PIN);

    GPIO_setAsPeripheralModuleFunctionInputPin(
    	TEMP_TAO_PORT,
		TEMP_TAO_PIN,
		GPIO_SECONDARY_MODULE_FUNCTION);

    termoPon();
};

long readT(){
	//static uint16_t counter = 0;
	//ADCInit();
//	termoPon();
	long  temper = getV(REF_VREF1_5V, TEMP_TAO_AN);
//	termoPoff();
	return (21200 - 19*temper);
	//counter++;
	//return counter;
};
