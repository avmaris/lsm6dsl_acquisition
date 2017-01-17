/*
 * leddiod.c
 *
 *  Created on: Nov 16, 2016
 *      Author: marisov
 */
#include "leddiod.h"

struct GPIO_containar_Struc D1;
/*
 * ledinit
 * Initialize port wich connected to diode
 * @ * dir - port name
 * @ * out - pin number
 * */
void ledInit(uint8_t selectedPort,
        uint16_t selectedPins){
	D1.port = selectedPort;
	D1.pin = selectedPins;

    GPIO_setAsOutputPin(D1.port, D1.pin);
    GPIO_setOutputLowOnPin(D1.port, D1.pin);
}

void ledStateSet(char state){
	switch (state){
		case(ON):
			GPIO_setOutputHighOnPin(D1.port, D1.pin);
			break;

		case(OFF):
			GPIO_setOutputLowOnPin(D1.port, D1.pin);
			break;

		case(CHANGE):
			GPIO_toggleOutputOnPin(D1.port, D1.pin);
			break;
	}
}

void blink(int bNum)
{
	int j;
	unsigned char previesState;
	previesState = GPIO_getInputPinValue(D1.port, D1.pin);
	for(j = (bNum + 1); j; j--){
	    	__delay_cycles(62500);
			ledStateSet(CHANGE);
	    }
	if(previesState != GPIO_getInputPinValue(D1.port, D1.pin))
		ledStateSet(CHANGE);
}

void loBattSygnalling(){

}
