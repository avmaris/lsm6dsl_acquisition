/*
 * lsm6dsl.c
 *
 *  Created on: Nov 16, 2016
 *      Author: marisov
 */
#include "lsm6dsl.h"
//#include "i2c.h"
#include <stdint.h>
#include <stdio.h>
#include "../lsm6dsl/softi2c.h"
#include "../lsm6dsl/i2c.h"

char lsm6dslPowerMode(char a)
{
	return 0;
}

/*
 * Accelerator initializing
 * */
char lsm6dslInit(){
	//i2c_initialize();
	uint8_t retCode = 0xFF;
	P2DIR &= ~(GPIO_PIN3 | GPIO_PIN1);
	i2c_init();
	retCode = writeByte(LSM6DSL_SLAVE_ADDRESS, CTRL1_XL, ODR_XL_RATE_6_66kHz_6_66kHz);
	retCode = writeByte(LSM6DSL_SLAVE_ADDRESS, CTRL2_G, ODR_G_RATE_6_66_Khz_6_66_Khz | FS_125);

	return 0;
}


char lsm6dslGXL_Read(struct xyzDatStr *acs,
		struct xyzDatStr *grs){

	i2c_init();

	delayms(1);

	readMultipleByte(LSM6DSL_SLAVE_ADDRESS,
			OUTX_L_G,
			(uint8_t*)(acs),
			sizeof(struct xyzDatStr)
			);

	readMultipleByte(LSM6DSL_SLAVE_ADDRESS,
			OUTX_L_XL,
			(uint8_t*)(grs),
			sizeof(struct xyzDatStr));

	return 0;
}

char lsm6dslTest(){

/*	i2c_init();

	delayms(1);

	readMultipleByte(LSM6DSL_SLAVE_ADDRESS,
			OUTX_L_G,
			(uint8_t*)(&acs[gsxlPoi]),
			sizeof(struct xyzDatStr)
			);

	readMultipleByte(LSM6DSL_SLAVE_ADDRESS,
			OUTX_L_XL,
			(uint8_t*)(&grs[gsxlPoi]),
			sizeof(struct xyzDatStr));

	if(gsxlPoi < grasSize)
		gsxlPoi++;
	else
	{
		printG_XK();
	}*/
	return 0;
}
