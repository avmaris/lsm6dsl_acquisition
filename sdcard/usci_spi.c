/**
 * File: usci_spi.c - msp430 USCI SPI implementation
 *
 */
#include "driverlib.h"
#include <stdint.h>
#include "../sdcard/spi.h"
#include "../board.h"

//#ifndef __MSP430_HAS_USCI__
//#error "Error! This MCU doesn't have a USCI peripheral"
//#endif
// Prototypes of TX/RX wait functions


/**
 * USCI flags for various the SPI MODEs
 *
 * Note: The msp430 UCCKPL tracks the CPOL value. However,
 * the UCCKPH flag is inverted when compared to the CPHA
 * value described in Motorola documentation.
 */

#define SPI_MODE_0 (UCMSB | UCMST | UCSYNC | UCCKPH)			/* CPOL=0 CPHA=0 */
#define SPI_MODE_1 (UCMSB | UCMST | UCSYNC)         			/* CPOL=0 CPHA=1 */
#define SPI_MODE_2 (UCMSB | UCMST | UCSYNC | UCCKPL | UCCKPH)	/* CPOL=1 CPHA=0 */
#define SPI_MODE_3 (UCMSB | UCMST | UCSYNC | UCCKPL)			/* CPOL=1 CPHA=1 */

/**
 * utility macros for extracting hi/lo byte data from a word value
 */
#ifndef LOBYTE
#define LOBYTE(w) ((w)&0xFF)
#define HIBYTE(w) ((w)>>8)
#endif

/**
 * spi_initialize() - Configure USCI UCB0 for SPI mode
 *
 * P2.0 - CS (active low)
 * P1.5 - SCLK
 * P1.6 - SIMO/MOSI
 * P1.7 - SOMI/MISO
 */
void spi_initialize(void) {
	// Configure Pins for UCA_X_TXD/UCA_X_SIMO, UCA_X_RXD/UCA_X_SOMI, UCA_X_CLK
    //Set pins as Secondary Module Function Input.
    GPIO_setAsPeripheralModuleFunctionInputPin(
    	UCA_X_SIMO_SOMI_CLK_PORT,
		UCA_X_CLK + UCA_X_SIMO_PIN + UCA_X_SOMI_PIN,
        GPIO_SECONDARY_MODULE_FUNCTION
        );
    // Configure Pins for UCA0TXD/UCA0SIMO, UCA0RXD/UCA0SOMI
    //Set P2.0, P2.1 as Secondary Module Function Input.

    //Initialize Master
    EUSCI_A_SPI_initMasterParam param = {0};
    param.selectClockSource = EUSCI_A_SPI_CLOCKSOURCE_ACLK;
    param.clockSourceFrequency = CS_getACLK();
    param.desiredSpiClock = 250000;
    param.msbFirst = EUSCI_A_SPI_MSB_FIRST;
    param.clockPhase = EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    param.clockPolarity = EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
    param.spiMode = EUSCI_A_SPI_3PIN;
    EUSCI_A_SPI_initMaster(EUSCI_A1_BASE, &param);

    //Enable SPI module
    EUSCI_A_SPI_enable(EUSCI_A1_BASE);

    EUSCI_A_SPI_clearInterrupt(EUSCI_A1_BASE,
                               EUSCI_A_SPI_RECEIVE_INTERRUPT + EUSCI_A_SPI_TRANSMIT_INTERRUPT);
    // Enable USCI_A0 RX interrupt
//    EUSCI_A_SPI_enableInterrupt(EUSCI_A1_BASE,
//                                EUSCI_A_SPI_RECEIVE_INTERRUPT + EUSCI_A_SPI_TRANSMIT_INTERRUPT);

    //Wait for slave to initialize
    __delay_cycles(100);
}
/**
 * spi_send() - send a byte and recv response
 * Logic of sending token from an example and could be improved.
 */
uint8_t spi_send(const uint8_t c) {
	uint8_t RXDataAnsfer = 0;

    //spi_waitTX(); // wait for previous tx to complete
	spi_wait();
    EUSCI_A_SPI_transmitData(EUSCI_SPI_BASE, c); //Transmit Data to slave
    spi_waitRX(); // wait for an rx character?
    RXDataAnsfer = EUSCI_A_SPI_receiveData(EUSCI_SPI_BASE);
    //printf("SEND: s.%x r.%x\n", c, RXDataAnsfer);
    return RXDataAnsfer; // read data
}

/**
 * spi_receive() - send dummy btye then recv response
 * Logic of reading token from an example and could be improved.
 */
uint8_t spi_receive(void) {
	uint8_t RXData__ = 0;

	//spi_waitTX(); // wait for any previous xmits to complete
	spi_wait();
	EUSCI_A_SPI_transmitData(EUSCI_SPI_BASE, 0xFF); // Send dummy packet to get data back.

	spi_waitRX(); //
	RXData__ = EUSCI_A_SPI_receiveData(EUSCI_SPI_BASE);
    return RXData__;
}

/* spi_waitTX()
 * Wait when USCI_A1 TX buffer will ready
 *
 * Function return 0 or TIMEOUT
 * */
uint8_t spi_waitTX(){
    while(!EUSCI_A_SPI_getInterruptStatus(EUSCI_SPI_BASE,
    		EUSCI_A_SPI_TRANSMIT_INTERRUPT))
    {
        // TODO: timer to apply timeout
    }
    return 0;
}

uint8_t spi_wait(){
    while(EUSCI_A_SPI_isBusy(EUSCI_SPI_BASE))
    {
        // TODO: timer to apply timeout
    }
    return 0;
}

/* spi_waitRX()
 * Wait when USCI_A1 TX buffer will ready
 *
 * Function return 0 or TIMEOUT
 * */
uint8_t spi_waitRX(){
    while(!EUSCI_A_SPI_getInterruptStatus(EUSCI_SPI_BASE,
    		EUSCI_A_SPI_RECEIVE_INTERRUPT))
    {
        // TODO: timer to apply timeout
    }
    return 0;
}

