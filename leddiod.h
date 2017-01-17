/*
 * leddiod.h
 *
 *  Created on: Nov 16, 2016
 *      Author: marisov
 */

#ifndef LEDDIOD_H_
#define LEDDIOD_H_
#include "board.h"
#include "driverlib.h"


void ledInit(uint8_t selectedPort,
        uint16_t selectedPins);

void ledStateSet(char);

void loBattSygnalling();
void blink(int bNum);

#endif /* LEDDIOD_H_ */
