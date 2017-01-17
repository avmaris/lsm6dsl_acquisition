/*
 * board.h
 *
 *  Created on: Nov 16, 2016
 *      Author: Marisov
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "driverlib.h"
#define CPU_FRQ 1000000

#define X_TAL 32768
#define delayms(x) __delay_cycles(x * CPU_FRQ/1000)
#define delayus(x) __delay_cycles(x * CPU_FRQ/1000000)

/*
 * Main processes timer defines
 * */
#define TIME_PERIOD_MS 40
#define PREDEFINE_TA_DIV TIMER_A_CLOCKSOURCE_DIVIDER_64
#define PREDEFINE_TA_DIV_VAL 64
//((TIME_PERIOD_MS * CPU_FRQ/(PREDEFINE_TA_DIV_VAL*1000)))
#define COMPARE_VALUE 625

/* Led D1 defines */
#define LED1_PORT GPIO_PORT_P3
#define LED1_PIN GPIO_PIN7

/* SD-Card CS defines */
#define SPI_CS_PORT GPIO_PORT_P2
#define SPI_CS_PIN GPIO_PIN7

/* SD-Card Power on defines */
#define SD_EN_PORT GPIO_PORT_P2
#define SD_EN_PIN GPIO_PIN0

/*SPI*/
#define EUSCI_SPI_BASE EUSCI_A1_BASE

#define UCA_X_SIMO_SOMI_CLK_PORT GPIO_PORT_P2
#define UCA_X_CLK GPIO_PIN4
#define UCA_X_SIMO_PIN GPIO_PIN5
#define UCA_X_SOMI_PIN GPIO_PIN6

/*I2C Interface*/
/*
 * for soft i2c
 * */
#define I2C_PORT P1OUT
#define I2C_DIR   P1DIR
#define I2C_PINC P1IN
#define I2C_SDA_PIN 6
#define I2C_SCL_PIN 7
//
#define UCX_I2C_PORT GPIO_PORT_P1
#define UCX_I2C_SDA_PIN GPIO_PIN6
#define UCX_I2C_SCL_PIN GPIO_PIN7


/*LMT70*/
#define TEMP_TON_PORT GPIO_PORT_P3
#define TEMP_PWR_PORT GPIO_PORT_P3
#define TEMP_TAO_PORT GPIO_PORT_P3

#define TEMP_TON_PIN GPIO_PIN0
#define TEMP_PWR_PIN GPIO_PIN1
#define TEMP_TAO_PIN GPIO_PIN2

#define TEMP_TON_AN A12
#define TEMP_PWR_AN A13
#define TEMP_TAO_AN ADC10_B_INPUT_A14

#define UCX_I2C_SDA_PIN GPIO_PIN6
#define UCX_I2C_SCL_PIN GPIO_PIN7


#define BLINK_PERIOD 6 /*Period of batt. led blinking*/
#define BLINK_HI_TIME 1 /*Time when led in H level*/

#define BLINK_CLK_DIV TIMER_B_CLOCKSOURCE_DIVIDER_64
#define BLINK_CLK_DIV_VAL 64
#define BLINK_TIMER_PERIOD (BLINK_PERIOD * (X_TAL/BLINK_CLK_DIV_VAL))
#define BLINK_HI_TIME_CMP_VAL (BLINK_HI_TIME * X_TAL/BLINK_CLK_DIV_VAL)

//
#define VBAT_TH 2600 /*In which mV led diode start to blink*/
enum {ON = 0, OFF, CHANGE};
struct GPIO_containar_Struc {
	uint8_t port;
	uint16_t pin;
};

//#define DATA_STORAGE_BASE_ADR 0xCA00
#define DATA_STORAGE_SIZE 1024

//13695

enum {OK_RETURN = 0, ERROR};

#endif /* BOARD_H_ */
