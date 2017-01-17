/*
 * lsm6dsl.h
 *
 *  Created on: Nov 16, 2016
 *      Author: marisov
 */

#ifndef LSM6DSL_H_
#define LSM6DSL_H_
#include <stdint.h>
#define LSM6DSL_SLAVE_ADDRESS 0x6A /*1101010*/

#define ODR_MASK 0x0F

#define ODR_XL_RATE_POW_DOWN 		0x00 /*Power-down Power-down                                      */
#define ODR_XL_RATE_1_6Hz_12_5Hz	0xB0/*1.6 Hz (low power only) 12.5 Hz (high performance)         */
#define ODR_XL_RATE_12_5Hz_12_5Hz	0x10 /*12.5 Hz (low power) 12.5 Hz (high performance)             */
#define ODR_XL_RATE_26Hz_26Hz		0x20 /*26 Hz (low power) 26 Hz (high performance)                 */
#define ODR_XL_RATE_52Hz_52Hz		0x30 /*52 Hz (low power) 52 Hz (high performance)                 */
#define ODR_XL_RATE_104Hz_104Hz		0x40 /*104 Hz (normal mode) 104 Hz (high performance)             */
#define ODR_XL_RATE_208Hz_208Hz		0x50 /*208 Hz (normal mode) 208 Hz (high performance)             */
#define ODR_XL_RATE_416Hz_416Hz		0x60 /*416 Hz (high performance) 416 Hz (high performance)        */
#define ODR_XL_RATE_833Hz_833Hz		0x70 /*833 Hz (high performance) 833 Hz (high performance)        */
#define ODR_XL_RATE_1_66kHz_1_66kHz 0x80 /*1.66 kHz (high performance) 1.66 kHz (high performance)    */
#define ODR_XL_RATE_3_33kHz_3_33kHz 0x90 /*3.33 kHz (high performance) 3.33 kHz (high performance)    */
#define ODR_XL_RATE_6_66kHz_6_66kHz 0xA0 /*6.66 kHz (high performance) 6.66 kHz (high performance)    */

#define ODR_G_RATE_ 0x00 /* Power down Power down	*/
#define ODR_G_RATE_12_5_hz_12_5_hz 	0x10 /* 12.5 Hz (low power) 12.5 Hz (high performance)	*/
#define ODR_G_RATE_26_hz_26_hz		0x20 /* 26 Hz (low power) 26 Hz (high performance)	*/
#define ODR_G_RATE_52_hz_52_hz		0x30 /* 52 Hz (low power) 52 Hz (high performance)	*/
#define ODR_G_RATE_104_hz_104_hz	0x40 /* 104 Hz (normal mode) 104 Hz (high performance)	*/
#define ODR_G_RATE_208_hz_208_hz	0x50 /* 208 Hz (normal mode) 208 Hz (high performance)	*/
#define ODR_G_RATE_416_hz_416_hz	0x60 /* 416 Hz (high performance) 416 Hz (high performance)	*/
#define ODR_G_RATE_833_hz_833_hz	0x70 /* 833 Hz (high performance) 833 Hz (high performance)	*/
#define ODR_G_RATE_1_66_Khz_1_66_Khz 0x80 /* 1.66 kHz (high performance) 1.66 kHz (high performance)	*/
#define ODR_G_RATE_3_33_Khz_3_33_Khz 0x90 /* 3.33 kHz (high performance 3.33 kHz (high performance)	*/
#define ODR_G_RATE_6_66_Khz_6_66_Khz 0xA0 /* 6.66 kHz (high performance 6.66 kHz (high performance)	*/
#define FS_125 0x02

    /*Embedded functions configuration register*/
    #define FUNC_CFG_ACCESS 0x01


    #define SENSOR_SYNC_TIME_FRAME 0x04 /*Sensor sync configuration register*/
    #define SENSOR_SYNC_RES_RATIO 0x05 /*Sensor sync configuration register*/


    #define FIFO_CTRL1 0x06 /*FIFO configuration registers*/
    #define FIFO_CTRL2 0x07 /*FIFO configuration registers*/
    #define FIFO_CTRL3 0x08 /*FIFO configuration registers*/
    #define FIFO_CTRL4 0x09 /*FIFO configuration registers*/
    #define FIFO_CTRL5 0x0A /*FIFO configuration registers*/

    #define DRDY_PULSE_CFG_G 0x0B

    #define INT1_CTRL 0x0D /*INT1 pin control*/
    #define INT2_CTRL 0x0E /*INT2 pin control*/
    #define WHO_AM_I 0x0F /*Who I am ID*/

    #define CTRL1_XL 0x10 /*Accelerometer and gyroscope control register*/
    #define CTRL2_G 0x11 /*Accelerometer and gyroscope control register*/
    #define CTRL3_C 0x12 /*Accelerometer and gyroscope control register*/
    #define CTRL4_C 0x13 /*Accelerometer and gyroscope control register*/
    #define CTRL5_C 0x14 /*Accelerometer and gyroscope control register*/
    #define CTRL6_C 0x15 /*Accelerometer and gyroscope control register*/
    #define CTRL7_G 0x16 /*Accelerometer and gyroscope control register*/
    #define CTRL8_XL 0x17 /*Accelerometer and gyroscope control register*/
    #define CTRL9_XL 0x18 /*Accelerometer and gyroscope control register*/
    #define CTRL10_C 0x19 /*Accelerometer and gyroscope control register*/

    #define MASTER_CONFIG 0x1A /*I2C master configuration register*/

    #define WAKE_UP_SRC 0x1B /*Interrupt register*/
    #define TAP_SRC 0x1C /*Interrupt register*/
    #define D6D_SRC 0x1D /*Interrupt register*/

    #define STATUS_REG 0x1E /*Status data register for user interface*/

    #define OUT_TEMP_L 0x20 /*Temperature output data registers*/
    #define OUT_TEMP_H 0x21 /*Temperature output data registers*/

    #define OUTX_L_G 0x22 /* Gyroscope output register for user interface */
    #define OUTX_H_G 0x23 /* Gyroscope output register for user interface */
    #define OUTY_L_G 0x24 /* Gyroscope output register for user interface */
    #define OUTY_H_G 0x25 /* Gyroscope output register for user interface */
    #define OUTZ_L_G 0x26 /* Gyroscope output register for user interface */
    #define OUTZ_H_G 0x27 /* Gyroscope output register for user interface */

    #define OUTX_L_XL 0x28 /* Accelerometer output register */
    #define OUTX_H_XL 0x29 /* Accelerometer output register */
    #define OUTY_L_XL 0x2A /* Accelerometer output register */
    #define OUTY_H_XL 0x2B /* Accelerometer output register */
    #define OUTZ_L_XL 0x2C /* Accelerometer output register */
    #define OUTZ_H_XL 0x2D /* Accelerometer output register */

    #define SENSORHUB1_REG 0x2E  /*Sensor hub output register */
    #define SENSORHUB2_REG 0x2F  /*Sensor hub output register */
    #define SENSORHUB3_REG 0x30  /*Sensor hub output register */
    #define SENSORHUB4_REG 0x31  /*Sensor hub output register */
    #define SENSORHUB5_REG 0x32  /*Sensor hub output register */
    #define SENSORHUB6_REG 0x33  /*Sensor hub output register */
    #define SENSORHUB7_REG 0x34  /*Sensor hub output register */
    #define SENSORHUB8_REG 0x35  /*Sensor hub output register */
    #define SENSORHUB9_REG 0x36  /*Sensor hub output register */
    #define SENSORHUB10_REG 0x37 /*Sensor hub output register */
    #define SENSORHUB11_REG 0x38 /*Sensor hub output register */
    #define SENSORHUB12_REG 0x39 /*Sensor hub output register */

    #define FIFO_STATUS1 0x3A /*FIFO status register 1*/
    #define FIFO_STATUS2 0x3B /*FIFO status register 2*/
    #define FIFO_STATUS3 0x3C /*FIFO status register 3*/
    #define FIFO_STATUS4 0x3D /*FIFO status register 4*/

    #define FIFO_DATA_OUT_L 0x3E /* * FIFO data output registers L*/
    #define FIFO_DATA_OUT_H 0x3F /* * FIFO data output registers H*/

    #define TIMESTAMP0_REG 0x40  /*Timestamp output register 0*/
    #define TIMESTAMP1_REG 0x41  /*Timestamp output register 1*/
    #define TIMESTAMP2_REG 0x42  /*Timestamp output register 2*/

    #define STEP_TIMESTAMP_L 0x49 /*Step counter timestamp register L*/
    #define STEP_TIMESTAMP_H 0x4A /*Step counter timestamp register H*/

    #define STEP_COUNTER_L 0x4B /*Step counter output register L*/
    #define STEP_COUNTER_H 0x4C /*Step counter output register H*/

    #define SENSORHUB13_REG 0x4D /*Sensor hub output register 13*/
    #define SENSORHUB14_REG 0x4E /*Sensor hub output register 14*/
    #define SENSORHUB15_REG 0x4F /*Sensor hub output register 15*/
    #define SENSORHUB16_REG 0x50 /*Sensor hub output register 16*/
    #define SENSORHUB17_REG 0x51 /*Sensor hub output register 17*/
    #define SENSORHUB18_REG 0x52 /*Sensor hub output register 18*/

    #define FUNC_SRC1 0x53 /* Interrupt register 1 */
    #define FUNC_SRC2 0x54 /* Interrupt register 2 */

    /*Interrupt register*/
    #define WRIST_TILT_IA 0x55

    #define TAP_CFG 0x58     /* Interrupt register */
    #define TAP_THS_6D 0x59  /* Interrupt register */
    #define INT_DUR2 0x5A    /* Interrupt register */
    #define WAKE_UP_THS 0x5B /* Interrupt register */
    #define WAKE_UP_DUR 0x5C /* Interrupt register */
    #define FREE_FALL 0x5D   /* Interrupt register */
    #define MD1_CFG 0x5E     /* Interrupt register */
    #define MD2_CFG 0x5F     /* Interrupt register */

    #define MASTER_CMD_CODE 0x60
    #define SENS_SYNC_SPI_ERROR_CODE 0x61

    #define OUT_MAG_RAW_X_L 0x66 /*External magnetometer raw data output register*/
    #define OUT_MAG_RAW_X_H 0x67 /*External magnetometer raw data output register*/
    #define OUT_MAG_RAW_Y_L 0x68 /*External magnetometer raw data output register*/
    #define OUT_MAG_RAW_Y_H 0x69 /*External magnetometer raw data output register*/
    #define OUT_MAG_RAW_Z_L 0x6A /*External magnetometer raw data output register*/
    #define OUT_MAG_RAW_Z_H 0x6B /*External magnetometer raw data output register*/

    #define X_OFS_USR 0x73 /*Accelerometer user offset correction*/
    #define Y_OFS_USR 0x74 /*Accelerometer user offset correction*/
    #define Z_OFS_USR 0x75 /*Accelerometer user offset correction*/



typedef struct xyzDatStr{
	uint16_t valX;
	uint16_t valY;
	uint16_t valZ;
};
typedef struct gyroAccelStr{
	struct xyzDatStr gyro;
	struct xyzDatStr accel;
};

char lsm6dslTest();
char lsm6dslGXL_Read(struct xyzDatStr *acs,
		struct xyzDatStr *grs);
char lsm6dsPowerMode(char);
char lsm6dslInit(void);
/*Commands*/


#endif /* LSM6DSL_H_ */
