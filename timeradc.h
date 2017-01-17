/*
 * timeradc.h
 *
 *  Created on: Nov 29, 2016
 *      Author: marisov
 */

#ifndef TIMERADC_H_
#define TIMERADC_H_


uint32_t getV(uint16_t ref, uint8_t inputSourceSelect);
void timerInit();
void ADCInit();
uint32_t getVbat();
void battSignalingOn();
void battSignalingOff();
void battSignaliningInit();
#endif /* TIMERADC_H_ */
