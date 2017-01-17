/*
 * termo.h
 *
 *  Created on: Nov 29, 2016
 *      Author: marisov
 */

#ifndef TERMO_H_
#define TERMO_H_

void termoInit(void);
long readT();

void termoPon();
void termoPoff();
int mv2DC(int  mV);

#endif /* TERMO_H_ */
