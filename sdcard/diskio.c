/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2009      */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* msp430 USCI support routines                                          */
/*-----------------------------------------------------------------------*/

#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include "../sdcard/diskio.h"
#include "../sdcard/pff.h"
#include "../sdcard/spi.h"
#include "../board.h"

#define DELAY_100US() delayus(100)  /* ( 100us/(1/16Mhz) )  = 1600 ticks */
//#define SELECT()    P2OUT &= ~BIT0      /* CS = L */
//#define	DESELECT()	P2OUT |= BIT0       /* CS = H */
#define	MMC_SEL		!(P2OUT & BIT0)     /* CS status (true:CS == L) */
#define	FORWARD(d)	putchar(d)  /* Data forwarding function (Console out in this example) */

/* Definitions for MMC/SDC command */
#define CMD0	(0x40+0)	/* GO_IDLE_STATE */
#define CMD1	(0x40+1)	/* SEND_OP_COND (MMC) */
#define	ACMD41	(0xC0+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(0x40+8)	/* SEND_IF_COND */
#define CMD16	(0x40+16)	/* SET_BLOCKLEN */
#define CMD17	(0x40+17)	/* READ_SINGLE_BLOCK */
#define CMD24	(0x40+24)	/* WRITE_BLOCK */
#define CMD55	(0x40+55)	/* APP_CMD */
#define CMD58	(0x40+58)	/* READ_OCR */

/* Card type flags (CardType) */
#define CT_MMC				0x01	/* MMC ver 3 */
#define CT_SD1				0x02	/* SD ver 1 */
#define CT_SD2				0x04	/* SD ver 2 */
#define CT_BLOCK			0x08	/* Block addressing */

static BYTE CardType;

/* SC, EN Ports declarations */
struct GPIO_containar_Struc cs;
struct GPIO_containar_Struc en;

/*
* sdRelease()
*	Function to release SD Card at SPI I/O level
*/
void sdRelease(){
	GPIO_setOutputHighOnPin(cs.port, cs.pin); // CS pin to H level
	__delay_cycles(1);
}

/*
 * sdSelect()
*	Function to select SD Card at SPI I/O level
*/
void sdSelect(){
	GPIO_setOutputLowOnPin(cs.port, cs.pin); // CS pin to L level
	__delay_cycles(1);
}

/*
* sdSelectStatus()
*	Check select status
*	@ 0 - not selected
*/
uint8_t sdSelectStatus(){
	uint8_t  pinState = GPIO_getInputPinValue(cs.port, cs.pin);
	pinState = pinState ? 0 : 1;
	return pinState;
}

/*
* sdPowerON()
*	Function to powered SD Card
*/
void sdPowerON(){
//	printf("ON");
	GPIO_setOutputHighOnPin(en.port, en.pin); // CS pin to H level
	delayms(500);
}


/*
* sdPowerOFF()
*	Function to unpowered SD Card
*/
void sdPowerOFF(){
//	__delay_cycles(62500);
//	printf("OFF");
	GPIO_setOutputLowOnPin(en.port, en.pin); // CS pin to L level
}

/*
* sdPowerStatus()
*	Check power status
*	@ 0 - not powered
*/
uint8_t sdPowerStatus(){
	uint8_t  pinState = GPIO_getInputPinValue(en.port, en.pin);
	pinState = pinState ? 1 : 0;
	return pinState;
}

/*
 * sdCsEnStatus()
 * 	Check Select and power status
 * 	Return 1 if power on and cs in active state
 * */
uint8_t sdCsEnStatus(){
	uint8_t sdState = sdPowerStatus() && sdSelectStatus();
	return sdState;
}

/*
 * sdInit()
 * Function of SDcard initialization in low level
 * it contain :
 * - Interface control port initializing (CS, EN),
 * - SPI initializing
 * */
void sdInit(uint8_t CS_Port,
        uint16_t CS_Pins,
		uint8_t SD_en_Port,
		uint16_t SD_en_Pins
){
	// Fill cs and en structures
	cs.port = CS_Port;
	cs.pin = CS_Pins;
	en.port = SD_en_Port;
	en.pin = SD_en_Pins;

	// Ports initializing
	sdRelease(); // Deselec at the begin

	sdPowerOFF(); // OFF power at the begin
    GPIO_setAsOutputPin(cs.port, cs.pin);

    GPIO_setAsOutputPin(en.port, en.pin);

    // SPI interface initialize
    spi_initialize();
}

/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/
static uint8_t sdCrc7(uint8_t* chr, uint8_t cnt, uint8_t crc) {
	uint8_t i, a;
	uint8_t Data;

	for (a = 0; a < cnt; a++) {
		Data = chr[a];
		for (i = 0; i < 8; i++) {
			crc <<= 1;
			if ((Data & 0x80) ^ (crc & 0x80)) { crc ^= 0x09; }
			Data <<= 1;
		}
	}
	return ((crc & 0x7F) << 1) | 1;
}

static BYTE send_cmd(BYTE cmd, /* 1st byte (Start + Index) */
DWORD arg /* Argument (32 bits) */
) {
    BYTE n, res, i;
    BYTE cmdtoSd[5];
    if (cmd & 0x80) { /* ACMD<n> is the command sequense of CMD55-CMD<n> */
        cmd &= 0x7F;
        res = send_cmd(CMD55, 0);
        if (res > 1)
            return res;
    }

    /* Select the card */
    sdRelease(); // ???
    spi_receive();//??

    sdSelect();
    spi_receive();

    /* Send a command packet */
    cmdtoSd[0] = cmd;
    cmdtoSd[1] = arg >> 24;	/* Argument[31..24] */
    cmdtoSd[2] = arg >> 16;	/* Argument[23..16] */
    cmdtoSd[3] = arg >> 8;	/* Argument[15..8] 	*/
    cmdtoSd[4] = arg;		/* Argument[7..0] 	*/

    for(i = 0; i<sizeof(cmdtoSd);i++)
    	spi_send(cmdtoSd[i]);

    n = 0x01; /* Dummy CRC + Stop */
    if (cmd == CMD0)
        n = 0x95; /* Valid CRC for CMD0(0) */
    if (cmd == CMD8)
        n = 0x87; /* Valid CRC for CMD8(0x1AA) */
    else
    	n = sdCrc7(cmdtoSd, 5, 0);

    spi_send(n);

    /* Receive a command response */
    n = 10; /* Wait for a valid response in timeout of 10 attempts */
    do {
        res = spi_receive();
    } while ((res & 0x80) && --n);

    return res; /* Return with the response value */
}

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DRESULT ___disk_initilize___(void) {
    BYTE n, cmd, ty, ocr[4];
    UINT tmr;
    //sdSelect();
    n = 255;
    do{
    __delay_cycles(4000);
	}while(--n);

    sdPowerON();
#if _USE_WRITE
    //if (CardType && MMC_SEL)
    if (CardType && sdCsEnStatus())
        disk_writep(0, 0); /* Finalize write process if it is in progress */
#endif

    sdRelease();
    for (n = 10; n; n--)
        spi_receive(); /* 80 dummy clocks with CS=H */
    sdSelect();

    ty = 0;
    if (send_cmd(CMD0, 0) == 1) { /* Enter Idle state */
        if (send_cmd(CMD8, 0x1AA) == 1) { /* SDv2 */
            for (n = 0; n < 4; n++)
                ocr[n] = spi_receive(); /* Get trailing return value of R7 resp */
            if (ocr[2] == 0x01 && ocr[3] == 0xAA) { /* The card can work at vdd range of 2.7-3.6V */
                for (tmr = 10000; tmr && send_cmd(ACMD41, 1UL << 30); tmr--)
                    DELAY_100US(); /* Wait for leaving idle state (ACMD41 with HCS bit) */
                if (tmr && send_cmd(CMD58, 0) == 0) { /* Check CCS bit in the OCR */
                    for (n = 0; n < 4; n++)
                        ocr[n] = spi_receive();
                    ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2; /* SDv2 (HC or SC) */
                }
            }
        } else { /* SDv1 or MMCv3 */
            if (send_cmd(ACMD41, 0) <= 1) {
                ty = CT_SD1;
                cmd = ACMD41; /* SDv1 */
            } else {
                ty = CT_MMC;
                cmd = CMD1; /* MMCv3 */
            }
            for (tmr = 10000; tmr && send_cmd(cmd, 0); tmr--)
                DELAY_100US(); /* Wait for leaving idle state */
            if (!tmr || send_cmd(CMD16, 512) != 0) /* Set R/W block length to 512 */
                ty = 0;
        }
    }
    CardType = ty;
    while(1);
    sdRelease();
    spi_receive();

    return ty ? 0 : STA_NOINIT;
}
/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DRESULT disk_initilize___(void) {
    BYTE resp, n, cmd, ty, ocr[4];
    UINT tmr;

    sdPowerON();
#if _USE_WRITE
    //if (CardType && MMC_SEL)
    if (CardType && sdCsEnStatus())
        disk_writep(0, 0); /* Finalize write process if it is in progress */
#endif

    sdRelease();
    for (n = 10; n; n--)
        spi_receive(); /* 80 dummy clocks with CS=H */

    sdSelect();

    ty = 0;

    tmr = 100;
    do{
    	resp = send_cmd(CMD0, 0);
    	DELAY_100US();
    }while((resp != 1) && tmr--);

    if (tmr) { /* Enter Idle state */
        resp = send_cmd(CMD8, 0x1AA);
    	if (resp == 1) { /* SDv2 */
            for (n = 0; n < 4; n++)
                ocr[n] = spi_receive(); /* Get trailing return value of R7 resp */

            if (ocr[2] == 0x01 && ocr[3] == 0xAA) { /* The card can work at vdd range of 2.7-3.6V */
                do{
                	resp = send_cmd(ACMD41, 1UL << 30);
                	DELAY_100US();
                }while(resp && tmr--);
            	//for (tmr = 10000; tmr && send_cmd(ACMD41, 1UL << 30); tmr--)
                //    DELAY_100US(); /* Wait for leaving idle state (ACMD41 with HCS bit) */

                if (tmr && send_cmd(CMD58, 0) == 0) { /* Check CCS bit in the OCR */
                    for (n = 0; n < 4; n++)
                        ocr[n] = spi_receive();

                    ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2; /* SDv2 (HC or SC) */
                }

            }
        } else { /* SDv1 or MMCv3 */
            if (send_cmd(ACMD41, 0) <= 1) {
                ty = CT_SD1;
                cmd = ACMD41; /* SDv1 */
            } else {
                ty = CT_MMC;
                cmd = CMD1; /* MMCv3 */
            }
            for (tmr = 10000; tmr && send_cmd(cmd, 0); tmr--)
                DELAY_100US(); /* Wait for leaving idle state */
            if (!tmr || send_cmd(CMD16, 512) != 0) /* Set R/W block length to 512 */
                ty = 0;
        }
    }

    CardType = ty;

    if(ty)
    	send_cmd(0x40+16, 0x00000200);

    sdRelease();
    spi_receive();

    return ty ? 0 : STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read partial sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp(
		BYTE *buff, /* Pointer to the read buffer (NULL:Read bytes are forwarded to the stream) */
		DWORD lba, /* Sector number (LBA) */
		WORD ofs, /* Byte offset to read from (0..511) */
		WORD cnt /* Number of bytes to read (ofs + cnt mus be <= 512) */
) {
    DRESULT res;
    BYTE rc;
    WORD bc;
    BYTE cmdRet;

    if (!(CardType & CT_BLOCK))
        lba *= 512; /* Convert to byte address if needed */

    res = RES_ERROR;
    cmdRet = send_cmd(CMD17, lba);
    lba++;
    lba--;


    if (cmdRet == 0) { /* READ_SINGLE_BLOCK */

        bc = 40000;
        do { /* Wait for data packet */
            rc = spi_receive();
        } while (rc == 0xFF && --bc);

        if (rc == 0xFE) { /* A data packet arrived */
            bc = 514 - ofs - cnt;

            /* Skip leading bytes */
            if (ofs) {
                do
                    spi_receive();
                while (--ofs);
            }

            /* Receive a part of the sector */
            if (buff) { /* Store data to the memory */
                do {
                    *buff++ = spi_receive();
                } while (--cnt);
            } else { /* Forward data to the outgoing stream (depends on the project) */
                do {
                    FORWARD(spi_receive());
                } while (--cnt);
            }

            /* Skip trailing bytes and CRC */
            do
                spi_receive();
            while (--bc);

            res = RES_OK;
        }
    }

    sdRelease();
    spi_receive();

    return res;
}

/*-----------------------------------------------------------------------*/
/* Write partial sector                                                  */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_writep(const BYTE *buff, /* Pointer to the bytes to be written (NULL:Initiate/Finalize sector write) */
DWORD sa /* Number of bytes to send, Sector number (LBA) or zero */
) {
    DRESULT res;
    WORD bc;
    static WORD wc;

    res = RES_ERROR;

    if (buff) { /* Send data bytes */
        bc = (WORD) sa;
        while (bc && wc) { /* Send data bytes to the card */
            spi_send(*buff++);
            wc--;
            bc--;
        }
        res = RES_OK;
    } else {
        if (sa) { /* Initiate sector write process */
            if (!(CardType & CT_BLOCK))
                sa *= 512; /* Convert to byte address if needed */
            if (send_cmd(CMD24, sa) == 0) { /* WRITE_SINGLE_BLOCK */
                spi_send(0xFF);
                spi_send(0xFE); /* Data block header */
                wc = 512; /* Set byte counter */
                res = RES_OK;
            }
        } else { /* Finalize sector write process */
            bc = wc + 2;
            while (bc--)
                spi_send(0); /* Fill left bytes and CRC with zeros */
            if ((spi_receive() & 0x1F) == 0x05) { /* Receive data resp and wait for end of write process in timeout of 500ms */
                for (bc = 5000; spi_receive() != 0xFF && bc; bc--)
                    DELAY_100US(); /* Wait ready */
                if (bc)
                    res = RES_OK;
            }
            sdRelease();
            spi_receive();
        }
    }

    return res;
}
#endif
